// SPDX-License-Identifier: TODO
// All reusable typedefs / `using` aliases extracted from the PDF spec.
// Sections are kept alphabetical. Add new entries via /pdf-to-cpp-types.
//
// Source : specs/euro_ctp_spec.pdf — section §4.1.2 (Data types)
// Spec version: 1.3   Generated: 2026-06-12
// Do not edit manually unless spec changed.

#pragma once

#include "messages/Common.hpp"

namespace protocol::messages::types {

// === AppId — §4.1.2 ==========================================================
/// Client application identifier — alphanumerical, fixed 32 bytes (ASCII).
/// Used in the Logon message (ClientAppId field).
using AppId = char[32];

inline constexpr unsigned int APP_ID_SIZE = 32;
static_assert(sizeof(AppId) == APP_ID_SIZE,
              "AppId size mismatch with spec (32 bytes)");

// === AppVersion — §4.1.2 =====================================================
/// Client application version — alphanumerical, fixed 16 bytes (ASCII).
/// Used in the Logon message (ClientAppVersion field).
using AppVersion = char[16];

inline constexpr unsigned int APP_VERSION_SIZE = 16;
static_assert(sizeof(AppVersion) == APP_VERSION_SIZE,
              "AppVersion size mismatch with spec (16 bytes)");

// === Currency — §4.1.2 =======================================================
/// ISO 4217 currency code — alpha, fixed 3 bytes (ASCII).
using Currency = char[3];

inline constexpr unsigned int CURRENCY_SIZE = 3;
static_assert(sizeof(Currency) == CURRENCY_SIZE,
              "Currency size mismatch with spec (3 bytes)");

// === EsmaCode — §4.1.2 =======================================================
/// ESMA reference code — alpha, fixed 4 bytes (ASCII).
using EsmaCode = char[4];

inline constexpr unsigned int ESMA_CODE_SIZE = 4;
static_assert(sizeof(EsmaCode) == ESMA_CODE_SIZE,
              "EsmaCode size mismatch with spec (4 bytes)");

// === InternalIdentifier — §4.1.2 =============================================
/// Internal identifier (e.g. EBBO/EBAP/EFBA/Trade IDs) — int64, 8 bytes (LE).
using InternalIdentifier = int64;

inline constexpr unsigned int INTERNAL_IDENTIFIER_SIZE = 8;
static_assert(sizeof(InternalIdentifier) == INTERNAL_IDENTIFIER_SIZE,
              "InternalIdentifier size mismatch with spec (8 bytes)");

// === Isin — §4.1.2 ===========================================================
/// ISO 6166 instrument identifier (ISIN) — alphanumerical, fixed 12 bytes (ASCII).
using Isin = char[12];

inline constexpr unsigned int ISIN_SIZE = 12;
static_assert(sizeof(Isin) == ISIN_SIZE,
              "Isin size mismatch with spec (12 bytes)");

// === LogoutReason — §4.1.2 ===================================================
/// Free-form logout reason text — alphanumerical, fixed 256 bytes (ASCII).
/// Carried by the Logout administrative message (see §4.2.3).
using LogoutReason = char[256];

inline constexpr unsigned int LOGOUT_REASON_SIZE = 256;
static_assert(sizeof(LogoutReason) == LOGOUT_REASON_SIZE,
              "LogoutReason size mismatch with spec (256 bytes)");

// === Mic — §4.1.2 ============================================================
/// ISO 10383 Market Identifier Code — alpha, fixed 4 bytes (ASCII).
/// Used for SecurityExchange (207), LastMkt (30), MarketID (1301), PartyID (448), etc.
using Mic = char[4];

inline constexpr unsigned int MIC_SIZE = 4;
static_assert(sizeof(Mic) == MIC_SIZE,
              "Mic size mismatch with spec (4 bytes)");

// === MmtFlag — §4.1.2 ========================================================
/// Market Model Typology (MMT) flag — efficient encoding, fixed 14 bytes (ASCII).
/// Built by EuroCTP from contributors' input following MMT 5.0 standard.
/// Each character represents a position starting at 1; '?' = no value for a
/// mandatory position.
using MmtFlag = char[14];

inline constexpr unsigned int MMT_FLAG_SIZE = 14;
static_assert(sizeof(MmtFlag) == MMT_FLAG_SIZE,
              "MmtFlag size mismatch with spec (14 bytes)");

// === Password — §4.1.2 =======================================================
/// Logon password — alphanumerical, fixed 32 bytes (ASCII).
using Password = char[32];

inline constexpr unsigned int PASSWORD_SIZE = 32;
static_assert(sizeof(Password) == PASSWORD_SIZE,
              "Password size mismatch with spec (32 bytes)");

// === SeqNum — §4.1.2 =========================================================
/// Sequence number (instrument-level / packet-level) — int64, 8 bytes (LE).
using SeqNum = int64;

inline constexpr unsigned int SEQ_NUM_SIZE = 8;
static_assert(sizeof(SeqNum) == SEQ_NUM_SIZE,
              "SeqNum size mismatch with spec (8 bytes)");

// === TransactionIdentifier — §4.1.2 ==========================================
/// Trading Venue Transaction Identification Code (TVTIC) — alphanumerical, 52 B (ASCII).
/// Identifies a single transaction per Article 12 of Commission Delegated Regulation
/// (EU) 2017/580. Carried in RegulatoryTradeID (1903) of post-trade messages.
using TransactionIdentifier = char[52];

inline constexpr unsigned int TRANSACTION_IDENTIFIER_SIZE = 52;
static_assert(sizeof(TransactionIdentifier) == TRANSACTION_IDENTIFIER_SIZE,
              "TransactionIdentifier size mismatch with spec (52 bytes)");

// === Username — §4.1.2 =======================================================
/// Logon username — alphanumerical, fixed 32 bytes (ASCII).
using Username = char[32];

inline constexpr unsigned int USERNAME_SIZE = 32;
static_assert(sizeof(Username) == USERNAME_SIZE,
              "Username size mismatch with spec (32 bytes)");

} // namespace protocol::messages::types
