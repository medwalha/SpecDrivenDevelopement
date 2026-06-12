---
agent: agent
description: Génère des structures de messages C++ (style maison) à partir d'un PDF de spécification (ICD / protocole binaire).
---

# Génération de structures de messages C++ depuis un PDF

Ton rôle : analyser un **PDF de spécification** (ICD, protocole binaire, format de trame) et générer des **messages C++ au style maison** :

- struct binaire `<Name>Struct` packée (`#pragma pack(push,1)`) avec champs `m_PascalCase`,
- typedefs courts (`uint8`, `int64`, ...) de `Common.hpp`, chaînes ASCII fixes en `char[N]` brut, **stubs domaine** (`Decimal`, `TimestampNano`) de `Common.hpp`, et **structs composés** (`RptGrpHeader`, `FramingHeader`, ...) de `types/Structs.hpp` — **pas de `std::uint*_t`**,
- classe wrapper `<Name>` qui :
  - contient `MessageHeader const m_MessageHeader` et `<Name>Struct const & m_Struct`,
  - expose `static constexpr uint16 TYPE`, `const char* NAME`, `unsigned int SIZE`,
  - construit avec `explicit <Name>(InputData const & data)` et un `reinterpret_cast` sur le buffer,
  - fournit un getter par champ (`std::string` pour les `char[N]`),
  - implémente `virtual void dump(std::ostream&) const` avec les macros `DUMP_FIELD` / `DUMP_FIELD_AS_INT`,
- `inline std::ostream& operator<<(...)` qui appelle `dump()`.

> **Référence obligatoire** : [.github/instructions/cpp-messages.instructions.md](../instructions/cpp-messages.instructions.md).
> **Glossaire & ADR** : [CONTEXT.md](../../CONTEXT.md) et [docs/adr/0001-message-struct-uses-raw-primitives.md](../../docs/adr/0001-message-struct-uses-raw-primitives.md) — la règle "primitifs bruts dans les champs de message" y est documentée.
> **Exemple canonique** : [examples/PostTrade.hpp](../../examples/PostTrade.hpp).
> **Template** : [templates/Message.hpp.template](../../templates/Message.hpp.template).
> **Workflow types** : [.github/prompts/pdf-to-cpp-types.prompt.md](pdf-to-cpp-types.prompt.md).

## ⚠️ Règle d'or — Primitifs bruts dans les champs de message (ADR-0001)

Les **champs** d'une `<Name>Struct` se déclarent **uniquement** avec :

1. Les **typedefs scalaires maison** de [include/messages/Common.hpp](../../include/messages/Common.hpp) :
   `uint8`, `int8`, `uint16`, `int16`, `uint32`, `int32`, `uint64`, `int64`.
2. Les **chaînes ASCII fixes** en brut : `char m_Field[N];` — **pas** de typedef-alias intermédiaire (`Isin`, `Mic`, ... sont interdits).
3. Les **stubs domaine** définis dans `Common.hpp` (à remplacer par le projet consommateur) :
   - `Decimal` (prix / quantité),
   - `TimestampNano` (horodatage nanoseconde).
4. Les **structs composés** publiés dans [include/messages/types/Structs.hpp](../../include/messages/types/Structs.hpp) (ex: `RptGrpHeader`, `FramingHeader`, `UnitHeader`) — **seule** catégorie de `types/` dont un nom est autorisé en tant que type de champ.

Les **enums** ([include/messages/types/Enums.hpp](../../include/messages/types/Enums.hpp)) et **bitmaps** ([include/messages/types/Bitmaps.hpp](../../include/messages/types/Bitmaps.hpp)) sont **disponibles** pour la validation sémantique côté logique métier, mais le champ qui *porte* leurs octets reste typé en entier primitif sous-jacent (`uint8` / `uint16`). C'est l'arbitrage central d'ADR-0001 : mapping binaire 1:1 visible dans la struct, sémantique exposée à côté.

