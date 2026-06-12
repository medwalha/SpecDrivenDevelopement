// SPDX-License-Identifier: TODO
// Consolidated header for all reusable scalar enumerations extracted from the
// PDF spec via /pdf-to-cpp-types.
//
// House rules (see CONTEXT.md > "Enum entry" and docs/adr/0001):
//   - Use `enum class <Name> : <underlying primitive>` only.
//   - The underlying primitive comes from messages/Common.hpp
//     (uint8 / uint16 / int8 / ...). Never `std::uint*_t`.
//   - No companion `*_SIZE` constant — `sizeof(<Name>)` is obvious at the use
//     site and the enum is *never* substituted into a message struct field.
//   - Inline `static_assert(sizeof(<Name>) == N, "...")` is fine when the spec
//     pins the width.
//   - Keep blocks alphabetical. One block per enum.

#pragma once

#include <type_traits>

#include "messages/Common.hpp"

namespace protocol::messages::types {

// === <NomEnum> — §<section> =================================================
// /// <BRIEF_DESCRIPTION>
// /// @see <pdf> §<section>
// enum class <NomEnum> : uint8
// {
//     OkValue    = 0x00,
//     ErrorValue = 0xFF,
// };
// static_assert(sizeof(<NomEnum>) == 1, "<NomEnum> must be 1 byte per spec");

} // namespace protocol::messages::types
