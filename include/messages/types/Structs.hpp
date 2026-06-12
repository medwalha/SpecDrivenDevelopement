// SPDX-License-Identifier: TODO
// All reusable composed binary structs extracted from the PDF spec.
// Sections are kept alphabetical. Add new entries via /pdf-to-cpp-types.
//
// Source : specs/euro_ctp_spec.pdf — sections §4.1.2, §4.1.3, §4.1.6, §4.1.7
// Spec version: 1.3   Generated: 2026-06-12
// Do not edit manually unless spec changed.

#pragma once

#include <ostream>
#include <type_traits>

#include "messages/Common.hpp"

namespace protocol::messages::types {

// === Decimal — §4.1.2 ========================================================
/// Floating-point decimal used for prices and sizes (FIX SBE encoding).
/// Value = m_Mantissa * 10^m_Exponent. Up to 17 decimal places per spec.
/// Wire layout (little-endian): int64 mantissa (8B) || int8 exponent (1B) = 9B.
#pragma pack(push, 1)
struct Decimal
{
    int64 m_Mantissa;    ///< Signed 64-bit mantissa.
    int8  m_Exponent;    ///< Decimal exponent (typically negative).
};
#pragma pack(pop)

inline std::ostream& operator<<(std::ostream& os, Decimal const& d)
{
    return os << d.m_Mantissa << "e" << static_cast<int>(d.m_Exponent);
}

inline constexpr unsigned int DECIMAL_SIZE = 9;
static_assert(sizeof(Decimal) == DECIMAL_SIZE,
              "Decimal size mismatch with spec (9 bytes)");
static_assert(std::is_standard_layout<Decimal>::value,
              "Decimal must be standard-layout");
static_assert(std::is_trivially_copyable<Decimal>::value,
              "Decimal must be trivially copyable");

// === FramingHeader — §4.1.6 ==================================================
/// SBE framing header — supports stream-based message framing.
/// Precedes the UnitHeader on the wire. Wire layout (little-endian): uint16 (2B).
#pragma pack(push, 1)
struct FramingHeader
{
    uint16 m_MessageLength;    ///< Overall message length including headers, in bytes.
};
#pragma pack(pop)

inline constexpr unsigned int FRAMING_HEADER_SIZE = 2;
static_assert(sizeof(FramingHeader) == FRAMING_HEADER_SIZE,
              "FramingHeader size mismatch with spec (2 bytes)");
static_assert(std::is_standard_layout<FramingHeader>::value,
              "FramingHeader must be standard-layout");
static_assert(std::is_trivially_copyable<FramingHeader>::value,
              "FramingHeader must be trivially copyable");

// === GeoPosition — §4.3.2 (legacy sample) ====================================
/// WGS-84 geographic position with altitude.
/// Kept from the original sample spec to demonstrate composed binary structs.
#pragma pack(push, 1)
struct GeoPosition
{
    int32  m_LatitudeE7;     ///< Latitude  in 1e-7 deg, range [-90e7, +90e7].
    int32  m_LongitudeE7;    ///< Longitude in 1e-7 deg, range [-180e7, +180e7].
    int32  m_AltitudeMm;     ///< Altitude above ellipsoid, millimeters.
    uint32 m_AccuracyMm;     ///< Horizontal 1-sigma accuracy, millimeters.
    uint32 m_TimestampMs;    ///< GPS time of fix, milliseconds (mod 2^32).
};
#pragma pack(pop)

inline constexpr unsigned int GEO_POSITION_SIZE = 20;
static_assert(sizeof(GeoPosition) == GEO_POSITION_SIZE,
              "GeoPosition size mismatch with spec (20 bytes)");
static_assert(std::is_standard_layout<GeoPosition>::value,
              "GeoPosition must be standard-layout");
static_assert(std::is_trivially_copyable<GeoPosition>::value,
              "GeoPosition must be trivially copyable");

// === RptGrpHeader — §4.1.2 ===================================================
/// SBE repeating-group header.
/// Wire layout (little-endian): uint8 entryLength (1B) || uint8 numOfEntries (1B) = 2B.
#pragma pack(push, 1)
struct RptGrpHeader
{
    uint8 m_EntryLength;     ///< Fixed-length size, in bytes, of one group entry's fixed fields.
    uint8 m_NumOfEntries;    ///< Number of entries in the repeating group.
};
#pragma pack(pop)

inline constexpr unsigned int RPT_GRP_HEADER_SIZE = 2;
static_assert(sizeof(RptGrpHeader) == RPT_GRP_HEADER_SIZE,
              "RptGrpHeader size mismatch with spec (2 bytes)");
static_assert(std::is_standard_layout<RptGrpHeader>::value,
              "RptGrpHeader must be standard-layout");
static_assert(std::is_trivially_copyable<RptGrpHeader>::value,
              "RptGrpHeader must be trivially copyable");

// === TimestampNano — §§4.1.2 / 4.1.3 =========================================
/// Nanoseconds-since-Epoch timestamp.
/// Epoch base = 1970-01-01 (UTC); will be rebased on 2036-01-01 per spec.
/// Wire layout (little-endian): uint64 (8B).
#pragma pack(push, 1)
struct TimestampNano
{
    uint64 m_Ns;    ///< Nanoseconds since Epoch.
};
#pragma pack(pop)

inline std::ostream& operator<<(std::ostream& os, TimestampNano const& t)
{
    return os << t.m_Ns << "ns";
}

inline constexpr unsigned int TIMESTAMP_NANO_SIZE = 8;
static_assert(sizeof(TimestampNano) == TIMESTAMP_NANO_SIZE,
              "TimestampNano size mismatch with spec (8 bytes)");
static_assert(std::is_standard_layout<TimestampNano>::value,
              "TimestampNano must be standard-layout");
static_assert(std::is_trivially_copyable<TimestampNano>::value,
              "TimestampNano must be trivially copyable");

// === UnitHeader — §4.1.7 =====================================================
/// SBE unit header — identifies the SBE template used to decode the payload.
/// Follows the FramingHeader on the wire.
/// Wire layout (little-endian): uint16 (2B) × 4 = 8 B.
#pragma pack(push, 1)
struct UnitHeader
{
    uint16 m_MsgSize;       ///< Length of the entire message including the message header (bytes).
    uint16 m_TemplateID;    ///< Template ID — message type identifier (see §4.1.1).
    uint16 m_SchemaID;      ///< Schema ID: 1 = Application, 2 = Administrative.
    uint16 m_Version;       ///< Schema version used for backward compatibility.
};
#pragma pack(pop)

inline constexpr unsigned int UNIT_HEADER_SIZE = 8;
static_assert(sizeof(UnitHeader) == UNIT_HEADER_SIZE,
              "UnitHeader size mismatch with spec (8 bytes)");
static_assert(std::is_standard_layout<UnitHeader>::value,
              "UnitHeader must be standard-layout");
static_assert(std::is_trivially_copyable<UnitHeader>::value,
              "UnitHeader must be trivially copyable");

} // namespace protocol::messages::types

// =============================================================================
// Backward-compat aliases — re-export the most-used domain structs in the
// parent namespace `protocol::messages` so legacy code (e.g.
// examples/PostTrade.hpp) referring to `Decimal` / `TimestampNano` keeps
// compiling without qualifying with `types::`.
// =============================================================================
namespace protocol::messages {
using Decimal       = types::Decimal;
using TimestampNano = types::TimestampNano;
} // namespace protocol::messages
