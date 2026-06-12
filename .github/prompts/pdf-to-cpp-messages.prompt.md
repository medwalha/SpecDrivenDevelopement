---
agent: agent
description: Génère des structures de messages C++ (style maison) à partir d'un PDF de spécification (ICD / protocole binaire).
---

# Génération de structures de messages C++ depuis un PDF

Ton rôle : analyser un **PDF de spécification** (ICD, protocole binaire, format de trame) et générer des **messages C++ au style maison** :

- struct binaire `<Name>Struct` packée (`#pragma pack(push,1)`) avec champs `m_PascalCase`,
- typedefs courts (`uint8`, `int64`, ...) et **types réutilisables** issus de `include/messages/types/` (`Decimal`, `TimestampNano`, `Isin`, `Mic`, `LogoutCode`, `MdEntryType`, `TradeTypeBitmap`, ...) — **pas de `std::uint*_t`**,
- classe wrapper `<Name>` qui :
  - contient `MessageHeader const m_MessageHeader` et `<Name>Struct const & m_Struct`,
  - expose `static constexpr uint16 TYPE`, `const char* NAME`, `unsigned int SIZE`,
  - construit avec `explicit <Name>(InputData const & data)` et un `reinterpret_cast` sur le buffer,
  - fournit un getter par champ (`std::string` pour les `char[N]`),
  - implémente `virtual void dump(std::ostream&) const` avec les macros `DUMP_FIELD` / `DUMP_FIELD_AS_INT`,
- `inline std::ostream& operator<<(...)` qui appelle `dump()`.

> **Référence obligatoire** : [.github/instructions/cpp-messages.instructions.md](../instructions/cpp-messages.instructions.md).
> **Exemple canonique** : [examples/PostTrade.hpp](../../examples/PostTrade.hpp).
> **Template** : [templates/Message.hpp.template](../../templates/Message.hpp.template).
> **Workflow types** : [.github/prompts/pdf-to-cpp-types.prompt.md](pdf-to-cpp-types.prompt.md).

## ⚠️ Règle d'or — Types réutilisables UNIQUEMENT

Les **structs de messages** doivent référencer **exclusivement** :

1. Les **typedefs scalaires de base** déclarés dans `include/messages/Common.hpp` :
   `uint8`, `int8`, `uint16`, `int16`, `uint32`, `int32`, `uint64`, `int64`.
2. Les **types extraits de la spec** consolidés dans `include/messages/types/` :
   - `Aliases.hpp` (chaînes ASCII fixes, identifiants entiers : `Isin`, `Mic`, `Currency`, `SeqNum`, `InternalIdentifier`, `Username`, `Password`, ...).
   - `Enums.hpp` (énumérations : `LogoutCode`, `MdEntryType`, `VenueType`, ...).
   - `Bitmaps.hpp` (bitmaps : `TradeTypeBitmap`, `QuoteConditionsBitmap`, ...).
   - `Structs.hpp` (composés : `Decimal`, `TimestampNano`, `RptGrpHeader`, `FramingHeader`, `UnitHeader`, ...).

❌ **Interdits dans `<Name>Struct`** :
- ❌ Tout `using ... = ...;` ad-hoc local au fichier message.
- ❌ Toute déclaration `enum class` ou `struct` interne.
- ❌ Tout `char m_Foo[N]` brut **si** un alias dédié existe (ex: utiliser `Isin` au lieu de `char m_SecurityID[12]` quand `Isin = char[12]` est déjà défini).
- ❌ Toute redéclaration ou copie d'un type déjà présent dans `types/`.

✅ **Procédure si un type manque** :
- Identifie la section du PDF qui décrit ce type.
- **Stoppe la génération du message** et invite l'utilisateur à exécuter `/pdf-to-cpp-types` pour cette section avec la catégorie adaptée (`alias` / `enum` / `bitmap` / `struct`).
- Reprends la génération du message **uniquement** une fois que le type est ajouté au fichier consolidé correspondant.

✅ **Convention de référencement** : dans le `<Name>Struct`, utilise les types **non qualifiés** quand ils ont un alias re-exporté dans `protocol::messages` (cas de `Decimal`, `TimestampNano`), sinon préfixe avec `types::` (ex: `types::RptGrpHeader m_PartiesHeader;`).

