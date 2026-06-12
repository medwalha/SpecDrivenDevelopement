// SPDX-License-Identifier: TODO
// Auto-generated from specs/euro_ctp_spec.pdf — §4.2.11 Post-trade
// Spec version: 1.3   Generated: 2026-06-12
// TemplateID = 201 (Application, SchemaID=1). Payload = 170 bytes (with 1 Parties entry).
//
// FIX semantic: MarketDataIncrementalRefresh [X]
//
// Constants NOT on the wire (per the SBE table):
//   NoMDEntries=1, MDEntryType=2 (Trade), Symbol="[N/A]",
//   SecurityIDSource=4 (ISIN), NoRegulatoryTradeIDs=1,
//   RegulatoryTradeIDType=5 (TVTIC), NoTradeTypes=0 (encoded as bitmap),
//   NoTradePriceConditions=0 (bitmap), NoTrdRegPublications=0 (bitmap),
//   TrdRegPublicationType=0 (except for 'LRGS' where it is 1),
//   NoTrdRegTimestamps=5,
//   TrdRegTimestampType_0..4 / TrdRegTimestampOrigin_0..4
//     (always 1/C, 2/P, 2/C, 11/P, 11/C),
//   PartyIDSource=G (MIC).
//
// ⚠️ Special handling notes:
//   • MDOriginType is ALWAYS present on the wire (1 byte) but the spec
//     mandates the application IGNORE it when LastMkt is 'SINT' or 'XOFF'.
//   • <Parties> is a variable-size repeating group prefixed by a
//     RptGrpHeader. This struct hard-codes a single entry to keep the type
//     trivially copyable. TODO: introduce a dynamic-view class to handle
//     N>1 parties (e.g. when both PartyRole=62 and 73 are present).

#pragma once

#include <ostream>
#include <string>
#include <type_traits>

#include "messages/Common.hpp"
#include "messages/MessageHeader.hpp"
#include "messages/MessageIds.hpp"
#include "messages/types/Types.hpp"

