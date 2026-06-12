// SPDX-License-Identifier: TODO
// All reusable bitmap aliases + UPPER_SNAKE masks extracted from the PDF spec.
// Sections are kept alphabetical. Add new entries via /pdf-to-cpp-types.
//
// Source : specs/euro_ctp_spec.pdf — sections §4.2.8 / §4.2.11
// Spec version: 1.3   Generated: 2026-06-12
// Do not edit manually unless spec changed.

#pragma once

#include "messages/Common.hpp"

namespace protocol::messages::types {

// === MdQualityIndicatorBitmap — §4.2.11 ======================================
/// MDQualityIndicator (FIX Tag 3105) — suspicious-trade flag bitmap.
/// Spec values: 0, 1, 2, 4 — described as a bitmap. Use the masks below to
/// test bits with bitwise AND. Value 0 (no mask set) means 'No data quality issue'.
using MdQualityIndicatorBitmap = uint8;

inline constexpr uint8 MD_QUALITY_INDICATOR_NONE_VALUE              = 0x00;  ///< 'FALSE' (no flag set).
inline constexpr uint8 MD_QUALITY_INDICATOR_SUSPICIOUS_TRADE_MASK   = 0x01;  ///< 'TRUE' — Suspicious trade.
inline constexpr uint8 MD_QUALITY_INDICATOR_SIZE_DEVIATION_MASK     = 0x02;  ///< Suspicious due to size deviation.
inline constexpr uint8 MD_QUALITY_INDICATOR_PRICE_DEVIATION_MASK    = 0x04;  ///< Suspicious due to price deviation.

inline constexpr unsigned int MD_QUALITY_INDICATOR_BITMAP_SIZE = sizeof(MdQualityIndicatorBitmap);
static_assert(MD_QUALITY_INDICATOR_BITMAP_SIZE == 1,
              "MdQualityIndicatorBitmap size mismatch with spec (1 byte)");

// === QuoteConditionsBitmap — §§4.2.8 / 4.2.9 / 4.2.10 ========================
/// QuoteConditions (FIX Tag 276) — quote conditions bitmap.
/// EBBO supports values "0, 1, 2, 3, 4, 6, 8 or 10" — bitwise combinations.
/// EBAP / EFBA only use NONE or the OutOfSequence bit.
using QuoteConditionsBitmap = uint8;

inline constexpr uint8 QUOTE_CONDITIONS_NONE_VALUE                       = 0x00;  ///< none.
inline constexpr uint8 QUOTE_CONDITIONS_LOCKED_MASK                      = 0x01;  ///< 'E' Locked.
inline constexpr uint8 QUOTE_CONDITIONS_OUT_OF_SEQUENCE_MASK             = 0x02;  ///< 'o' Out of sequence.
inline constexpr uint8 QUOTE_CONDITIONS_CROSSED_DUE_TO_LATEST_BID_MASK   = 0x04;  ///< '8' Crossed (latest bid).
inline constexpr uint8 QUOTE_CONDITIONS_CROSSED_DUE_TO_LATEST_OFFER_MASK = 0x08;  ///< '9' Crossed (latest offer).

inline constexpr unsigned int QUOTE_CONDITIONS_BITMAP_SIZE = sizeof(QuoteConditionsBitmap);
static_assert(QUOTE_CONDITIONS_BITMAP_SIZE == 1,
              "QuoteConditionsBitmap size mismatch with spec (1 byte)");

// === TradePriceConditionBitmap — §4.2.11 =====================================
/// TradePriceCondition (FIX Tag 1839) — repeating-group content encoded as a bitmap.
/// NoTradePriceConditions (1838) is constant 0 — the group content is collapsed
/// into the single byte below.
using TradePriceConditionBitmap = uint8;

inline constexpr uint8 TRADE_PRICE_CONDITION_SPECIAL_DIVIDEND_MASK     = 0x01;  ///< '13' 'SDIV'.
inline constexpr uint8 TRADE_PRICE_CONDITION_NON_PRICE_FORMING_MASK    = 0x02;  ///< '15' 'NPFT'.
inline constexpr uint8 TRADE_PRICE_CONDITION_PRICE_PENDING_MASK        = 0x04;  ///< '17' 'PNDG'.
inline constexpr uint8 TRADE_PRICE_CONDITION_PRICE_NOT_APPLICABLE_MASK = 0x08;  ///< '18' 'NOAP'.

inline constexpr unsigned int TRADE_PRICE_CONDITION_BITMAP_SIZE = sizeof(TradePriceConditionBitmap);
static_assert(TRADE_PRICE_CONDITION_BITMAP_SIZE == 1,
              "TradePriceConditionBitmap size mismatch with spec (1 byte)");

// === TradeTypeBitmap — §4.2.11 ===============================================
/// TradeType (FIX Tag 3006) — repeating-group content encoded as a bitmap.
/// NoTradeTypes (3005) is constant 0 — the group content is collapsed into
/// the single byte below.
using TradeTypeBitmap = uint8;

inline constexpr uint8 TRADE_TYPE_PORTFOLIO_TRADE_MASK = 0x01;  ///< '50' 'PORT'.
inline constexpr uint8 TRADE_TYPE_BENCHMARK_MASK       = 0x02;  ///< '64' 'BENC'.
inline constexpr uint8 TRADE_TYPE_PACKAGE_TRADE_MASK   = 0x04;  ///< '65' 'CONT'.

inline constexpr unsigned int TRADE_TYPE_BITMAP_SIZE = sizeof(TradeTypeBitmap);
static_assert(TRADE_TYPE_BITMAP_SIZE == 1,
              "TradeTypeBitmap size mismatch with spec (1 byte)");

// === TrdRegPublicationReasonBitmap — §4.2.11 =================================
/// TrdRegPublicationReason (FIX Tag 2670) — repeating-group content as a bitmap.
/// NoTrdRegPublications (2668) is constant 0 — the group content is collapsed
/// into the single byte below.
/// LRGS (16) corresponds to TrdRegPublicationType = PostTradeDeferral; the four
/// other bits correspond to TrdRegPublicationType = PreTradeTransparencyWaiver.
using TrdRegPublicationReasonBitmap = uint8;

inline constexpr uint8 TRD_REG_PUBLICATION_REASON_NLIQ_MASK = 0x01;  ///< 'NLIQ'.
inline constexpr uint8 TRD_REG_PUBLICATION_REASON_OILQ_MASK = 0x02;  ///< 'OILQ'.
inline constexpr uint8 TRD_REG_PUBLICATION_REASON_PRIC_MASK = 0x04;  ///< 'PRIC'.
inline constexpr uint8 TRD_REG_PUBLICATION_REASON_RFPT_MASK = 0x08;  ///< 'RFPT'.
inline constexpr uint8 TRD_REG_PUBLICATION_REASON_LRGS_MASK = 0x10;  ///< 'LRGS' (Large in Scale deferral).

inline constexpr unsigned int TRD_REG_PUBLICATION_REASON_BITMAP_SIZE = sizeof(TrdRegPublicationReasonBitmap);
static_assert(TRD_REG_PUBLICATION_REASON_BITMAP_SIZE == 1,
              "TrdRegPublicationReasonBitmap size mismatch with spec (1 byte)");

} // namespace protocol::messages::types
