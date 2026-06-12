Status: ready-for-agent
Category: enhancement

# Drop the `alias` indirection — message fields use primitive types directly

This issue was produced by `/triage` and then sharpened by a `/grill-with-docs` session on 2026-06-12. See the Comments section at the bottom for the grilling-round summary.

## Reporter intent

> "I don't want to add aliases under types — I need to use them directly in messages."

The current `/pdf-to-cpp-types` + `/pdf-to-cpp-messages` workflow prescribes a `using Isin = char[12]` style typedef-alias in `include/messages/types/Aliases.hpp` and forbids the corresponding message struct field from declaring `char m_SecurityID[12]` directly when such an alias exists. The reporter wants message struct fields to use the underlying primitive (`char m_SecurityID[12]`, `int64 m_MsgSeqNum`, `uint8 m_TradeType`) directly, with no typedef-alias layer in between.

The canonical example `examples/PostTrade.hpp` already declares fields this way — so this change **realigns the prompts and the `types/` scaffolding with the example**, rather than introducing a new style.

## Glossary & ADR references

This issue formalises terminology and a load-bearing trade-off already captured in:

- `CONTEXT.md` at the repo root — see entries: *Message struct*, *House typedefs*, *Domain stub*, *Typedef-alias* (forbidden), *Alias category* (deprecated), *Reusable type*, *Semantic-only usage*, *Bitmap entry*, *Enum entry*, *Struct entry*.
- `docs/adr/0001-message-struct-uses-raw-primitives.md` — the trade-off underlying the whole change.

Whenever this brief uses one of those terms, it means exactly what the glossary says.

## Agent Brief

**Category:** enhancement

**Summary:** Eliminate the `alias` type category. Message struct fields declare scalar/ASCII-array fields directly using the house typedefs from `Common.hpp` and raw `char[N]`. The `Aliases.hpp` prescription is removed; `Bitmaps.hpp` and `Enums.hpp` carry only named constants and `enum class` declarations (no `using` typedef-aliases); `/pdf-to-cpp-types` accepts only three categories (`enum`, `bitmap`, `struct`); the aggregator `Types.hpp` is rewritten to reflect the new scaffolding.

### Current behavior

- `/pdf-to-cpp-types` accepts four categories: `alias`, `enum`, `bitmap`, `struct`. Each was routed to a dedicated consolidated header under `include/messages/types/` (`Aliases.hpp` / `Enums.hpp` / `Bitmaps.hpp` / `Structs.hpp`).
- `/pdf-to-cpp-messages` enforces a hard rule (the "Règle d'or") that forbids declaring a field as `char m_Foo[N]` "if a dedicated alias exists" — e.g. it requires `Isin m_SecurityID;` instead of `char m_SecurityID[12];`. When the prescribed typedef-alias is missing, the prompt **stops generation** and requires the user to run `/pdf-to-cpp-types` first.
- The reusable-types mapping table in `/pdf-to-cpp-messages` step 2 lists `Aliases.hpp` as one of four sources to map fields against and incorrectly claims `Decimal` / `TimestampNano` live in `types/Structs.hpp` (they actually live in `Common.hpp` as domain stubs).
- The `Bitmaps.hpp` skeleton in `/pdf-to-cpp-types` prescribes a `using <Nom>Bitmap = uint8;` typedef-alias alongside the `*_MASK` constants and a `*_BITMAP_SIZE = sizeof(<Nom>Bitmap)` constant.
- The `Enums.hpp` skeleton prescribes a `*_SIZE = sizeof(<NomEnum>)` constant alongside the enum class.
- **On disk today:** `include/messages/types/` contains only `Types.hpp`. `Aliases.hpp`, `Enums.hpp`, `Bitmaps.hpp`, `Structs.hpp` have never been created. `Types.hpp` includes a stale `messages/types/GeoPosition.hpp` that does not exist (left over from an earlier scaffold).
- `examples/PostTrade.hpp` already uses raw primitives in its message struct fields (`char m_SecurityID[12]`, `int64 m_MsgSeqNum`, `uint8 m_TradeType`, …) — contradicting the prompts.

### Desired behavior

