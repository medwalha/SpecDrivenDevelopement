// SPDX-License-Identifier: TODO
// All reusable scalar enumerations extracted from the PDF spec.
// Sections are kept alphabetical. Add new entries via /pdf-to-cpp-types.
//
// Source : specs/euro_ctp_spec.pdf — sections §3.1.1, §4.1.1.x, §4.1.7, §4.1.8, §4.2.x
// Spec version: 1.3   Generated: 2026-06-12
// Do not edit manually unless spec changed.

#pragma once

#include "messages/Common.hpp"

namespace protocol::messages::types {

// === AdminTemplateId — §4.1.1.1 ==============================================
/// Template ID for administrative messages (SchemaID = 2).
/// Carried in UnitHeader.m_TemplateID.
enum class AdminTemplateId : uint16
{
    Heartbeat       = 1,
    SequenceReset   = 2,
    Logon           = 10,
    Logout          = 11,
    ReplayRequest   = 20,
    ReplayStart     = 21,
    SnapshotEnd     = 30
};

inline constexpr unsigned int ADMIN_TEMPLATE_ID_SIZE = sizeof(AdminTemplateId);
static_assert(ADMIN_TEMPLATE_ID_SIZE == 2,
              "AdminTemplateId underlying size mismatch with spec (2 bytes)");

// === AlgorithmicTradeIndicator — §4.2.11 =====================================
/// AlgorithmicTradeIndicator (FIX Tag 2667) — flags whether the trade was algorithmic.
enum class AlgorithmicTradeIndicator : uint8
{
    NonAlgorithmic = 0,
    Algorithmic    = 1
};

inline constexpr unsigned int ALGORITHMIC_TRADE_INDICATOR_SIZE = sizeof(AlgorithmicTradeIndicator);
static_assert(ALGORITHMIC_TRADE_INDICATOR_SIZE == 1,
              "AlgorithmicTradeIndicator underlying size mismatch with spec (1 byte)");

// === ApplicationTemplateId — §4.1.1.2 ========================================
/// Template ID for application (market data) messages (SchemaID = 1).
/// Carried in UnitHeader.m_TemplateID.
enum class ApplicationTemplateId : uint16
{
    Ebbo                       = 101,
    Ebap                       = 102,
    Efba                       = 103,
    PostTrade                  = 201,
    InstrumentStatus           = 301,
    OrderMatchingSystemStatus  = 302
};

inline constexpr unsigned int APPLICATION_TEMPLATE_ID_SIZE = sizeof(ApplicationTemplateId);
static_assert(APPLICATION_TEMPLATE_ID_SIZE == 2,
              "ApplicationTemplateId underlying size mismatch with spec (2 bytes)");

// === FeedId — §3.1.1 =========================================================
/// Identifies the type of feed carried by a multicast channel.
/// Carried in PacketHeader.m_FeedID.
enum class FeedId : uint8
{
    PreTrade         = 1,
    PostTrade        = 2,
    RegulatoryStatus = 3
};

inline constexpr unsigned int FEED_ID_SIZE = sizeof(FeedId);
static_assert(FEED_ID_SIZE == 1,
              "FeedId underlying size mismatch with spec (1 byte)");

// === LogoutCode — §4.1.8 =====================================================
/// Logout reason code — root cause for session termination.
/// Carried in Logout message LogoutCode (uint16) — see §4.2.3.
/// Note: the spec reserves the unenumerated ranges; do not invent new values.
enum class LogoutCode : uint16
{
    Success                  = 0,
    BadEncoding              = 1,
    UnexpectedMessage        = 2,
    BadMessageLength         = 3,
    InvalidCredentials       = 101,
    TooManyConnections       = 102,
    InvalidChannelId         = 103,
    TooManyReplayRequests    = 104,
    InvalidReplayRequest     = 105,
    BadSequenceVersion       = 201,
    OtherError               = 999
};

inline constexpr unsigned int LOGOUT_CODE_SIZE = sizeof(LogoutCode);
static_assert(LOGOUT_CODE_SIZE == 2,
              "LogoutCode underlying size mismatch with spec (2 bytes)");

// === MatchType — §4.2.12 =====================================================
/// MatchType (FIX Tag 574) — Trading system phase, mutually exclusive with TradingSessionSubID.
enum class MatchType : uint8
{
    Empty                                = 0,
    OffExchangeTradeReporting            = 1,
    OnExchangeTradeReporting             = 3,
    SystematicInternaliserTradeReporting = 9
};

inline constexpr unsigned int MATCH_TYPE_SIZE = sizeof(MatchType);
static_assert(MATCH_TYPE_SIZE == 1,
              "MatchType underlying size mismatch with spec (1 byte)");

// === MdEntryType — §§4.2.8–4.2.11 ============================================
/// MDEntryType (FIX Tag 269) — type of market data entry.
/// Wire encoding: char (1 byte ASCII). Values are aggregated across EBBO, EBAP,
/// EFBA and Post-trade messages (each message constrains a subset).
enum class MdEntryType : char
{
    Bid                  = '0',    ///< EBBO.
    Offer                = '1',    ///< EBBO.
    Trade                = '2',    ///< Post-trade.
    EmptyBook            = 'J',    ///< EBBO/EBAP/EFBA.
    UncrossingBid        = 'j',    ///< EBBO.
    UncrossingOffer      = 'k',    ///< EBBO.
    AuctionClearingPrice = 'Q'     ///< EBAP/EFBA.
};

inline constexpr unsigned int MD_ENTRY_TYPE_SIZE = sizeof(MdEntryType);
static_assert(MD_ENTRY_TYPE_SIZE == 1,
              "MdEntryType underlying size mismatch with spec (1 byte)");

// === MdOriginType — §4.2.11 ==================================================
/// MDOriginType (FIX Tag 1024) — type of trading system on which the instrument is traded.
/// Per §4.2.11 the field is omitted when LastMkt(30) is set to 'SINT' or 'XOFF'.
enum class MdOriginType : uint8
{
    CentralLimitOrderBook = 0,    ///< CLOB.
    QuoteDrivenMarket     = 3,    ///< QDTS.
    AuctionDrivenMarket   = 5,    ///< PATS.
    QuoteNegotiation      = 6,    ///< RFQT.
    HybridMarket          = 8,    ///< HYBR.
    OtherMarket           = 9     ///< OTHR.
};

inline constexpr unsigned int MD_ORIGIN_TYPE_SIZE = sizeof(MdOriginType);
static_assert(MD_ORIGIN_TYPE_SIZE == 1,
              "MdOriginType underlying size mismatch with spec (1 byte)");

// === MdUpdateAction — §4.2.11 ================================================
/// MDUpdateAction (FIX Tag 279) — used when amending or cancelling a previously
/// reported transaction.
enum class MdUpdateAction : uint8
{
    New     = 0,
    Change  = 1,    ///< 'AMND'.
    Delete  = 2     ///< 'CANC'.
};

inline constexpr unsigned int MD_UPDATE_ACTION_SIZE = sizeof(MdUpdateAction);
static_assert(MD_UPDATE_ACTION_SIZE == 1,
              "MdUpdateAction underlying size mismatch with spec (1 byte)");

// === MostLiquidMarketIndicator — §4.2.12 =====================================
/// MostLiquidMarketIndicator (FIX Tag 3103) — most relevant market in terms of liquidity.
enum class MostLiquidMarketIndicator : uint8
{
    False = 0,
    True  = 1
};

inline constexpr unsigned int MOST_LIQUID_MARKET_INDICATOR_SIZE = sizeof(MostLiquidMarketIndicator);
static_assert(MOST_LIQUID_MARKET_INDICATOR_SIZE == 1,
              "MostLiquidMarketIndicator underlying size mismatch with spec (1 byte)");

// === PartyIdSource — §4.2.11 =================================================
/// PartyIDSource (FIX Tag 447) — identifier scheme used for PartyID.
/// Wire encoding: char (always 'G' = MIC per spec).
enum class PartyIdSource : char
{
    MarketIdentifierCode = 'G'    ///< Constant value.
};

inline constexpr unsigned int PARTY_ID_SOURCE_SIZE = sizeof(PartyIdSource);
static_assert(PARTY_ID_SOURCE_SIZE == 1,
              "PartyIdSource underlying size mismatch with spec (1 byte)");

// === PartyRole — §4.2.11 =====================================================
/// PartyRole (FIX Tag 452) — role of the party in the post-trade message.
enum class PartyRole : uint8
{
    ReportOriginator = 62,    ///< Venue of publication, with PartyIDSource = 62.
    ExecutionVenue   = 73     ///< Third-country trading venue, with PartyIDSource = 73.
};

inline constexpr unsigned int PARTY_ROLE_SIZE = sizeof(PartyRole);
static_assert(PARTY_ROLE_SIZE == 1,
              "PartyRole underlying size mismatch with spec (1 byte)");

// === RegulatoryTradeIdType — §4.2.11 =========================================
/// RegulatoryTradeIDType (FIX Tag 1906) — type of transaction identifier.
/// Wire encoding: uint8 (always 5 = TVTIC per spec).
enum class RegulatoryTradeIdType : uint8
{
    TradingVenueTransactionIdentifier = 5    ///< TVTIC, constant value.
};

inline constexpr unsigned int REGULATORY_TRADE_ID_TYPE_SIZE = sizeof(RegulatoryTradeIdType);
static_assert(REGULATORY_TRADE_ID_TYPE_SIZE == 1,
              "RegulatoryTradeIdType underlying size mismatch with spec (1 byte)");

// === SchemaId — §4.1.7 =======================================================
/// Schema identifier — disambiguates the template space carried by UnitHeader.m_TemplateID.
enum class SchemaId : uint16
{
    Application    = 1,    ///< Application messages (market data).
    Administrative = 2     ///< Administrative messages (control flow).
};

inline constexpr unsigned int SCHEMA_ID_SIZE = sizeof(SchemaId);
static_assert(SCHEMA_ID_SIZE == 2,
              "SchemaId underlying size mismatch with spec (2 bytes)");

// === SecurityIdSource — §4.2.x ===============================================
/// SecurityIDSource (FIX Tag 22) — identifier scheme used for SecurityID.
/// Wire encoding: uint8 (always 4 = ISIN per spec).
enum class SecurityIdSource : uint8
{
    Isin = 4    ///< Constant value.
};

inline constexpr unsigned int SECURITY_ID_SOURCE_SIZE = sizeof(SecurityIdSource);
static_assert(SECURITY_ID_SOURCE_SIZE == 1,
              "SecurityIdSource underlying size mismatch with spec (1 byte)");

// === SecurityStatus — §4.2.12 ================================================
/// SecurityStatus (FIX Tag 965) — instrument status (except 'HALT').
enum class SecurityStatus : uint8
{
    Empty     = 0,
    Active    = 1,    ///< 'ACTV' (when there is no previous status).
    Delisted  = 5,    ///< 'RMOV'.
    Suspended = 9     ///< 'SUSP'.
};

inline constexpr unsigned int SECURITY_STATUS_SIZE = sizeof(SecurityStatus);
static_assert(SECURITY_STATUS_SIZE == 1,
              "SecurityStatus underlying size mismatch with spec (1 byte)");

// === SecurityTradingStatus — §4.2.12 =========================================
/// SecurityTradingStatus (FIX Tag 326) — instrument trading status.
/// 'Resume' is used when state changes to 'ACTV' after being in 'SUSP' or 'HALT'.
enum class SecurityTradingStatus : uint8
{
    Empty       = 0,
    TradingHalt = 2,    ///< 'HALT'.
    Resume      = 3     ///< 'ACTV' when previous status was 'SUSP' or 'HALT'.
};

inline constexpr unsigned int SECURITY_TRADING_STATUS_SIZE = sizeof(SecurityTradingStatus);
static_assert(SECURITY_TRADING_STATUS_SIZE == 1,
              "SecurityTradingStatus underlying size mismatch with spec (1 byte)");

// === TradeCondition — §4.2.11 ================================================
/// TradeCondition (FIX Tag 277) — out-of-sequence flag.
enum class TradeCondition : char
{
    False = '0',    ///< 'FALSE'.
    True  = 'k'     ///< 'TRUE'.
};

inline constexpr unsigned int TRADE_CONDITION_SIZE = sizeof(TradeCondition);
static_assert(TRADE_CONDITION_SIZE == 1,
              "TradeCondition underlying size mismatch with spec (1 byte)");

// === TradingSessionId — §§4.2.10, 4.2.12 =====================================
/// TradingSessionID (FIX Tag 336) — trading session identifier.
/// Wire encoding: uint8 (always 1 = Day per spec).
enum class TradingSessionId : uint8
{
    Day = 1    ///< Constant value.
};

inline constexpr unsigned int TRADING_SESSION_ID_SIZE = sizeof(TradingSessionId);
static_assert(TRADING_SESSION_ID_SIZE == 1,
              "TradingSessionId underlying size mismatch with spec (1 byte)");

// === TradingSessionSubId — §§4.2.10, 4.2.12 ==================================
/// TradingSessionSubID (FIX Tag 625) — trading system phase.
/// Mutually exclusive with MatchType (574) in Instrument status messages.
/// EFBA always uses OnDemandAuction (constant value).
enum class TradingSessionSubId : uint8
{
    Empty                    = 0,
    ScheduledOpeningAuction  = 2,     ///< 'SOAU'.
    ContinuousTrading        = 3,     ///< 'COTR'.
    ScheduledClosingAuction  = 4,     ///< 'SCAU'.
    AtMarketCloseTrading     = 5,     ///< 'MACT'.
    ScheduledIntradayAuction = 6,     ///< 'SIAU'.
    UndefinedAuction         = 8,     ///< 'UDUC'.
    UnscheduledAuction       = 9,     ///< 'UAUC'.
    OutOfMainSessionTrading  = 10,    ///< 'OMST'.
    OnDemandAuction          = 14,    ///< 'ODAU' (FBA).
    Other                    = 99     ///< 'OTSP'.
};

inline constexpr unsigned int TRADING_SESSION_SUB_ID_SIZE = sizeof(TradingSessionSubId);
static_assert(TRADING_SESSION_SUB_ID_SIZE == 1,
              "TradingSessionSubId underlying size mismatch with spec (1 byte)");

// === TradSesStatus — §4.2.13 =================================================
/// TradSesStatus (FIX Tag 340) — status of the trading system.
enum class TradSesStatus : uint8
{
    Open          = 2,    ///< 'ACTV'.
    Outage        = 7,    ///< 'OTAG'.
    PartialOutage = 8     ///< 'POTG'.
};

inline constexpr unsigned int TRAD_SES_STATUS_SIZE = sizeof(TradSesStatus);
static_assert(TRAD_SES_STATUS_SIZE == 1,
              "TradSesStatus underlying size mismatch with spec (1 byte)");

// === TrdRegPublicationType — §4.2.11 =========================================
/// TrdRegPublicationType (FIX Tag 2669) — publication type for the post-trade entry.
enum class TrdRegPublicationType : uint8
{
    PreTradeTransparencyWaiver = 0,    ///< Combined with TrdRegPublicationReason.
    PostTradeDeferral          = 1     ///< 'LRGS'.
};

inline constexpr unsigned int TRD_REG_PUBLICATION_TYPE_SIZE = sizeof(TrdRegPublicationType);
static_assert(TRD_REG_PUBLICATION_TYPE_SIZE == 1,
              "TrdRegPublicationType underlying size mismatch with spec (1 byte)");

// === TrdRegTimestampOrigin — §§4.2.8–4.2.11 ==================================
/// TrdRegTimestampOrigin (FIX Tag 771) — origin of the regulatory timestamp.
enum class TrdRegTimestampOrigin : char
{
    Contributor = 'C',    ///< Trading venue.
    Publisher   = 'P'
};

inline constexpr unsigned int TRD_REG_TIMESTAMP_ORIGIN_SIZE = sizeof(TrdRegTimestampOrigin);
static_assert(TRD_REG_TIMESTAMP_ORIGIN_SIZE == 1,
              "TrdRegTimestampOrigin underlying size mismatch with spec (1 byte)");

// === TrdRegTimestampType — §§4.2.8–4.2.11 ====================================
/// TrdRegTimestampType (FIX Tag 770) — semantic of a regulatory timestamp.
/// Each occurrence is paired with a TrdRegTimestampOrigin.
enum class TrdRegTimestampType : uint8
{
    ExecutionTime               = 1,     ///< Trading date and time.
    TimeIn                      = 2,     ///< Reception date and time.
    PubliclyReported            = 11,    ///< Dissemination/Publication date and time.
    ReferenceTimeForBboOrAuction = 34,   ///< Reference time for NBBO / EBBO / EBAP / EFBA.
    UpdateTime                  = 36     ///< Entry / Indicative date and time.
};

inline constexpr unsigned int TRD_REG_TIMESTAMP_TYPE_SIZE = sizeof(TrdRegTimestampType);
static_assert(TRD_REG_TIMESTAMP_TYPE_SIZE == 1,
              "TrdRegTimestampType underlying size mismatch with spec (1 byte)");

// === VenueType — §§4.2.12, 4.2.13 ============================================
/// VenueType (FIX Tag 1430) — type of trading system on which the instrument /
/// system status is provided.
enum class VenueType : char
{
    CentralLimitOrderBook = 'B',    ///< CLOB.
    QuoteDrivenMarket     = 'Q',    ///< QDTS.
    AuctionDrivenMarket   = 'A',    ///< PATS.
    QuoteNegotiation      = 'N',    ///< RFQT.
    HybridMarket          = 'H',    ///< HYBR.
    OtherMarket           = 'z'     ///< OTHR (lower-case as per spec).
};

inline constexpr unsigned int VENUE_TYPE_SIZE = sizeof(VenueType);
static_assert(VENUE_TYPE_SIZE == 1,
              "VenueType underlying size mismatch with spec (1 byte)");

} // namespace protocol::messages::types