## Étape 0 — Pré-requis OBLIGATOIRE : `MessageHeader` & `PacketHeader`

**Avant toute génération de message**, demande systématiquement à l'utilisateur :

1. **`MessageHeader`** : quel est son layout ?
   - Champs (nom, type, taille) ?
   - Taille totale (`MessageHeader::SIZE`) ?
   - Quels champs doivent apparaître dans `MessageHeader::dump()` ?
   - Existe-t-il déjà un fichier `include/messages/MessageHeader.hpp` à réutiliser ?
   - **Préférer une composition** de structs déjà présentes dans `types/Structs.hpp` (ex: `FramingHeader` + `UnitHeader`) plutôt que de redéclarer les champs.

2. **`PacketHeader`** : le protocole encapsule-t-il les messages dans un paquet de transport ?
   - Si **oui** : layout, taille, et est-ce que plusieurs `MessageHeader` peuvent suivre un seul `PacketHeader` ?
   - Si **non** : confirme explicitement « pas de PacketHeader ».

3. **Typedefs domaine** : confirme que `Decimal`, `TimestampNano`, `InputData` correspondent bien aux types réels du projet (sinon adapter `include/messages/Common.hpp` et/ou les définitions sous `types/Structs.hpp`).

Tant que ces réponses ne sont pas fournies, **n'écris aucun message**. Les stubs actuels :
- [include/messages/MessageHeader.hpp](../../include/messages/MessageHeader.hpp)
- [include/messages/PacketHeader.hpp](../../include/messages/PacketHeader.hpp)

doivent être adaptés (ou remplacés) avec les informations utilisateur **avant** de générer les messages.

## Entrée du PDF

L'utilisateur fournit :
- un chemin vers un fichier PDF (ex: `specs/protocol_v1.pdf`), OU
- le contenu du PDF déjà extrait (texte / tableaux), OU
- une page / section spécifique du PDF.

Si le PDF n'est pas accessible directement, **demande à l'utilisateur** d'extraire le texte (ex: `pdftotext -layout`, copier-coller, OCR) avant de continuer.

## Étapes

### 1. Inventaire des messages
Liste tous les messages identifiés : nom, ID/opcode, taille payload, direction (TX/RX), description courte. Présente l'inventaire en tableau Markdown et **demande confirmation** avant de générer du code.