- `/pdf-to-cpp-types` accepts exactly three categories: `enum`, `bitmap`, `struct`. The `alias` category is removed from the frontmatter description, the input-variable enum, the category-to-file routing, the skeletons section, the "règles strictes", and the démarrage checklist.
- `/pdf-to-cpp-messages` instructs message struct fields to use the underlying binary primitive directly. The "Règle d'or" and reusable-types table both reflect:
  - Scalar fields → house typedefs from `Common.hpp` (`uint8`, `int16`, `int64`, …).
  - Fixed-width ASCII strings → raw `char m_Field[N];`.
  - Composed types from `Structs.hpp` (e.g. `RptGrpHeader`, `FramingHeader`) → referenced by name (this is the **only** `types/` category whose name is allowed in a message struct field).
  - Domain stubs (`Decimal`, `TimestampNano`) → referenced by name from `Common.hpp` (not `Structs.hpp` — fix this factual error wherever it appears).
  - Enums and bitmaps → field typed as the underlying primitive (`uint8` / `uint16`); the `enum class` and mask constants are available for business-logic validation only.
- The blocking flow ("stop the message and run `/pdf-to-cpp-types`") triggers only when an `enum`, `bitmap`, or `struct` is missing — never for an ASCII shape or scalar identifier.
- The skeletons for `Bitmaps.hpp` and `Enums.hpp` no longer prescribe any `using` typedef-alias. The `*_BITMAP_SIZE` and enum `*_SIZE` constants are dropped (their information is obvious at the use site and they referenced types being removed anyway). `*_NONE_VALUE` sentinels in `Bitmaps.hpp` are preserved as an optional artefact when the spec names one.
- `include/messages/types/Enums.hpp`, `Bitmaps.hpp`, `Structs.hpp` are created from scratch with the new skeletons (none exist today).
- `include/messages/types/Types.hpp` is rewritten to include exactly those three category headers in alphabetical order — and **nothing else** (the stale `GeoPosition.hpp` include disappears).
- `include/messages/types/Aliases.hpp` is **not** created.
- `examples/PostTrade.hpp` is **not** edited. It must still compile after the change — it is the validation oracle.
- `include/messages/Common.hpp` is **not** edited. The `Decimal` / `TimestampNano` / `InputData` stubs stay exactly where they are.

### Key interfaces

- **`/pdf-to-cpp-types` prompt** (`.github/prompts/pdf-to-cpp-types.prompt.md`):
  - Frontmatter `description` — drop the "aliases" mention.
  - "Règle de regroupement" section — file inventory lists exactly three category files plus the aggregator, and explicitly states that typedef-aliases are out of scope.
  - "Entrée requise" item #4 — `category` accepts only `enum` / `bitmap` / `struct`.
  - "Étapes" → step 2 ("Extraction") — drop the `alias` sub-bullet.
  - "Étapes" → step 3 ("Insertion") — drop the alias routing row from the category-to-file table.
  - "Squelettes des fichiers consolidés" — three category skeletons plus the aggregator skeleton (was four). The `Bitmaps.hpp` skeleton drops the `using <Nom>Bitmap = uint8;` line, drops `*_BITMAP_SIZE`, keeps the `*_MASK` constants and the optional `*_NONE_VALUE`. The `Enums.hpp` skeleton drops the `*_SIZE` constant; the inline `static_assert(sizeof(<NomEnum>) == N, …)` remains.
  - The `Types.hpp` skeleton — references the three remaining category headers only, in alphabetical order.
  - "Règles strictes" — keep the "no per-type file" rule; add an explicit "no typedef-aliases under `types/`" rule. ASCII strings as `char[N]` directly is now the expected form at the field-declaration site.
  - "Variables" → `category` enum drops `alias`.
  - "Démarrage" checklist — update step 5's file-list to the three remaining files.

- **`/pdf-to-cpp-messages` prompt** (`.github/prompts/pdf-to-cpp-messages.prompt.md`):
  - Top paragraph and "Règle d'or" — remove every reference to `Aliases.hpp`, `Isin`, `Mic`, `Currency` as named field types. State that ASCII fields use raw `char m_Field[N];` and integer identifiers use the house typedefs.
  - "Interdits dans `<Name>Struct`" list — remove the `char m_Foo[N]` interdiction; keep the bans on ad-hoc `using` / `enum class` / `struct` inside the message file.
  - "Procédure si un type manque" — applies to `enum` / `bitmap` / `struct` only.
  - The reusable-types mapping table in step 2 — drop the `Aliases.hpp` row; rewrite the `SecurityID` example row to show `char m_SecurityID[12]` is the expected form; correct the `Decimal` row to point at `Common.hpp` (not `Structs.hpp`).
  - The paragraph naming `Decimal` and `TimestampNano` as types "re-exported in `protocol::messages`" — clarify they're domain stubs *defined* in `Common.hpp`, not re-exported from `types/`.
  - "Includes minimaux" — drop references to `Aliases.hpp`. Allowed includes from `types/`: the aggregator `Types.hpp` or any subset of `Enums.hpp` / `Bitmaps.hpp` / `Structs.hpp`.
  - "Règles strictes" footer — the "never redeclare a type already present in `types/`" rule still applies, but only for the three remaining categories.

