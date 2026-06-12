// SPDX-License-Identifier: TODO
// Auto-generated from specs/euro_ctp_spec.pdf — §4.2.12 Instrument status
// Spec version: 1.3   Generated: 2026-06-12
// TemplateID = 301 (Application, SchemaID=1). Payload = 61 bytes.
//
// FIX semantic: SecurityStatus [f]
// Constant fields (Symbol, SecurityIDSource=4, TradingSessionID=1) are NOT
// part of the wire payload per the SBE encoding table.

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

/// InstrumentStatus payload (§4.2.12).
struct InstrumentStatusStruct
{
    types::SeqNum                       m_MsgSeqNum;                 ///< 8B — instrument-level sequence number.
    types::Isin                         m_SecurityID;                ///< 12B — ISO 6166 ISIN.
    types::SecurityStatus               m_SecurityStatus;            ///< 1B — 0/1/5/9 (ACTV/RMOV/SUSP).
    types::Currency                     m_Currency;                  ///< 3B — ISO 4217 currency code.
    types::Mic                          m_MarketID;                  ///< 4B — trading venue MIC.
    types::Mic                          m_SecurityExchange;          ///< 4B — place-of-listing MIC.
    types::MostLiquidMarketIndicator    m_MostLiquidMarketIndicator; ///< 1B — 0/1 (FALSE/TRUE).
    types::VenueType                    m_VenueType;                 ///< 1B — char enum (B/Q/A/N/H/z).
    types::TradingSessionSubId          m_TradingSessionSubID;       ///< 1B — trading-system phase.
    types::MatchType                    m_MatchType;                 ///< 1B — trading-system phase (mutually exclusive w/ TradingSessionSubID).
    types::SecurityTradingStatus        m_SecurityTradingStatus;     ///< 1B — 0/2/3 (HALT/Resume).
    types::TimestampNano                m_TransactTime;              ///< 8B — dissemination time.
    types::TimestampNano                m_EffectiveTime;             ///< 8B — instrument status start time.
    types::InternalIdentifier           m_MDEntryID;                 ///< 8B — instrument status identifier.
};

#pragma pack(pop)

static_assert(sizeof(InstrumentStatusStruct) == 61,
              "InstrumentStatusStruct size mismatch with spec (61 bytes)");
static_assert(std::is_standard_layout<InstrumentStatusStruct>::value,
              "InstrumentStatusStruct must be standard-layout");
static_assert(std::is_trivially_copyable<InstrumentStatusStruct>::value,
              "InstrumentStatusStruct must be trivially copyable");

/// InstrumentStatus application message — disseminates the regulatory /
/// trading status of a single instrument (§4.2.12).
class InstrumentStatus
{
    MessageHeader              const   m_MessageHeader;
    InstrumentStatusStruct     const & m_Struct;

public:
    static constexpr unsigned int TYPE = MSG_ID_INSTRUMENT_STATUS;
    static constexpr unsigned int SIZE = MessageHeader::SIZE + sizeof(InstrumentStatusStruct);

    /// @param data Raw byte buffer that MUST start at the FramingHeader and
    ///             remain alive for the lifetime of this instance.
    explicit InstrumentStatus(InputData const & data)
    : m_MessageHeader(data)
    , m_Struct(*reinterpret_cast<const InstrumentStatusStruct*>(data.data() + MessageHeader::SIZE))
    {
    }

    MessageHeader const & getMessageHeader() const { return m_MessageHeader; }

    types::SeqNum                       getMsgSeqNum()                 const { return m_Struct.m_MsgSeqNum; }
    std::string                         getSecurityID()                const { return std::string(m_Struct.m_SecurityID,        types::ISIN_SIZE); }
    types::SecurityStatus               getSecurityStatus()            const { return m_Struct.m_SecurityStatus; }
    std::string                         getCurrency()                  const { return std::string(m_Struct.m_Currency,          types::CURRENCY_SIZE); }
    std::string                         getMarketID()                  const { return std::string(m_Struct.m_MarketID,          types::MIC_SIZE); }
    std::string                         getSecurityExchange()          const { return std::string(m_Struct.m_SecurityExchange,  types::MIC_SIZE); }
    types::MostLiquidMarketIndicator    getMostLiquidMarketIndicator() const { return m_Struct.m_MostLiquidMarketIndicator; }
    types::VenueType                    getVenueType()                 const { return m_Struct.m_VenueType; }
    types::TradingSessionSubId          getTradingSessionSubID()       const { return m_Struct.m_TradingSessionSubID; }
    types::MatchType                    getMatchType()                 const { return m_Struct.m_MatchType; }
    types::SecurityTradingStatus        getSecurityTradingStatus()     const { return m_Struct.m_SecurityTradingStatus; }
    types::TimestampNano                getTransactTime()              const { return m_Struct.m_TransactTime; }
    types::TimestampNano                getEffectiveTime()             const { return m_Struct.m_EffectiveTime; }
    types::InternalIdentifier           getMDEntryID()                 const { return m_Struct.m_MDEntryID; }

    void dump(std::ostream & os) const
    {
        os << "InstrumentStatus:\n";
        m_MessageHeader.dump(os);
        os
        DUMP_FIELD(MsgSeqNum)
        DUMP_FIELD(SecurityID)
        ;
        os << "  SecurityStatus="            << static_cast<int>(m_Struct.m_SecurityStatus)            << "\n";
        os
        DUMP_FIELD(Currency)
        DUMP_FIELD(MarketID)
        DUMP_FIELD(SecurityExchange)
        ;
        os << "  MostLiquidMarketIndicator=" << static_cast<int>(m_Struct.m_MostLiquidMarketIndicator) << "\n";
        os << "  VenueType="                 << static_cast<char>(m_Struct.m_VenueType)                << "\n";
        os << "  TradingSessionSubID="       << static_cast<int>(m_Struct.m_TradingSessionSubID)       << "\n";
        os << "  MatchType="                 << static_cast<int>(m_Struct.m_MatchType)                 << "\n";
        os << "  SecurityTradingStatus="     << static_cast<int>(m_Struct.m_SecurityTradingStatus)     << "\n";
        os
        DUMP_FIELD(TransactTime)
        DUMP_FIELD(EffectiveTime)
        DUMP_FIELD(MDEntryID)
        ;
    }
};

inline std::ostream& operator<<(std::ostream& out, InstrumentStatus const& obj)
{
    obj.dump(out);
    return out;
}

} // namespace protocol::messages
