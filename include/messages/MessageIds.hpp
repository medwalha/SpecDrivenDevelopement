// SPDX-License-Identifier: TODO
// Auto-generated message ID registry — keep in sync with the spec.
// Source : specs/euro_ctp_spec.pdf — sections §4.1.1.1 / §4.1.1.2
// Spec version: 1.3   Generated: 2026-06-12

#pragma once

#include "messages/Common.hpp"

namespace protocol::messages {

// ---------------------------------------------------------------------------
// Administrative messages — SchemaID = 2 (§4.1.1.1)
// ---------------------------------------------------------------------------
inline constexpr uint16 MSG_ID_HEARTBEAT       = 1;
inline constexpr uint16 MSG_ID_SEQUENCE_RESET  = 2;
inline constexpr uint16 MSG_ID_LOGON           = 10;
inline constexpr uint16 MSG_ID_LOGOUT          = 11;
inline constexpr uint16 MSG_ID_REPLAY_REQUEST  = 20;
inline constexpr uint16 MSG_ID_REPLAY_START    = 21;
inline constexpr uint16 MSG_ID_SNAPSHOT_END    = 30;

// ---------------------------------------------------------------------------
// Application messages — SchemaID = 1 (§4.1.1.2)
// ---------------------------------------------------------------------------
inline constexpr uint16 MSG_ID_EBBO                          = 101;
inline constexpr uint16 MSG_ID_EBAP                          = 102;
inline constexpr uint16 MSG_ID_EFBA                          = 103;
inline constexpr uint16 MSG_ID_POST_TRADE                    = 201;
inline constexpr uint16 MSG_ID_INSTRUMENT_STATUS             = 301;
inline constexpr uint16 MSG_ID_ORDER_MATCHING_SYSTEM_STATUS  = 302;

} // namespace protocol::messages