❌ **Interdits dans `<Name>Struct`** :
- ❌ Tout `using ... = ...;` ad-hoc local au fichier message.
- ❌ Toute déclaration `enum class` ou `struct` interne.
- ❌ Toute référence à un nom d'enum ou de bitmap (`types::LogoutCode`, `types::TradeTypeBitmap`, ...) en tant que **type de champ** — utiliser le primitif sous-jacent (`uint16`, `uint8`) et commenter `/// see types::<Name>`.
- ❌ Toute référence à `Aliases.hpp` (le fichier n'existe pas et est interdit — voir [pdf-to-cpp-types](pdf-to-cpp-types.prompt.md)).
- ❌ Toute redéclaration ou copie d'un type déjà présent dans `Structs.hpp` (les composés se référencent par nom).

✅ **Procédure si un type composé / enum / bitmap manque** :
- Identifie la section du PDF qui le décrit.
- **Stoppe la génération du message** et invite l'utilisateur à exécuter `/pdf-to-cpp-types` pour cette section avec la catégorie adaptée (`enum` / `bitmap` / `struct`).
- Reprends la génération du message **uniquement** une fois que le type est ajouté au fichier consolidé correspondant.
- ⚠️ **Ne déclenche jamais ce blocage pour un `char[N]` ou un entier** — ces formes se déclarent directement dans la struct, sans détour par `types/`.

✅ **Convention de référencement** :
- `Decimal`, `TimestampNano` → non qualifiés (définis dans `protocol::messages` via `Common.hpp`).
- Composés de `Structs.hpp` → préfixés `types::` (ex: `types::RptGrpHeader m_PartiesHeader;`).
- Enums / bitmaps → **jamais** utilisés comme type de champ. Le commentaire à côté du champ documente la sémantique : `uint16 m_LogoutCode; /// see types::LogoutCode`.

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

3. **Typedefs domaine** : confirme que `Decimal`, `TimestampNano`, `InputData` correspondent bien aux types réels du projet. Ces trois types sont des **stubs domaine** **définis dans [include/messages/Common.hpp](../../include/messages/Common.hpp)** (avec un `// TODO: replace` explicite), à remplacer par le projet consommateur. Ils ne sont **pas** dans `types/Structs.hpp` (ce dossier est réservé aux structs composés extraits de la spec PDF).

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
- **Champs** dans l'ordre, avec leur **mapping vers le type C++ correct** (ADR-0001 — primitif brut au site du champ) :

  | Champ spec | Type spec | Type C++ à utiliser | Origine |
  |------------|-----------|---------------------|---------|
  | `MsgSeqNum` | int64 | `int64 m_MsgSeqNum;` | `Common.hpp` |
  | `SecurityID` | Alphanumerical 12 | `char m_SecurityID[12];` (brut — pas de typedef-alias) | en ligne |
  | `MDEntryPx` | Decimal | `Decimal m_MDEntryPx;` | `Common.hpp` (stub domaine) |
  | `TransactTime` | Timestamp ns | `TimestampNano m_TransactTime;` | `Common.hpp` (stub domaine) |
  | `LogoutCode` | uint16 (enum) | `uint16 m_LogoutCode; /// see types::LogoutCode` | `types/Enums.hpp` (sémantique) |
  | `TradeType` | bitmap uint8 | `uint8 m_TradeType; /// see types::TradeTypeBitmap, masques TRADE_TYPE_*_MASK` | `types/Bitmaps.hpp` (sémantique) |
  | `PartiesHeader` | RptGrpHeader (struct composé) | `types::RptGrpHeader m_PartiesHeader;` | `types/Structs.hpp` |

  > **Note** : pour les enums et bitmaps, le **layout binaire reste l'entier sous-jacent** (`uint8`/`uint16`) ; l'`enum class` et les masques fournissent la **sémantique** mais ne sont jamais le type effectif du champ. C'est l'arbitrage explicite d'ADR-0001 (cohérent avec `examples/PostTrade.hpp`). Documente le type sémantique en commentaire (`/// see types::LogoutCode`).
- **Champs réservés / padding** : `m_Reserved_<n>`.
- **CRC / checksum** éventuels.

**Si un type composé / enum / bitmap manque** :
- Vérifie d'abord dans les 3 fichiers consolidés `include/messages/types/{Enums,Bitmaps,Structs}.hpp`.
- Sinon, **stoppe** et demande à l'utilisateur de lancer `/pdf-to-cpp-types` pour cette section avec la catégorie adaptée (`enum` / `bitmap` / `struct`). Ne pas inventer de type ad-hoc dans le fichier message.
- ⚠️ **Ne pas déclencher** ce blocage pour un `char[N]` ou un entier : ces formes se déclarent directement, sans détour par `types/`.

### 3. Génération du code C++
Pour chaque message validé, produis :
- `include/messages/<Name>.hpp` à partir du [template](../../templates/Message.hpp.template), en respectant **strictement** :
  - `#pragma pack(push, 1)` autour de `<Name>Struct`,
  - champs typés en **primitifs bruts** (typedefs `Common.hpp` + `char[N]` direct) ou en `types::<Composé>` issus de `Structs.hpp` — **jamais** un nom d'enum / bitmap / alias,
  - préfixe `m_` sur les champs,
  - classe wrapper avec `MessageHeader const m_MessageHeader` + `reinterpret_cast` sur le payload,
  - constantes `TYPE` / `NAME` / `SIZE`,
  - getters (`std::string` pour `char[N]`, retour direct sinon),
  - `dump()` avec `DUMP_FIELD_AS_INT` pour `uint8` / `int8`, `DUMP_FIELD` sinon,
  - `operator<<` inline.
- **Includes minimaux depuis `types/`** : `#include "messages/types/Types.hpp"` (préféré, agrégateur) **OU** un sous-ensemble de `messages/types/Bitmaps.hpp` / `Enums.hpp` / `Structs.hpp` selon les noms réellement utilisés dans le fichier (typiquement uniquement `Structs.hpp` si seuls des composés sont référencés). ❌ Pas d'`#include "messages/types/<NomType>.hpp"` individuel. ❌ Pas d'`#include "messages/types/Aliases.hpp"` (le fichier n'existe pas).
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
- ❌ **Ne jamais** redéclarer un type (enum / bitmap / struct composé) déjà présent dans `types/`.
- ❌ **Ne jamais** typer un champ avec un nom d'enum ou de bitmap — c'est la violation centrale d'ADR-0001. Le champ porte le primitif sous-jacent (`uint8`/`uint16`) ; le commentaire `/// see types::<Name>` documente la sémantique.
- ❌ **Ne jamais** créer un typedef-alias (`using Isin = char[N]`, ...) ni inclure `messages/types/Aliases.hpp` (le fichier n'existe pas).
- ❌ **Ne jamais** créer ou inclure un fichier `types/<NomType>.hpp` individuel — utilisation des 3 fichiers consolidés (`Bitmaps.hpp` / `Enums.hpp` / `Structs.hpp`) uniquement.
- ✅ **Toujours** préserver le mapping binaire 1:1 avec la spec (ordre, taille, endianness).
- ✅ **Toujours** demander `MessageHeader` et `PacketHeader` à l'étape 0.
- ✅ **Toujours** mapper chaque champ vers un primitif brut (`Common.hpp` + `char[N]`), un stub domaine (`Decimal`, `TimestampNano` de `Common.hpp`), ou un composé de `Structs.hpp` ; pour les enums/bitmaps, mapper en primitif et stopper pour `/pdf-to-cpp-types` uniquement si l'enum/bitmap est lui-même absent et nécessaire à la logique métier.

## Variables

- Chemin PDF : ${input:pdfPath:Chemin du PDF (ou texte extrait)}
- Namespace : ${input:cppNamespace:protocol::messages}
- Standard C++ : ${input:cppStandard:c++17}
- Endianness : ${input:endianness:little}

## Démarrage

1. **Demander `MessageHeader` et `PacketHeader`** (étape 0) — bloquant.
2. Confirmer la lecture du PDF.
3. **Vérifier** que les 3 fichiers consolidés `types/{Bitmaps,Enums,Structs}.hpp` existent et couvrent les enums/bitmaps/composés nécessaires (sinon → `/pdf-to-cpp-types`). Les champs ASCII et scalaires n'exigent **aucun** détour par `types/`.
4. Produire l'inventaire des messages avec **mapping de chaque champ** (primitif brut / stub domaine / composé `Structs.hpp` / + commentaire sémantique pour enum/bitmap).
5. Attendre validation avant de générer le code.