### 2. Analyse de chaque message — vérification des types
Pour chaque message, identifie :
- **Nom** PascalCase (sans suffixe `Message` sauf si la spec l'utilise — ex: `PostTrade`).
- **ID / opcode** (constante `MSG_ID_<UPPER_SNAKE>`).
- **Champs** dans l'ordre, avec leur **mapping vers un type existant** :

  | Champ spec | Type spec | Type C++ à utiliser | Origine |
  |------------|-----------|---------------------|---------|
  | `MsgSeqNum` | int64 | `int64` | `Common.hpp` |
  | `SecurityID` | Alphanumerical 12 | `char m_SecurityID[12]` (équivalent à `Isin`) | `types/Aliases.hpp` |
  | `MDEntryPx` | Decimal | `Decimal` | `types/Structs.hpp` |
  | `LogoutCode` | uint16 | `uint16` (valeurs validées via `types::LogoutCode` côté logique métier) | `types/Enums.hpp` |
  | `TradeType` | bitmap uint8 | `uint8` (masques `TRADE_TYPE_*_MASK`) | `types/Bitmaps.hpp` |

  > **Note** : pour les enums et bitmaps, le **layout binaire reste l'entier sous-jacent** (`uint8`/`uint16`) ; l'enum class et les masques fournissent la **sémantique** mais pas le type effectif du champ. C'est un trade-off explicite du style maison (cohérent avec `examples/PostTrade.hpp`). Documente toutefois le type sémantique en commentaire (`/// see types::LogoutCode`).
- **Champs réservés / padding** : `m_Reserved_<n>`.
- **CRC / checksum** éventuels.

**Si un type manque** :
- Vérifie d'abord dans les 4 fichiers consolidés `include/messages/types/{Aliases,Enums,Bitmaps,Structs}.hpp`.
- Sinon, **stoppe** et demande à l'utilisateur de lancer `/pdf-to-cpp-types` pour cette section. Ne pas inventer de type ad-hoc dans le fichier message.

### 3. Génération du code C++
Pour chaque message validé, produis :
- `include/messages/<Name>.hpp` à partir du [template](../../templates/Message.hpp.template), en respectant **strictement** :
  - `#pragma pack(push, 1)` autour de `<Name>Struct`,
  - typedefs maison + types `protocol::messages::types::*` **uniquement**,
  - préfixe `m_` sur les champs,
  - classe wrapper avec `MessageHeader const m_MessageHeader` + `reinterpret_cast` sur le payload,
  - constantes `TYPE` / `NAME` / `SIZE`,
  - getters (`std::string` pour `char[N]`, retour direct sinon),
  - `dump()` avec `DUMP_FIELD_AS_INT` pour `uint8` / `int8`, `DUMP_FIELD` sinon,
  - `operator<<` inline.
- **Includes minimaux** : un `#include "messages/types/Types.hpp"` (préféré, agrégateur) **OU** `#include "messages/types/Aliases.hpp"` / `Enums.hpp` / `Bitmaps.hpp` / `Structs.hpp` selon ce qui est utilisé. Pas d'`#include "messages/types/<NomType>.hpp"` individuel — ces fichiers n'existent plus.
- Mise à jour de `include/messages/MessageIds.hpp` avec le nouvel opcode.

### 4. Validation à la compilation
Pour **chaque** struct, ajoute :
```cpp
static_assert(sizeof(<Name>Struct) == EXPECTED_PAYLOAD_SIZE,
              "<Name>Struct size mismatch with spec");
static_assert(std::is_standard_layout<<Name>Struct>::value, "must be standard-layout");
static_assert(std::is_trivially_copyable<<Name>Struct>::value, "must be trivially copyable");
```

### 5. Récapitulatif final
Termine par :
- liste des fichiers créés (liens cliquables),
- liste des **types réutilisés** depuis `types/` (et flag explicite si un type a dû être ajouté en cours de route),
- hypothèses prises (endianness, padding, types ambigus),
- points qui nécessitent une revue humaine (`// TODO:` dans le code),
- une commande de compilation suggérée (ex: `g++ -std=c++17 -Wall -Iinclude include/messages/<Name>.hpp -fsyntax-only`).

## Règles strictes

- ❌ **Ne jamais** utiliser `std::uint8_t`, `unsigned int`, `int`, `long` dans une struct de message → typedefs maison uniquement.
- ❌ **Ne jamais** utiliser `snake_case` ou supprimer le préfixe `m_` sur les champs.
- ❌ **Ne jamais** copier le payload — toujours `reinterpret_cast` + référence const.
- ❌ **Ne jamais** inventer un champ absent du PDF (marquer `// TODO: à clarifier`).
- ❌ **Ne jamais** générer plus de 5 messages d'un coup sans confirmation utilisateur.
- ❌ **Ne jamais** redéclarer un type (alias / enum / bitmap / struct) déjà présent dans `types/`.
- ❌ **Ne jamais** créer ou inclure un fichier `types/<NomType>.hpp` individuel — utilisation des 4 fichiers consolidés uniquement.
- ✅ **Toujours** préserver le mapping binaire 1:1 avec la spec (ordre, taille, endianness).
- ✅ **Toujours** demander `MessageHeader` et `PacketHeader` à l'étape 0.
- ✅ **Toujours** mapper chaque champ vers un type existant dans `types/` ; si manquant, stopper et invoquer `/pdf-to-cpp-types`.

## Variables

- Chemin PDF : ${input:pdfPath:Chemin du PDF (ou texte extrait)}
- Namespace : ${input:cppNamespace:protocol::messages}
- Standard C++ : ${input:cppStandard:c++17}
- Endianness : ${input:endianness:little}

## Démarrage

1. **Demander `MessageHeader` et `PacketHeader`** (étape 0) — bloquant.
2. Confirmer la lecture du PDF.
3. **Vérifier** que les 4 fichiers consolidés `types/{Aliases,Enums,Bitmaps,Structs}.hpp` existent et couvrent les types nécessaires (sinon → `/pdf-to-cpp-types`).
4. Produire l'inventaire des messages avec **mapping de chaque champ vers un type existant**.
5. Attendre validation avant de générer le code.
