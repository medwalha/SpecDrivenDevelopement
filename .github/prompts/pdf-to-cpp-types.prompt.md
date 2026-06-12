---
agent: agent
description: Génère des types C++ réutilisables (enums, bitmaps, structs composés) à partir d'un PDF de spec, regroupés par catégorie dans des fichiers consolidés.
---

# Génération de types C++ depuis un PDF — fichiers consolidés par catégorie

Ton rôle : extraire des **types réutilisables** depuis un PDF de spécification et les **ajouter dans le fichier consolidé** correspondant à leur catégorie. Les types générés alimenteront ensuite le workflow [pdf-to-cpp-messages](pdf-to-cpp-messages.prompt.md).

> 📐 **Contrainte de design (ADR-0001)** : les structs de messages utilisent les **primitifs bruts** (`uint8`, `char[N]`, ...) au site de déclaration des champs. Les types extraits ici (enums, bitmaps, structs composés) servent à la **validation sémantique** par la logique métier, **jamais** comme type de champ d'une struct de message. Voir `CONTEXT.md` et `docs/adr/0001-message-struct-uses-raw-primitives.md`.

## ⚠️ Règle de regroupement

**Un seul fichier par catégorie**, pas un fichier par type. L'arborescence cible est **fixe** :

```
include/messages/types/
├── Enums.hpp       # tous les `enum class X : uint8 { ... }`
├── Bitmaps.hpp     # tous les `inline constexpr uint8 X_FLAG_MASK = 0x.. ;` (+ `_NONE_VALUE` optionnel)
├── Structs.hpp     # toutes les structs binaires composées (RptGrpHeader, FramingHeader, UnitHeader, ...)
└── Types.hpp       # agrégateur — `#include` exactement les 3 fichiers ci-dessus
```

- ❌ **Ne crée plus jamais** un fichier `types/<NomType>.hpp` séparé.
- ❌ **Pas de typedef-alias** sous `types/` (ni `using Isin = char[N]`, ni `using <Nom>Bitmap = uint8`). Les chaînes ASCII fixes et les entiers se déclarent directement au site de leur champ dans la struct de message. Voir ADR-0001.
- ❌ **`Aliases.hpp` n'existe pas** et ne doit pas être créé.
- ✅ **Ajoute** chaque nouveau type sous forme de **bloc commenté** (avec traçabilité PDF + section) à l'intérieur du fichier consolidé adapté.
- ✅ **Tri alphabétique** des blocs à l'intérieur de chaque fichier.
- ✅ Si l'un des 3 fichiers de catégorie n'existe pas encore, **crée-le** avec le squelette donné plus bas.

## Entrée requise (à demander à l'utilisateur si absente)

L'utilisateur **doit fournir** :
1. Le **PDF** ou son texte extrait (`specs/*.pdf` ou `specs/*.txt`).
2. Un **identifiant de section** dans le PDF :
   - numéro de paragraphe (ex: `§4.1.2`),
   - numéro / titre de table (ex: `Table 7`),
   - plage de pages (ex: `pages 18-20`),
   - ou bien « toute la spec » si tu dois faire un balayage complet.
3. Le **nom du type** souhaité en PascalCase (ex: `LogoutCode`, `TradeTypeBitmap`, `RptGrpHeader`). Peut être omis si l'utilisateur demande un balayage de section : tu proposes alors une liste à valider.
4. La **catégorie** parmi :
   - `enum` — `enum class X : <uint8|uint16|...>` énumération scalaire fermée,
   - `bitmap` — jeu de masques `UPPER_SNAKE` (ex-bitfield), portés par un `uint8`/`uint16` au site de leur champ,
   - `struct` — composition binaire packée de plusieurs champs.

> ℹ️ **Si l'utilisateur demande une "alias" / `using` / type renommé d'un primitif**, refuse poliment et oriente vers la déclaration directe (`char m_Foo[N];`, `int64 m_Bar;`) dans la struct de message — voir ADR-0001.

Si l'un de ces éléments manque, **demande-le**. Ne devine pas la section.

## Étapes

1. **Localisation**
   - Confirme la section ciblée en citant 1-3 lignes du PDF.
   - Si la section décrit plusieurs types, **liste-les** en tableau Markdown (nom proposé, catégorie, sous-jacent / taille) et demande validation.

2. **Extraction**
   - **enum** : valeurs (nom PascalCase, valeur numérique ou littéral `char`, signification), type sous-jacent.
   - **bitmap** : masques (`UPPER_SNAKE_MASK = 0x01`, etc.), valeur "neutre" éventuelle (`*_NONE_VALUE = 0x00`), type sous-jacent (`uint8`/`uint16`) — celui-ci sera porté par le champ correspondant dans la struct de message, pas par un `using` ici.
   - **struct** : liste des champs (nom `m_PascalCase`, type, taille, unité, plage).

3. **Insertion dans le fichier consolidé**
   Selon la catégorie, ajoute un **bloc** dans le fichier correspondant en respectant strictement [.github/instructions/cpp-messages.instructions.md](../instructions/cpp-messages.instructions.md) :

   - Catégorie `enum`   → [include/messages/types/Enums.hpp](../../include/messages/types/Enums.hpp)
   - Catégorie `bitmap` → [include/messages/types/Bitmaps.hpp](../../include/messages/types/Bitmaps.hpp)
   - Catégorie `struct` → [include/messages/types/Structs.hpp](../../include/messages/types/Structs.hpp)

   Chaque bloc doit contenir :
   - **Séparateur de bloc** : ligne `// === <NomType> — §<section> ===========================================`
   - **Doc Doxygen** rappelant le PDF + section + version (1-3 lignes max).
   - **Le type lui-même** (`enum class`, constantes `*_MASK` + `*_NONE_VALUE` optionnel, ou `struct` packée).
   - Les **`static_assert`** appropriés :
     - `enum` : `static_assert(sizeof(<NomEnum>) == N, "...")` inline (pas de constante `_SIZE` séparée).
     - `struct` : `sizeof`, `is_standard_layout`, `is_trivially_copyable`. Constante `<NAME>_SIZE` **optionnelle**.
     - `bitmap` : aucun (la largeur est portée au site du champ).

4. **Mise à jour de l'agrégateur**
   - Si tu crées l'un des 3 fichiers consolidés pour la première fois, mets à jour [include/messages/types/Types.hpp](../../include/messages/types/Types.hpp) pour qu'il `#include` exactement ces 3 fichiers (et **rien d'autre**).
   - Aucun `#include "messages/types/<NomType>.hpp"` individuel ne doit subsister dans le projet.
   - Aucun `#include "messages/types/Aliases.hpp"` ne doit apparaître.

5. **Récap**
   Termine par :
   - Liens vers le(s) fichier(s) consolidé(s) modifié(s) + nom des nouveaux blocs ajoutés.
   - Hypothèses prises (endianness, encodage ASCII, taille des champs ambigus).
   - Suggestion : *« Tu peux maintenant utiliser ces types pour la validation sémantique côté logique métier. Les champs de struct de message correspondants restent typés en primitif brut (`uint8`/`uint16`/`char[N]`/...) — voir ADR-0001. »*
   - Points ambigus à clarifier (`// TODO:` dans le code).

## Squelettes des 3 fichiers de catégorie (+ agrégateur)

Si l'un des fichiers est absent, **crée-le** avec ce squelette (blocs alphabétiques à ajouter en dessous).

### `Enums.hpp`
```cpp
// SPDX-License-Identifier: TODO
// All reusable scalar enumerations extracted from the PDF spec.
// Sections are kept alphabetical. Add new entries via /pdf-to-cpp-types.
//
// House rules: see CONTEXT.md > "Enum entry" and docs/adr/0001.
// No `*_SIZE` constant — use an inline `static_assert(sizeof(...) == N, ...)`.

#pragma once

#include <type_traits>

#include "messages/Common.hpp"

namespace protocol::messages::types {

// === <NomEnum> — §<section> =================================================
// enum class <NomEnum> : uint8 { OkValue = 0x00, ErrorValue = 0xFF };
// static_assert(sizeof(<NomEnum>) == 1, "<NomEnum> must be 1 byte per spec");

} // namespace protocol::messages::types
```

### `Bitmaps.hpp`
```cpp
// SPDX-License-Identifier: TODO
// All reusable bitmap mask constants extracted from the PDF spec.
// Sections are kept alphabetical. Add new entries via /pdf-to-cpp-types.
//
// House rules: see CONTEXT.md > "Bitmap entry" and docs/adr/0001.
// No `using <Nom>Bitmap = ...;` typedef. No `*_SIZE` / `*_BITMAP_SIZE` constant.

#pragma once

#include "messages/Common.hpp"

namespace protocol::messages::types {

// === <Nom>Bitmap — §<section> ===============================================
// // Carried in a `uint8 m_<Field>;` slot in the owning message struct.
// inline constexpr uint8 <NAME>_NONE_VALUE     = 0x00;  // optional sentinel
// inline constexpr uint8 <NAME>_<FLAG_A>_MASK  = 0x01;
// inline constexpr uint8 <NAME>_<FLAG_B>_MASK  = 0x02;

} // namespace protocol::messages::types
```

### `Structs.hpp`
```cpp
// SPDX-License-Identifier: TODO
// All reusable composed binary structs extracted from the PDF spec
// (e.g. RptGrpHeader, FramingHeader, UnitHeader, ...).
// Sections are kept alphabetical. Add new entries via /pdf-to-cpp-types.
//
// House rules: see CONTEXT.md > "Struct entry" and docs/adr/0001.
// Do NOT define `Decimal` or `TimestampNano` here — they live in `messages/Common.hpp`.

#pragma once

#include <ostream>
#include <type_traits>

#include "messages/Common.hpp"

namespace protocol::messages::types {

// === <NomType> — §<section> =================================================
// #pragma pack(push, 1)
// struct <NomType> { ... };
// #pragma pack(pop)
// static_assert(sizeof(<NomType>) == N, "...");
// static_assert(std::is_standard_layout<<NomType>>::value, "...");
// static_assert(std::is_trivially_copyable<<NomType>>::value, "...");
// inline constexpr unsigned int <NAME>_SIZE = N;   // optional

} // namespace protocol::messages::types
```

### `Types.hpp` (agrégateur — fixe)
```cpp
// SPDX-License-Identifier: TODO
// Aggregator header — pulls every reusable type extracted from the PDF spec.

#pragma once

#include "messages/types/Bitmaps.hpp"
#include "messages/types/Enums.hpp"
#include "messages/types/Structs.hpp"
```

## Règles strictes (style maison)

- ❌ **Ne crée plus** de fichier `types/<NomType>.hpp` séparé.
- ❌ **Pas de typedef-alias sous `types/`** : ni `using Isin = char[N]`, ni `using <Nom>Bitmap = uint8`, ni aucun autre renommage de primitif. Voir ADR-0001.
- ❌ **`Aliases.hpp` ne doit pas exister**. Le fichier est interdit ; toute mention dans un `#include` est un bug.
- ❌ **N'introduis pas** de nouveau `#include` dans `Types.hpp` autre que les 3 fichiers de catégorie (`Bitmaps.hpp`, `Enums.hpp`, `Structs.hpp`).
- ✅ **Préfixe `m_`** sur tous les champs de struct (`m_LatitudeE7`, `m_Ready`).
- ✅ **Typedefs maison uniquement** : `uint8`, `int16`, `int32`, `uint32`, `int64`, `uint64`. ❌ pas de `std::uint*_t` dans les structs.
- ✅ **Chaînes ASCII fixes** : déclarées **directement** comme `char m_Foo[N];` au site du champ (jamais via un `using` dans `types/`).
- ✅ **`#pragma pack(push, 1)` / `#pragma pack(pop)`** autour de **chaque** struct binaire (y compris dans `Structs.hpp`).
- ✅ **`enum class`** avec type sous-jacent obligatoire (`enum class X : uint8`).
- ✅ **Pas de suffixe `Struct`** sur les types réutilisables (réservé aux messages — `<Message>Struct`).
- ✅ **Ordre alphabétique** des blocs dans chaque fichier consolidé.
- ✅ Pour les **enums** dont la spec impose une plage réservée, ajoute un commentaire mais **ne crée pas** de valeurs synthétiques.
- ✅ **Ne pas inventer** de champs absents de la spec. Marquer `// TODO: à clarifier — <raison>` si ambigu.
- ❌ **Ne pas générer de message** ici. Les messages sont du ressort de `/pdf-to-cpp-messages`.

## Variables

- Chemin PDF : ${input:pdfPath:Chemin du PDF (ou texte extrait)}
- Section / Table : ${input:section:Ex. §4.3.2 ou Table 7 ou "all"}
- Nom du type : ${input:typeName:PascalCase, ex: LogoutCode, TradeTypeBitmap, RptGrpHeader (laisser vide pour balayage)}
- Catégorie : ${input:category|enum,bitmap,struct}
- Namespace : ${input:cppNamespace:protocol::messages::types}

## Démarrage

1. Confirme l'accès au PDF (extraction `pdftotext -layout` si besoin).
2. Cite la section ciblée pour valider la localisation.
3. Liste les types détectés (tableau Markdown : nom, catégorie, fichier cible, taille).
4. **Demande validation** si ≥ 5 types vont être ajoutés en un coup.
5. Insère les blocs dans `Enums.hpp` / `Bitmaps.hpp` / `Structs.hpp` (crée-les si absents).
6. Vérifie / mets à jour `Types.hpp` pour qu'il référence exactement ces 3 fichiers.
7. Compile-check rapide : `g++ -std=c++17 -Iinclude -fsyntax-only -x c++ include/messages/types/Types.hpp`.

