---
agent: agent
description: Génère des types C++ réutilisables (aliases, enums, bitmaps, structs) à partir d'un PDF de spec, regroupés par catégorie dans des fichiers consolidés.
---

# Génération de types C++ depuis un PDF — fichiers consolidés par catégorie

Ton rôle : extraire des **types réutilisables** depuis un PDF de spécification et les **ajouter dans le fichier consolidé** correspondant à leur catégorie. Les types générés alimenteront ensuite le workflow [pdf-to-cpp-messages](pdf-to-cpp-messages.prompt.md).

## ⚠️ Règle de regroupement (NOUVELLE)

**Un seul fichier par catégorie**, pas un fichier par type. L'arborescence cible est **fixe** :

```
include/messages/types/
├── Aliases.hpp     # tous les `using Foo = char[N]`, `using Bar = int64`, ...
├── Enums.hpp       # tous les `enum class X : uint8 { ... }`
├── Bitmaps.hpp     # tous les `using FlagsBitmap = uint8` + masques UPPER_SNAKE
├── Structs.hpp     # toutes les structs binaires composées (Decimal, TimestampNano, RptGrpHeader, GeoPosition, ...)
└── Types.hpp       # agrégateur — `#include` les 4 fichiers ci-dessus
```

- ❌ **Ne crée plus jamais** un fichier `types/<NomType>.hpp` séparé.
- ✅ **Ajoute** chaque nouveau type sous forme de **bloc commenté** (avec traçabilité PDF + section) à l'intérieur du fichier consolidé adapté.
- ✅ **Tri alphabétique** des blocs à l'intérieur de chaque fichier.
- ✅ Si l'un des 4 fichiers n'existe pas encore, **crée-le** avec le squelette donné plus bas.

## Entrée requise (à demander à l'utilisateur si absente)

L'utilisateur **doit fournir** :
1. Le **PDF** ou son texte extrait (`specs/*.pdf` ou `specs/*.txt`).
2. Un **identifiant de section** dans le PDF :
   - numéro de paragraphe (ex: `§4.1.2`),
   - numéro / titre de table (ex: `Table 7`),
   - plage de pages (ex: `pages 18-20`),
   - ou bien « toute la spec » si tu dois faire un balayage complet.
3. Le **nom du type** souhaité en PascalCase (ex: `GeoPosition`, `LogoutCode`). Peut être omis si l'utilisateur demande un balayage de section : tu proposes alors une liste à valider.
4. La **catégorie** parmi :
   - `alias` — `using Foo = <type fondamental ou tableau>` (chaînes ASCII fixes incluses),
   - `enum` — `enum class X : <uint8|uint16|...>` énumération scalaire fermée,
   - `bitmap` — `using XBitmap = uint8` + jeu de masques `UPPER_SNAKE` (ex-bitfield),
   - `struct` — composition binaire packée de plusieurs champs.

Si l'un de ces éléments manque, **demande-le**. Ne devine pas la section.

## Étapes

1. **Localisation**
   - Confirme la section ciblée en citant 1-3 lignes du PDF.
   - Si la section décrit plusieurs types, **liste-les** en tableau Markdown (nom proposé, catégorie, sous-jacent / taille) et demande validation.

2. **Extraction**
   - **alias** : type sous-jacent + taille (`char[N]`, `int64`, etc.) + contraintes (unité, scale, encodage ASCII).
   - **enum** : valeurs (nom PascalCase, valeur numérique ou littéral `char`, signification), type sous-jacent.
   - **bitmap** : masques (`UPPER_SNAKE_MASK = 0x01`, etc.), valeur "neutre" éventuelle (`*_NONE_VALUE = 0x00`).
   - **struct** : liste des champs (nom `m_PascalCase`, type, taille, unité, plage).

3. **Insertion dans le fichier consolidé**
   Selon la catégorie, ajoute un **bloc** dans le fichier correspondant en respectant strictement [.github/instructions/cpp-messages.instructions.md](../instructions/cpp-messages.instructions.md) :

   - Catégorie `alias` → [include/messages/types/Aliases.hpp](../../include/messages/types/Aliases.hpp)
   - Catégorie `enum`  → [include/messages/types/Enums.hpp](../../include/messages/types/Enums.hpp)
   - Catégorie `bitmap` → [include/messages/types/Bitmaps.hpp](../../include/messages/types/Bitmaps.hpp)
   - Catégorie `struct` → [include/messages/types/Structs.hpp](../../include/messages/types/Structs.hpp)

   Chaque bloc doit contenir :
   - **Séparateur de bloc** : ligne `// === <NomType> — §<section> ===========================================`
   - **Doc Doxygen** rappelant le PDF + section + version (1-3 lignes max).
   - **Le type lui-même** (struct/enum/alias/bitmap).
   - Les **constantes `UPPER_SNAKE`** associées (`<NAME>_SIZE`, masques pour bitmap).
   - Les **`static_assert`** appropriés (`sizeof`, `is_standard_layout`, `is_trivially_copyable` pour les structs).

4. **Mise à jour de l'agrégateur**
   - Si tu crées l'un des 4 fichiers consolidés pour la première fois, mets à jour [include/messages/types/Types.hpp](../../include/messages/types/Types.hpp) pour qu'il `#include` exactement ces 4 fichiers (et **rien d'autre**).
   - Aucun `#include "messages/types/<NomType>.hpp"` individuel ne doit subsister dans le projet.

5. **Récap**
   Termine par :
   - Liens vers le(s) fichier(s) consolidé(s) modifié(s) + nom des nouveaux blocs ajoutés.
   - Hypothèses prises (endianness, encodage ASCII, taille des champs ambigus).
   - Suggestion : *« Tu peux maintenant référencer ces types dans tes messages via `/pdf-to-cpp-messages` ; ils sont accessibles via `#include "messages/types/Types.hpp"` ou les 4 fichiers individuels. »*
   - Points ambigus à clarifier (`// TODO:` dans le code).

## Squelettes des 4 fichiers consolidés

Si l'un des fichiers est absent, **crée-le** avec ce squelette (alphabetical sections to be appended below).

### `Aliases.hpp`
```cpp
// SPDX-License-Identifier: TODO
// All reusable typedefs / `using` aliases extracted from the PDF spec.
// Sections are kept alphabetical. Add new entries via /pdf-to-cpp-types.
//
// Source : <pdf>   Spec version: <vX.Y>   Generated: <YYYY-MM-DD>

#pragma once

#include "messages/Common.hpp"

namespace protocol::messages::types {

// === <NomType> — §<section> =================================================
// (bloc docté + `using` + constante <NAME>_SIZE + static_assert si applicable)

} // namespace protocol::messages::types
```

### `Enums.hpp`
```cpp
// SPDX-License-Identifier: TODO
// All reusable scalar enumerations extracted from the PDF spec.
// Sections are kept alphabetical. Add new entries via /pdf-to-cpp-types.

#pragma once

#include "messages/Common.hpp"

namespace protocol::messages::types {

// === <NomEnum> — §<section> =================================================
// enum class <NomEnum> : <uint8|uint16|char> { ... };
// inline constexpr unsigned int <NAME>_SIZE = sizeof(<NomEnum>);
// static_assert(...);

} // namespace protocol::messages::types
```

### `Bitmaps.hpp`
```cpp
// SPDX-License-Identifier: TODO
// All reusable bitmap aliases + UPPER_SNAKE masks extracted from the PDF spec.
// Sections are kept alphabetical. Add new entries via /pdf-to-cpp-types.

#pragma once

#include "messages/Common.hpp"

namespace protocol::messages::types {

// === <Nom>Bitmap — §<section> ===============================================
// using <Nom>Bitmap = uint8;
// inline constexpr uint8 <NAME>_<FLAG>_MASK = 0x01;
// inline constexpr unsigned int <NAME>_BITMAP_SIZE = sizeof(<Nom>Bitmap);
// static_assert(...);

} // namespace protocol::messages::types
```

### `Structs.hpp`
```cpp
// SPDX-License-Identifier: TODO
// All reusable composed binary structs extracted from the PDF spec
// (Decimal, TimestampNano, RptGrpHeader, FramingHeader, UnitHeader, ...).
// Sections are kept alphabetical. Add new entries via /pdf-to-cpp-types.

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
// inline constexpr unsigned int <NAME>_SIZE = N;

} // namespace protocol::messages::types
```

### `Types.hpp` (agrégateur — fixe)
```cpp
// SPDX-License-Identifier: TODO
// Aggregator header — pulls every reusable type extracted from the PDF spec.

#pragma once

#include "messages/types/Aliases.hpp"
#include "messages/types/Bitmaps.hpp"
#include "messages/types/Enums.hpp"
#include "messages/types/Structs.hpp"
```

## Règles strictes (style maison)

- ❌ **Ne crée plus** de fichier `types/<NomType>.hpp` séparé.
- ❌ **N'introduis pas** de nouveau `#include` dans `Types.hpp` autre que les 4 fichiers de catégorie.
- ✅ **Préfixe `m_`** sur tous les champs de struct (`m_LatitudeE7`, `m_Ready`).
- ✅ **Typedefs maison uniquement** : `uint8`, `int16`, `int32`, `uint32`, `int64`, `uint64`. ❌ pas de `std::uint*_t` dans les structs.
- ✅ **Chaînes ASCII fixes** : `using Foo = char[N];` (❌ pas de `std::array<char, N>`).
- ✅ **`#pragma pack(push, 1)` / `#pragma pack(pop)`** autour de **chaque** struct binaire (même dans `Structs.hpp`).
- ✅ **`enum class`** avec type sous-jacent obligatoire (`enum class X : uint8`).
- ✅ **Pas de suffixe `Struct`** sur les types réutilisables (réservé aux messages — `<Message>Struct`).
- ✅ **Ordre alphabétique** des blocs dans chaque fichier consolidé.
- ✅ Pour les **enums** dont la spec impose une plage réservée, ajoute un commentaire mais **ne crée pas** de valeurs synthétiques.
- ✅ **Ne pas inventer** de champs absents de la spec. Marquer `// TODO: à clarifier — <raison>` si ambigu.
- ❌ **Ne pas générer de message** ici. Les messages sont du ressort de `/pdf-to-cpp-messages`.

## Variables

- Chemin PDF : ${input:pdfPath:Chemin du PDF (ou texte extrait)}
- Section / Table : ${input:section:Ex. §4.3.2 ou Table 7 ou "all"}
- Nom du type : ${input:typeName:PascalCase, ex: GeoPosition (laisser vide pour balayage)}
- Catégorie : ${input:category|alias,enum,bitmap,struct}
- Namespace : ${input:cppNamespace:protocol::messages::types}

## Démarrage

1. Confirme l'accès au PDF (extraction `pdftotext -layout` si besoin).
2. Cite la section ciblée pour valider la localisation.
3. Liste les types détectés (tableau Markdown : nom, catégorie, fichier cible, taille).
4. **Demande validation** si ≥ 5 types vont être ajoutés en un coup.
5. Insère les blocs dans `Aliases.hpp` / `Enums.hpp` / `Bitmaps.hpp` / `Structs.hpp` (crée-les si absents).
6. Vérifie / mets à jour `Types.hpp` pour qu'il référence exactement les 4 fichiers.
7. Compile-check rapide : `g++ -std=c++17 -Iinclude -fsyntax-only -x c++ include/messages/types/Types.hpp`.