- **`.github/instructions/cpp-messages.instructions.md`**:
  - Scan for any reference to `Aliases.hpp` or to a typedef-alias used as a field type. Currently the file does not mention `Aliases.hpp` directly, but verify and remove anything that conflicts with the new rule.
  - The `char[N]` row in the typedefs table is already correct and stays.
  - Optional: add a one-line reference to ADR-0001 so the rule is discoverable from the conventions file.

- **`include/messages/types/Enums.hpp`** *(create from scratch)*:
  - SPDX header, generated-from comment, `#pragma once`.
  - `#include "messages/Common.hpp"`.
  - Namespace `protocol::messages::types`.
  - Placeholder comment block matching the updated skeleton: `// === <NomEnum> — §<section> ===` with an example showing the `enum class` and the inline `static_assert(sizeof(...) == N, ...)` — **no** `*_SIZE` constant.

- **`include/messages/types/Bitmaps.hpp`** *(create from scratch)*:
  - Same preamble as above.
  - Placeholder comment block matching the updated skeleton: `// === <Nom>Bitmap — §<section> ===` with example `*_MASK` constants and an optional `*_NONE_VALUE` — **no** `using <Nom>Bitmap = …;` line, **no** `*_BITMAP_SIZE` constant.

- **`include/messages/types/Structs.hpp`** *(create from scratch)*:
  - Same preamble plus `#include <ostream>` and `#include <type_traits>`.
  - Placeholder comment block matching the existing skeleton (this category is unchanged in shape — composed structs with `#pragma pack`, `static_assert`s on `sizeof` / layout / trivial-copy, optional `*_SIZE` constant). The example list in the comment must **not** name `Decimal` or `TimestampNano` (those are in `Common.hpp`); use `RptGrpHeader`, `FramingHeader`, `UnitHeader` instead.

- **`include/messages/types/Types.hpp`** *(rewrite in place)*:
  - SPDX header, brief comment.
  - `#pragma once`.
  - Exactly three `#include` directives, alphabetical: `"messages/types/Bitmaps.hpp"`, `"messages/types/Enums.hpp"`, `"messages/types/Structs.hpp"`.
  - No reference to `Aliases.hpp`, `GeoPosition.hpp`, or any per-type header.

- **`include/messages/Common.hpp`**: untouched.
- **`include/messages/MessageHeader.hpp`** / **`PacketHeader.hpp`** / **`MessageIds.hpp`**: untouched.
- **`examples/PostTrade.hpp`**: untouched — used as the compile oracle.
- **`README.md`**: scan the "Style maison généré" and "Comment l'utiliser" sections for any alias reference. None expected today, but verify.

### Acceptance criteria

