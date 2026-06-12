// SPDX-License-Identifier: TODO
// Consolidated header for all reusable composed binary structs extracted from
// the PDF spec via /pdf-to-cpp-types
// (e.g. RptGrpHeader, FramingHeader, UnitHeader, ...).
//
// House rules (see CONTEXT.md > "Struct entry" and docs/adr/0001):
//   - This is the ONLY `types/` category whose name is allowed to appear in a
//     message struct field type.
//   - Wrap every struct in `#pragma pack(push, 1)` / `#pragma pack(pop)`.
//   - Field prefix `m_`, PascalCase. Use house typedefs from Common.hpp.
//   - Companion `static_assert`s on sizeof / is_standard_layout /
//     is_trivially_copyable are required. A `<NAME>_SIZE` named constant is
//     optional and only useful when external code needs it.
//   - Do NOT define `Decimal` or `TimestampNano` here — they are domain stubs
//     declared in `messages/Common.hpp` and replaced by the consumer project.
//   - Keep blocks alphabetical. One block per struct.

#pragma once

#include <ostream>
#include <type_traits>

#include "messages/Common.hpp"

namespace protocol::messages::types {

// === <NomType> — §<section> =================================================
// /// <BRIEF_DESCRIPTION>
// /// @see <pdf> §<section>
// #pragma pack(push, 1)
// struct <NomType>
// {
//     int32  m_FieldA;
//     uint32 m_FieldB;
//     char   m_FieldC[8];
// };
// #pragma pack(pop)
// static_assert(sizeof(<NomType>) == 16,                          "<NomType> size mismatch with spec");
// static_assert(std::is_standard_layout<<NomType>>::value,        "<NomType> must be standard-layout");
// static_assert(std::is_trivially_copyable<<NomType>>::value,     "<NomType> must be trivially copyable");
// inline constexpr unsigned int <NAME>_SIZE = sizeof(<NomType>);  // optional

} // namespace protocol::messages::types
