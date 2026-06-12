// SPDX-License-Identifier: TODO
// Common typedefs, dump macros, and InputData abstraction shared by all messages.

#pragma once

#include <cstdint>
#include <cstddef>
#include <ostream>
#include <string>
#include <vector>

namespace protocol::messages {

// ---------------------------------------------------------------------------
// Fixed-width typedefs — REQUIRED in every message struct field.
// Do NOT use std::uint*_t or built-in int/long inside message structs.
// ---------------------------------------------------------------------------
using uint8  = std::uint8_t;
using int8   = std::int8_t;
using uint16 = std::uint16_t;
using int16  = std::int16_t;
using uint32 = std::uint32_t;
using int32  = std::int32_t;
using uint64 = std::uint64_t;
using int64  = std::int64_t;

// ---------------------------------------------------------------------------
// Domain structs (Decimal, TimestampNano, RptGrpHeader, FramingHeader,
// UnitHeader, GeoPosition) live in include/messages/types/Structs.hpp.
// They are pulled in at the bottom of this header for backward compatibility:
// legacy code referring to `protocol::messages::Decimal` keeps compiling
// thanks to the using-aliases re-exported from Structs.hpp.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// InputData — opaque view over a raw byte buffer.
// TODO: replace by the project's real InputData type if different.
// ---------------------------------------------------------------------------
class InputData
{
    std::vector<uint8> m_Data;
public:
    InputData() = default;
    explicit InputData(std::vector<uint8> bytes) : m_Data(std::move(bytes)) {}

    [[nodiscard]] uint8 const* data() const noexcept { return m_Data.data(); }
    [[nodiscard]] std::size_t  size() const noexcept { return m_Data.size(); }
};

// ---------------------------------------------------------------------------
// dump() helper macros.
// Usage:
//   os
//   DUMP_FIELD(MyField)
//   DUMP_FIELD_AS_INT(MyByte)
//   ;
// Output format:  "  MyField=<value>\n"
// ---------------------------------------------------------------------------
#define DUMP_FIELD(NAME)        << "  " #NAME "=" << get##NAME() << "\n"
#define DUMP_FIELD_AS_INT(NAME) << "  " #NAME "=" << static_cast<int>(get##NAME()) << "\n"

} // namespace protocol::messages

// ---------------------------------------------------------------------------
// Pull in the consolidated reusable structs AFTER the basic typedefs and
// InputData / DUMP_FIELD have been declared. Structs.hpp itself includes
// Common.hpp, but `#pragma once` short-circuits the recursion.
// ---------------------------------------------------------------------------
#include "messages/types/Structs.hpp"