namespace protocol::messages {

#pragma pack(push, 1)

/// PostTrade Parties group — single entry layout (PartyID + PartyRole).
/// PartyIDSource is constant ('G' MIC) and therefore omitted from the wire.
struct PostTradePartyEntry
{
    types::Mic         m_PartyID;        ///< 4B — venue MIC.
    types::PartyRole   m_PartyRole;      ///< 1B — 62 (Report originator) or 73 (Execution venue).
};

static_assert(sizeof(PostTradePartyEntry) == 5,
              "PostTradePartyEntry size mismatch (5 bytes = 4+1)");

/// PostTrade payload (§4.2.11). Single Parties entry (TODO: variable).
struct PostTradeStruct
{
    types::MdUpdateAction              m_MDUpdateAction;            ///< 1B — 0/1/2 (New/AMND/CANC).
    types::SeqNum                      m_MsgSeqNum;                 ///< 8B — instrument-level sequence number.
    types::Isin                        m_SecurityID;                ///< 12B — ISO 6166 ISIN.
    types::Mic                         m_SecurityExchange;          ///< 4B — place-of-listing MIC.
    types::Mic                         m_LastMkt;                   ///< 4B — venue-of-execution MIC.
    types::Decimal                     m_MDEntryPx;                 ///< 9B — traded price (NULL when PNDG/NOAP).
    types::Currency                    m_Currency;                  ///< 3B — ISO 4217 price currency.
    types::Decimal                     m_MDEntrySize;               ///< 9B — quantity.
    types::MdOriginType                m_MDOriginType;              ///< 1B — trading system type (ignore when LastMkt='SINT'/'XOFF').
    types::AlgorithmicTradeIndicator   m_AlgorithmicTradeIndicator; ///< 1B — 0/1 (Non-algo/ALGO).
    types::MdQualityIndicatorBitmap    m_MDQualityIndicator;        ///< 1B — suspicious trade flag bitmap.
    types::TransactionIdentifier       m_RegulatoryTradeID;         ///< 52B — TVTIC code (alphanumeric, up to 52 chars).
    types::TradeTypeBitmap             m_TradeType;                 ///< 1B — bitmap (Portfolio/Benchmark/Package).
    types::TradePriceConditionBitmap   m_TradePriceCondition;       ///< 1B — bitmap (SDIV/NPFT/PNDG/NOAP).
    types::TrdRegPublicationReasonBitmap m_TrdRegPublicationReason; ///< 1B — bitmap (NLIQ/OILQ/PRIC/RFPT/LRGS).
    types::MmtFlag                     m_MMTCode;                   ///< 14B — MMT efficient encoding flag string.
    types::TradeCondition              m_TradeCondition;            ///< 1B — char enum ('0' or 'k').
    types::TimestampNano               m_TrdRegTimestamp_0;         ///< 8B — Execution time,         implied Type=1,  Origin=C.
    types::TimestampNano               m_TrdRegTimestamp_1;         ///< 8B — Time in (CTP),          implied Type=2,  Origin=P.
    types::TimestampNano               m_TrdRegTimestamp_2;         ///< 8B — Time in (APA),          implied Type=2,  Origin=C.
    types::TimestampNano               m_TrdRegTimestamp_3;         ///< 8B — Publication by CTP,     implied Type=11, Origin=P.
    types::TimestampNano               m_TrdRegTimestamp_4;         ///< 8B — Publication by Contrib, implied Type=11, Origin=C.
    types::RptGrpHeader                m_NoPartyIDs;                ///< 2B — SBE group header (BlockLength + NumInGroup).
    PostTradePartyEntry                m_Parties[1];                ///< 5B — TODO: support N>1 entries via dynamic view.
};

#pragma pack(pop)

static_assert(sizeof(PostTradeStruct) == 170,
              "PostTradeStruct size mismatch with spec (170 bytes with 1 Parties entry)");
static_assert(std::is_standard_layout<PostTradeStruct>::value,
              "PostTradeStruct must be standard-layout");
static_assert(std::is_trivially_copyable<PostTradeStruct>::value,
              "PostTradeStruct must be trivially copyable");

/// PostTrade application message — disseminates a single executed trade
/// (§4.2.11). Uses the FIX MarketDataIncrementalRefresh [X] template.
class PostTrade
{
    MessageHeader        const   m_MessageHeader;
    PostTradeStruct      const & m_Struct;

public:
    static constexpr unsigned int TYPE = MSG_ID_POST_TRADE;
    static constexpr unsigned int SIZE = MessageHeader::SIZE + sizeof(PostTradeStruct);

    /// @param data Raw byte buffer that MUST start at the FramingHeader and
    ///             remain alive for the lifetime of this PostTrade instance.
    explicit PostTrade(InputData const & data)
    : m_MessageHeader(data)
    , m_Struct(*reinterpret_cast<const PostTradeStruct*>(data.data() + MessageHeader::SIZE))
    {
    }

    MessageHeader const & getMessageHeader() const { return m_MessageHeader; }

    types::MdUpdateAction               getMDUpdateAction()            const { return m_Struct.m_MDUpdateAction; }
    types::SeqNum                       getMsgSeqNum()                 const { return m_Struct.m_MsgSeqNum; }
    std::string                         getSecurityID()                const { return std::string(m_Struct.m_SecurityID,        types::ISIN_SIZE); }
    std::string                         getSecurityExchange()          const { return std::string(m_Struct.m_SecurityExchange,  types::MIC_SIZE); }
    std::string                         getLastMkt()                   const { return std::string(m_Struct.m_LastMkt,           types::MIC_SIZE); }
    types::Decimal                      getMDEntryPx()                 const { return m_Struct.m_MDEntryPx; }
    std::string                         getCurrency()                  const { return std::string(m_Struct.m_Currency,          types::CURRENCY_SIZE); }
    types::Decimal                      getMDEntrySize()               const { return m_Struct.m_MDEntrySize; }
    types::MdOriginType                 getMDOriginType()              const { return m_Struct.m_MDOriginType; }
    types::AlgorithmicTradeIndicator    getAlgorithmicTradeIndicator() const { return m_Struct.m_AlgorithmicTradeIndicator; }
    types::MdQualityIndicatorBitmap     getMDQualityIndicator()        const { return m_Struct.m_MDQualityIndicator; }
    std::string                         getRegulatoryTradeID()         const { return std::string(m_Struct.m_RegulatoryTradeID, types::TRANSACTION_IDENTIFIER_SIZE); }
    types::TradeTypeBitmap              getTradeType()                 const { return m_Struct.m_TradeType; }
    types::TradePriceConditionBitmap    getTradePriceCondition()       const { return m_Struct.m_TradePriceCondition; }
    types::TrdRegPublicationReasonBitmap getTrdRegPublicationReason()  const { return m_Struct.m_TrdRegPublicationReason; }
    std::string                         getMMTCode()                   const { return std::string(m_Struct.m_MMTCode,           types::MMT_FLAG_SIZE); }
    types::TradeCondition               getTradeCondition()            const { return m_Struct.m_TradeCondition; }
    types::TimestampNano                getTrdRegTimestamp_0()         const { return m_Struct.m_TrdRegTimestamp_0; }
    types::TimestampNano                getTrdRegTimestamp_1()         const { return m_Struct.m_TrdRegTimestamp_1; }
    types::TimestampNano                getTrdRegTimestamp_2()         const { return m_Struct.m_TrdRegTimestamp_2; }
    types::TimestampNano                getTrdRegTimestamp_3()         const { return m_Struct.m_TrdRegTimestamp_3; }
    types::TimestampNano                getTrdRegTimestamp_4()         const { return m_Struct.m_TrdRegTimestamp_4; }

