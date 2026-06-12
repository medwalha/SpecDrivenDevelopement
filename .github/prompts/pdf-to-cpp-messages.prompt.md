---
agent: agent
description: Génère des structures de messages C++ (style maison) à partir d'un PDF de spécification (ICD / protocole binaire).
---

# Génération de structures de messages C++ depuis un PDF

Ton rôle : analyser un **PDF de spécification** (ICD, protocole binaire, format de trame) et générer des **messages C++ au style maison** :

- struct binaire `<Name>Struct` packée (`#pragma pack(push,1)`) avec champs `m_PascalCase`,
- typedefs courts (`uint8`, `int64`, `Decimal`, `TimestampNano`, ...) — **pas de `std::uint*_t`**,
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

## Étape 0 — Pré-requis OBLIGATOIRE : `MessageHeader` & `PacketHeader`

**Avant toute génération de message**, demande systématiquement à l'utilisateur :

1. **`MessageHeader`** : quel est son layout ?
   - Champs (nom, type, taille) ?
   - Taille totale (`MessageHeader::SIZE`) ?
   - Quels champs doivent apparaître dans `MessageHeader::dump()` ?
   - Existe-t-il déjà un fichier `include/messages/MessageHeader.hpp` à réutiliser ?

2. **`PacketHeader`** : le protocole encapsule-t-il les messages dans un paquet de transport ?
   - Si **oui** : layout, taille, et est-ce que plusieurs `MessageHeader` peuvent suivre un seul `PacketHeader` ?
   - Si **non** : confirme explicitement « pas de PacketHeader ».

3. **Typedefs domaine** : confirme que `Decimal`, `TimestampNano`, `InputData` correspondent bien aux types réels du projet (sinon adapter `include/messages/Common.hpp`).

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

### 2. Analyse de chaque message
Pour chaque message, identifie :
- **Nom** PascalCase (sans suffixe `Message` sauf si la spec l'utilise — ex: `PostTrade`).
- **ID / opcode** (constante `MSG_ID_<UPPER_SNAKE>`).
- **Champs** dans l'ordre : nom (PascalCase), type (typedef maison), taille, unité, contraintes.
- **Champs réservés / padding** : `m_Reserved_<n>`.
- **Champs bitfield** si bits non alignés sur octet.
- **Enums** associés.
- **CRC / checksum** éventuels.
- **Types réutilisables** (struct composé, enum partagé, bitfield) :
  - vérifie d'abord dans `include/messages/types/`,
  - sinon, **invite l'utilisateur** à lancer [/pdf-to-cpp-types](pdf-to-cpp-types.prompt.md) pour la section concernée et **attends** que le type soit créé,
  - puis référence le type via `#include "messages/types/<Nom>.hpp"`.

### 3. Génération du code C++
Pour chaque message validé, produis :
- `include/messages/<Name>.hpp` à partir du template, en respectant **strictement** :
  - `#pragma pack(push, 1)` autour de `<Name>Struct`,
  - typedefs maison (`uint8`, `int64`, `Decimal`, `TimestampNano`, `char[N]`),
  - préfixe `m_` sur les champs,
  - classe wrapper avec `MessageHeader const m_MessageHeader` + `reinterpret_cast` sur le payload,
  - constantes `TYPE` / `NAME` / `SIZE`,
  - getters (`std::string` pour `char[N]`, sinon retour direct),
  - `dump()` avec `DUMP_FIELD_AS_INT` pour `uint8` / `int8`, `DUMP_FIELD` sinon,
  - `operator<<` inline.
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
- hypothèses prises (endianness, padding, types ambigus),
- points qui nécessitent une revue humaine (`// TODO:` dans le code),
- une commande de compilation suggérée (ex: `g++ -std=c++17 -Wall -Iinclude examples/PostTrade.hpp -c -o /dev/null`).

## Règles strictes

- ❌ **Ne jamais** utiliser `std::uint8_t`, `unsigned int`, `int`, `long` dans une struct de message → typedefs maison uniquement.
- ❌ **Ne jamais** utiliser `snake_case` ou supprimer le préfixe `m_` sur les champs.
- ❌ **Ne jamais** copier le payload — toujours `reinterpret_cast` + référence const.
- ❌ **Ne jamais** inventer un champ absent du PDF (marquer `// TODO: à clarifier`).
- ❌ **Ne jamais** générer plus de 5 messages d'un coup sans confirmation utilisateur.
- ✅ **Toujours** préserver le mapping binaire 1:1 avec la spec (ordre, taille, endianness).
- ✅ **Toujours** demander `MessageHeader` et `PacketHeader` à l'étape 0.

## Variables

- Chemin PDF : ${input:pdfPath:Chemin du PDF (ou texte extrait)}
- Namespace : ${input:cppNamespace:protocol::messages}
- Standard C++ : ${input:cppStandard:c++17}
- Endianness : ${input:endianness:little}

## Démarrage

1. **Demander `MessageHeader` et `PacketHeader`** (étape 0) — bloquant.
2. Confirmer la lecture du PDF.
3. Produire l'inventaire des messages.
4. Attendre validation avant de générer le code.
