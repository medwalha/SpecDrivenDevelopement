// SPDX-License-Identifier: TODO
// Consolidated header for all reusable bitmap mask constants extracted from
// the PDF spec via /pdf-to-cpp-types.
//
// House rules (see CONTEXT.md > "Bitmap entry" and docs/adr/0001):
//   - No `using <Name>Bitmap = uint8;` typedef-alias. Message struct fields
//     declare the underlying primitive directly (e.g. `uint8 m_TradeType;`).
//   - Only named `*_MASK` constants live here; optionally a `*_NONE_VALUE`
//     sentinel when the spec explicitly names a "no flags set" value.
//   - No `*_BITMAP_SIZE` / `*_SIZE` constant — the width is at the
//     field-declaration site in the message struct.
//   - Keep blocks alphabetical. One block per bitmap.

#pragma once

#include "messages/Common.hpp"

namespace protocol::messages::types {

// === <Nom>Bitmap — §<section> ===============================================
// /// <BRIEF_DESCRIPTION>
// /// @see <pdf> §<section>
// // Carried in a `uint8 m_<Field>;` slot in the owning message struct.
// inline constexpr uint8 <NAME>_NONE_VALUE  = 0x00;   // optional sentinel
// inline constexpr uint8 <NAME>_<FLAG_A>_MASK = 0x01;
// inline constexpr uint8 <NAME>_<FLAG_B>_MASK = 0x02;
// inline constexpr uint8 <NAME>_<FLAG_C>_MASK = 0x04;

} // namespace protocol::messages::types
