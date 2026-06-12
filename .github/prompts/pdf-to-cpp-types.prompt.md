---
agent: agent
description: Génère des types C++ réutilisables (structs, enums, bitfields) à partir d'un paragraphe ou d'une table d'un PDF de spec.
---

# Génération de types C++ depuis un PDF (par paragraphe / table)

Ton rôle : extraire un **type réutilisable** (struct composé, enum, bitfield, alias) depuis une **section précise** d'un PDF de spécification, et générer la **classe / struct C++ correspondante**, qui pourra ensuite être référencée dans les définitions de messages.

> Les types générés ici alimentent le workflow [pdf-to-cpp-messages](pdf-to-cpp-messages.prompt.md).
> Ils vivent dans `include/messages/types/`.

## Entrée requise (à demander à l'utilisateur si absente)

L'utilisateur **doit fournir** :
1. Le **PDF** ou son texte extrait (`specs/*.pdf` ou `specs/*.txt`).
2. Un **identifiant de section** dans le PDF, parmi :
   - un **numéro de paragraphe** (ex: `§4.3.2`, `Section 4.3.2`),
   - un **numéro / titre de table** (ex: `Table 7`, `Table 4-3 — Coordinate frame`),
   - une **plage de pages** (ex: `pages 18-20`).
3. Le **nom du type** souhaité (PascalCase, ex: `GeoPosition`, `CommandStatus`).
4. La **catégorie** du type :
   - `struct` — composition de champs,
   - `enum` — énumération scalaire,
   - `bitfield` — découpage sub-byte,
   - `alias` — `using` simple sur un type fondamental.

Si l'un de ces éléments manque, **demande-le** avant de continuer. Ne devine pas la section.

## Étapes

1. **Localisation**
   - Confirme la section ciblée (paragraphe / table) en citant 1-3 lignes du PDF.
   - Si la section décrit plusieurs types, **liste-les** et demande lequel générer (ou tous, un par un).

2. **Extraction**
   Selon la catégorie :
   - **struct** : liste des champs (nom, type, taille, unité, plage, valeur par défaut).
   - **enum** : valeurs (nom, valeur numérique, signification, type sous-jacent).
   - **bitfield** : largeur en bits de chaque champ + ordre (LSB-first ou MSB-first selon la spec).
   - **alias** : type sous-jacent + contraintes éventuelles (unit, scale).

3. **Génération du code C++**
   Respecte strictement [.github/instructions/cpp-messages.instructions.md](../instructions/cpp-messages.instructions.md).

   Crée le fichier `include/messages/types/<NomType>.hpp` à partir du template
   [templates/Type.hpp.template](../../templates/Type.hpp.template).

   Le header doit contenir :
   - Bloc d'en-tête avec **traçabilité** (PDF, section, version, date).
   - Le type lui-même (struct/enum/bitfield/alias).
   - Les `static_assert` de taille / layout adaptés à la catégorie.
   - Les constantes `UPPER_SNAKE` associées (taille attendue, masques pour bitfield).

4. **Enregistrement**
   - Ajoute une entrée dans `include/messages/types/Types.hpp` (header agrégateur) :
     ```cpp
     #include "messages/types/<NomType>.hpp"
     ```
   - Si le fichier n'existe pas, **crée-le**.

5. **Récap**
   Termine par :
   - Lien vers le fichier créé.
   - Liste des hypothèses (endianness, padding, ordre des bitfields).
   - Suggestion de réutilisation : *« Tu peux maintenant référencer `NomType` dans tes messages via `/pdf-to-cpp-messages` »*.
   - Points ambigus à clarifier (`// TODO:` dans le code).

## Règles strictes (style maison)

- **Un seul type par fichier** (sauf enums étroitement liés à un struct, qui peuvent cohabiter).
- **Préfixe `m_`** sur tous les champs de struct / bitfield (`m_LatitudeE7`, `m_Ready`).
- **Typedefs maison uniquement** : `uint8`, `int16`, `int32`, `uint32`, `int64`, `uint64`, `Decimal`, `TimestampNano`. ❌ pas de `std::uint*_t` dans les structs.
- **Chaînes ASCII fixes** : `char m_Foo[N]` (❌ pas de `std::array<char, N>`).
- **`#pragma pack(push, 1)` / `#pragma pack(pop)`** autour de toute struct binaire.
- **`enum class`** avec type sous-jacent obligatoire (`enum class X : uint8`).
- **Pas de prefix `Struct`** sur les types réutilisables (réservé aux messages — `<Message>Struct`).
- **Ne pas inventer** de champs absents de la spec. Marquer `// TODO: à clarifier — <raison>` si ambigu.
- **Ne pas générer de message** ici. Les messages sont du ressort de `/pdf-to-cpp-messages`.
- Pour les **enums** dont la spec impose une plage réservée (`0x00..0x7F = reserved`), ajoute un commentaire mais **ne crée pas** de valeurs synthétiques.
- Pour les **bitfields**, documenter le **compilateur cible** (l'ordre des bits est implementation-defined).

## Variables

- Chemin PDF : ${input:pdfPath:Chemin du PDF (ou texte extrait)}
- Section / Table : ${input:section:Ex. §4.3.2 ou Table 7}
- Nom du type : ${input:typeName:PascalCase, ex: GeoPosition}
- Catégorie : ${input:category|struct,enum,bitfield,alias}
- Namespace : ${input:cppNamespace:protocol::messages::types}

## Démarrage

1. Confirme l'accès au PDF.
2. Cite la section ciblée pour valider la localisation.
3. Demande la catégorie si ambiguë.
4. Génère le fichier et ajoute-le à `Types.hpp`.