- [ ] `include/messages/types/Enums.hpp`, `Bitmaps.hpp`, `Structs.hpp` exist on disk with the new skeletons described above.
- [ ] `include/messages/types/Types.hpp` contains exactly three `#include` lines (`Bitmaps.hpp`, `Enums.hpp`, `Structs.hpp`, alphabetical) and no other `#include`.
- [ ] `include/messages/types/Aliases.hpp` does not exist on disk and is not referenced anywhere in `.github/`, `include/`, `templates/`, `examples/`, or `README.md`.
- [ ] `g++ -std=c++17 -Wall -Iinclude -fsyntax-only -x c++ include/messages/types/Types.hpp` succeeds.
- [ ] `g++ -std=c++17 -Wall -Iinclude -fsyntax-only -x c++ examples/PostTrade.hpp` still succeeds (the example is unchanged).
- [ ] `.github/prompts/pdf-to-cpp-types.prompt.md` lists exactly three categories (`enum`, `bitmap`, `struct`) consistently in: frontmatter `description`, the `${input:category|…}` variable, the category-to-file routing table, the skeletons section count, the "règles strictes" wording, and the "démarrage" checklist file-list.
- [ ] The `Bitmaps.hpp` skeleton in `/pdf-to-cpp-types` does not contain a `using` typedef-alias for the bitmap. The skeleton retains `*_MASK` examples and the optional `*_NONE_VALUE`.
- [ ] The `Enums.hpp` skeleton in `/pdf-to-cpp-types` does not contain a `*_SIZE` constant. An inline `static_assert(sizeof(...) == N, ...)` is acceptable in its place.
- [ ] `.github/prompts/pdf-to-cpp-messages.prompt.md` no longer forbids `char m_Foo[N]` in a message struct, and the blocking workflow ("stop and run `/pdf-to-cpp-types`") triggers only for missing `enum` / `bitmap` / `struct` types — never for an ASCII shape or scalar.
- [ ] Both prompts and the reusable-types mapping table correctly state that `Decimal` and `TimestampNano` live in `include/messages/Common.hpp`, not in `Structs.hpp`.
- [ ] A dry-run walk-through of `/pdf-to-cpp-messages` against a hypothetical `SecurityID: Alphanumerical 12` field produces `char m_SecurityID[12];` without consulting any alias file, without pausing, and without modifying `include/messages/types/`.
- [ ] `grep -RIn -E "Aliases\.hpp|alias category|category.*alias" .github/ include/ templates/ examples/ README.md CONTEXT.md` returns matches only where the term is used to *forbid* aliases or in this issue file / ADR-0001 (the historical reference).

### Out of scope

- **`Common.hpp` content** — `Decimal`, `TimestampNano`, `InputData` stay exactly as they are. They are domain stubs the consumer project replaces with its real types; they have nothing to do with the spec-extraction workflow.
- **`examples/PostTrade.hpp`** — it is the validation oracle. If it stops compiling after the changes, the changes are wrong, not the example.
- **`MessageHeader` / `PacketHeader` workflow** — the étape 0 blocking prerequisite gate in `/pdf-to-cpp-messages` is untouched.
- **Renaming or restructuring** the prompt or instruction files.
- **Backfilling real `enum class` / mask / struct content** from a PDF spec — this issue is about scaffolding and prompt definitions only.
- **Stricter typing of message struct fields** (e.g. typing the field as `TradeType` rather than `uint8`) — this is the exact trade-off rejected by ADR-0001; do not reintroduce it under any guise.
- **`Types.hpp` consumers** — no message file should be edited just to "modernise" its includes; only the four prompt / scaffold targets above are in scope.

## Comments

### 2026-06-12 — /grill-with-docs session

> *This was generated by AI during triage.*

Five sharpening rounds applied. Summary of what changed vs the original brief:

1. **Symmetric kill** *(was: asymmetric — only `Aliases.hpp` killed)*. The `using <Nom>Bitmap = uint8;` typedef-alias in `Bitmaps.hpp` is the same shape as the alias-category typedefs; it is dropped too, alongside `*_BITMAP_SIZE`. `Bitmaps.hpp` keeps only mask constants and the optional `*_NONE_VALUE`.
2. **Fact-fix**: `Decimal` and `TimestampNano` live in `Common.hpp` (as domain stubs), not in `Structs.hpp`. Both prompts and the reusable-types table get corrected.
3. **Disk reality**: only `Types.hpp` exists under `include/messages/types/` today. The work is therefore: create `Enums.hpp` / `Bitmaps.hpp` / `Structs.hpp` from scratch, rewrite `Types.hpp` (drop the stale `GeoPosition.hpp` include), and assert `Aliases.hpp` stays absent. The original brief's "delete `Aliases.hpp`" criterion was vacuous.
4. **Enum symmetry**: the `*_SIZE` constant in `Enums.hpp` is dropped (same logic as `*_BITMAP_SIZE` — duplicates information that's obvious at the use site).
5. **ADR-0001 created** capturing the "raw primitives in message struct, semantic types live alongside" trade-off — this was load-bearing enough that without it, a future maintainer would plausibly "fix" the perceived weakness by retyping fields.

`CONTEXT.md` and `docs/adr/0001-message-struct-uses-raw-primitives.md` were created during the session.





