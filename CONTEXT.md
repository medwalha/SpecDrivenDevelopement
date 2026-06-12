# SpecDrivenDevelopement

Workflow + conventions for generating C++ message structures from a PDF specification (binary protocol / ICD). The repo contains prompts (`/pdf-to-cpp-types`, `/pdf-to-cpp-messages`), instruction files, templates, and a fixed `include/messages/` tree that consumer projects can vendor.

## Language

**Message struct** (`<Name>Struct`):
The packed (`#pragma pack(push, 1)`) C++ struct that mirrors the binary spec field-for-field. Always paired with a wrapper class `<Name>` that owns a `MessageHeader` and a `const &` reference into the payload buffer.
_Avoid_: "POD", "DTO", "payload struct".

**Message wrapper** (`<Name>`):
The non-packed class that exposes typed getters, `dump()`, and `operator<<` over a `<Name>Struct`. It never copies the payload — only `reinterpret_cast`s a `const &` onto it.
_Avoid_: "message class", "view".

**House typedefs**:
The fixed-width scalar aliases declared in `include/messages/Common.hpp` — `uint8`, `int8`, `uint16`, `int16`, `uint32`, `int32`, `uint64`, `int64`. **The only typedef-aliases allowed in message struct fields.**
_Avoid_: `std::uint8_t`, `unsigned int`, `int`, `long`.

**Domain stub**:
A placeholder type the consumer project is expected to replace with its real type — currently `Decimal`, `TimestampNano`, and `InputData` in `Common.hpp`. They are not extracted from a spec PDF and do not belong under `types/`.
_Avoid_: "common type", "shared type" (those mean something else here — see below).

**Typedef-alias** (forbidden under `types/`):
A pure renaming of a primitive or array, e.g. `using Isin = char[12]` or `using TradeTypeBitmap = uint8`. These are **not** added to the project — message struct fields use the underlying primitive directly (`char m_SecurityID[12]`, `uint8 m_TradeType`).
_Avoid_: "alias" (ambiguous — see "Alias category" below), "typedef".

**Alias category** (deprecated):
The historical fourth category of `/pdf-to-cpp-types` (alongside `enum` / `bitmap` / `struct`) that produced typedef-aliases. **Removed.** `/pdf-to-cpp-types` now accepts only three categories.

**Reusable type** (under `include/messages/types/`):
A type extracted from the PDF spec that genuinely encapsulates structure — currently restricted to **enums** (`enum class X : uint8`), **bitmap masks** (`inline constexpr uint8 X_FLAG_MASK = 0x01;`), and **composed structs** (`#pragma pack`ed structs with ≥ 2 fields). Lives in `Enums.hpp`, `Bitmaps.hpp`, or `Structs.hpp` respectively. **Never substituted into a message struct field type** — used only by surrounding business logic for semantic validation.
_Avoid_: "shared type", "common type" (`Common.hpp` is its own thing — domain stubs only).

**Semantic-only usage**:
A reusable type is referenced from documentation, comments, and business-logic validation — but the message struct field that *carries* its bytes is typed as the raw underlying primitive (`uint8` for an enum/bitmap, the composed struct's name itself for a `Structs.hpp` entry). This trade-off preserves a strict 1:1 binary mapping in the struct while still publishing the semantic vocabulary.

**Bitmap entry**:
A block in `Bitmaps.hpp` for a spec-defined flag set. Contains: one or more `inline constexpr <UnderlyingPrimitive> <NAME>_<FLAG>_MASK = 0x..;` constants, and optionally one `inline constexpr <UnderlyingPrimitive> <NAME>_NONE_VALUE = 0x00;` sentinel when the spec names a "no flags set" value. No `using <Name>Bitmap = …;`, no `_BITMAP_SIZE` / `_SIZE` constant — the underlying width is declared at the field-declaration site in the message struct.

**Enum entry**:
A block in `Enums.hpp` for a spec-defined scalar enumeration. Contains: `enum class <Name> : <UnderlyingPrimitive> { … };` with values named PascalCase (or `char` literals when the spec uses ASCII). No companion `_SIZE` constant — `sizeof(enum class)` is obvious at the use site, and the enum is not substituted into the message struct field anyway.

**Struct entry**:
A block in `Structs.hpp` for a spec-defined composed binary type with ≥ 2 fields (e.g. `Decimal` *as defined in a real spec*, `RptGrpHeader`, `FramingHeader`, `UnitHeader`). Always wrapped in `#pragma pack(push, 1)` / `#pragma pack(pop)` and accompanied by `static_assert`s on `sizeof`, `is_standard_layout`, `is_trivially_copyable`. The **only** category whose name is allowed inside a message struct field type.

**Spec section**:
A PDF subdivision (`§4.3.2`, `Table 7`, `pages 18-20`) that defines one or more types/messages. Every generated block carries a `// === <Name> — §<section> ===` separator and a Doxygen `@see` tag back to it.
_Avoid_: "chapter" (too coarse), "page" (too brittle).

**MessageHeader / PacketHeader**:
Protocol-specific framing prepended to each message (`MessageHeader`) and optionally each transport packet (`PacketHeader`). Their layout is **not** extracted from the spec by these prompts — the consumer supplies them as the blocking étape 0 of `/pdf-to-cpp-messages`. Stub files exist under `include/messages/` until replaced.
