// SPDX-License-Identifier: TODO
// Auto-generated from sample_spec.pdf — section §4.3.2 (Table 7)
// Spec version: 1.0   Generated: 2026-06-12
// Category: struct
// Do not edit manually unless spec changed.

#pragma once

#include <type_traits>

#include "messages/Common.hpp"

namespace protocol::messages::types {

#pragma pack(push, 1)

/// WGS-84 geographic position with altitude.
/// @see sample_spec.pdf §4.3.2 — Table 7
struct GeoPosition
{
    int32  m_LatitudeE7;     ///< Latitude  in 1e-7 deg, range [-90e7, +90e7].
    int32  m_LongitudeE7;    ///< Longitude in 1e-7 deg, range [-180e7, +180e7].
    int32  m_AltitudeMm;     ///< Altitude above ellipsoid, millimeters.
    uint32 m_AccuracyMm;     ///< Horizontal 1-sigma accuracy, millimeters.
    uint32 m_TimestampMs;    ///< GPS time of fix, milliseconds (mod 2^32).
};

#pragma pack(pop)

static_assert(sizeof(GeoPosition) == 20,
              "GeoPosition size mismatch with spec (20 bytes)");
static_assert(std::is_standard_layout<GeoPosition>::value,
              "GeoPosition must be standard-layout");
static_assert(std::is_trivially_copyable<GeoPosition>::value,
              "GeoPosition must be trivially copyable");

} // namespace protocol::messages::types