    types::RptGrpHeader const & getNoPartyIDs()        const { return m_Struct.m_NoPartyIDs; }
    PostTradePartyEntry const & getParty(uint8 idx)    const { return m_Struct.m_Parties[idx]; }
    std::string                 getPartyID(uint8 idx)  const { return std::string(m_Struct.m_Parties[idx].m_PartyID, types::MIC_SIZE); }
    types::PartyRole            getPartyRole(uint8 idx) const { return m_Struct.m_Parties[idx].m_PartyRole; }

    void dump(std::ostream & os) const
    {
        os << "PostTrade:\n";
        m_MessageHeader.dump(os);
        os << "  MDUpdateAction="            << static_cast<int>(m_Struct.m_MDUpdateAction)            << "\n";
        os
        DUMP_FIELD(MsgSeqNum)
        DUMP_FIELD(SecurityID)
        DUMP_FIELD(SecurityExchange)
        DUMP_FIELD(LastMkt)
        DUMP_FIELD(MDEntryPx)
        DUMP_FIELD(Currency)
        DUMP_FIELD(MDEntrySize)
        ;
        os << "  MDOriginType="              << static_cast<int>(m_Struct.m_MDOriginType)              << "\n";
        os << "  AlgorithmicTradeIndicator=" << static_cast<int>(m_Struct.m_AlgorithmicTradeIndicator) << "\n";
        os
        DUMP_FIELD_AS_INT(MDQualityIndicator)
        DUMP_FIELD(RegulatoryTradeID)
        DUMP_FIELD_AS_INT(TradeType)
        DUMP_FIELD_AS_INT(TradePriceCondition)
        DUMP_FIELD_AS_INT(TrdRegPublicationReason)
        DUMP_FIELD(MMTCode)
        ;
        os << "  TradeCondition="            << static_cast<char>(m_Struct.m_TradeCondition)           << "\n";
        os
        DUMP_FIELD(TrdRegTimestamp_0)
        DUMP_FIELD(TrdRegTimestamp_1)
        DUMP_FIELD(TrdRegTimestamp_2)
        DUMP_FIELD(TrdRegTimestamp_3)
        DUMP_FIELD(TrdRegTimestamp_4)
        ;
        os << "  NoPartyIDs.m_EntryLength="  << static_cast<int>(m_Struct.m_NoPartyIDs.m_EntryLength)
           << " m_NumOfEntries="            << static_cast<int>(m_Struct.m_NoPartyIDs.m_NumOfEntries) << "\n";
        os << "  Party[0].PartyID="          << getPartyID(0)
           << " PartyRole="                  << static_cast<int>(getPartyRole(0)) << "\n";
    }
};

inline std::ostream& operator<<(std::ostream& out, PostTrade const& obj)
{
    obj.dump(out);
    return out;
}

} // namespace protocol::messages
