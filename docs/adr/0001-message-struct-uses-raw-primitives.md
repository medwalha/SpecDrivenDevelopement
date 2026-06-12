# Message struct fields use raw primitives, not semantic types

Status: accepted
Date: 2026-06-12

Message struct fields (`<Name>Struct` under `include/messages/`) declare each field with the underlying binary primitive only — `uint8`, `int64`, `char[N]`, or a composed `Structs.hpp` type. Enum classes from `Enums.hpp` and bitmap mask constants from `Bitmaps.hpp` are **never** used as field types; they are referenced separately by business logic for semantic validation. This trade-off preserves a strict 1:1 binary mapping between the spec and the struct, at the cost of weaker type safety on enum/bitmap fields.

## Considered options

- **Type fields by their semantic type** (e.g. `TradeType m_TradeType;`, `Isin m_SecurityID;`). Rejected: the struct stops being a mechanical transliteration of the binary spec, the consumer must trust the mapping, `reinterpret_cast` correctness becomes harder to audit visually, and `enum class` field assignment loses the ability to carry spec-permitted values outside the enum (a routine spec evolution).
- **Per-field typedef-aliases** (`using Isin = char[12]; using TradeTypeBitmap = uint8;` in `Aliases.hpp` / `Bitmaps.hpp`, substituted into struct fields). Rejected: introduces a name layer between the spec and the struct, asymmetric across categories (aliases vs bitmaps vs enums), and the canonical example `examples/PostTrade.hpp` already declared fields as raw primitives — the prompts disagreed with the example, not the other way around.

## Consequences

- `include/messages/types/Aliases.hpp` does not exist. `/pdf-to-cpp-types` accepts only three categories: `enum`, `bitmap`, `struct`.
- `Bitmaps.hpp` and `Enums.hpp` carry **named constants and enum classes**, never `using` aliases.
- Composed structs (`Structs.hpp`) are the **only** category whose name appears in a message struct field type — because they encapsulate genuine multi-field structure that cannot be inlined without losing information.
- Enum and bitmap validation is the responsibility of business logic, not the struct definition. Untyped fields imply the system must tolerate unknown values gracefully (spec evolution).
