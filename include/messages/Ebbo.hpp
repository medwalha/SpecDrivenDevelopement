// SPDX-License-Identifier: TODO
// Auto-generated from specs/euro_ctp_spec.pdf — §4.2.8 European Best Bid and Offer (EBBO)
// Spec version: 1.3   Generated: 2026-06-12
// TemplateID = 101 (Application, SchemaID=1). Payload = 121 bytes.
//
// FIX semantic: MarketDataSnapshotFullRefresh [W]
// Constants NOT on the wire: Symbol="[N/A]", SecurityIDSource=4, NoMDEntries=2,
//                            NoTrdRegTimestamps=5,
//                            TrdRegTimestampType_0..4 / TrdRegTimestampOrigin_0..4
//                            (always 2/P, 11/P, 11/C, 34/P, 36/C).

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

/// EBBO payload (§4.2.8). Two fixed MDEntries (Bid then Offer) and five
/// TrdRegTimestamps with implied Type / Origin metadata (constants omitted
/// from the wire).
struct EbboStruct
{
    types::SeqNum                  m_MsgSeqNum;              ///< 8B — instrument-level sequence number.
    types::Isin                    m_SecurityID;             ///< 12B — ISO 6166 ISIN.
    types::Mic                     m_SecurityExchange;       ///< 4B — instrument place-of-listing MIC.
    types::Mic                     m_MostLiquidMarketID;     ///< 4B — most relevant market MIC (MRMTL).

    // MDEntry #0 — Bid side ----------------------------------------------------
    types::MdEntryType             m_MDEntryType_0;          ///< 1B — '0' (Bid), 'J' (Empty book) or 'j' (Uncrossing bid).
    types::Decimal                 m_MDEntryPx_0;            ///< 9B — best bid price.
    types::Currency                m_Currency_0;             ///< 3B — ISO 4217 currency.
    types::Decimal                 m_MDEntrySize_0;          ///< 9B — best bid volume.

    // MDEntry #1 — Offer side --------------------------------------------------
    types::MdEntryType             m_MDEntryType_1;          ///< 1B — '1' (Offer), 'J' (Empty book) or 'k' (Uncrossing offer).
    types::Decimal                 m_MDEntryPx_1;            ///< 9B — best offer price.
    types::Currency                m_Currency_1;             ///< 3B — ISO 4217 currency.
    types::Decimal                 m_MDEntrySize_1;          ///< 9B — best offer volume.

    types::QuoteConditionsBitmap   m_QuoteConditions;        ///< 1B — bitmap (0/1/2/4/8).
    types::InternalIdentifier      m_MDEntryID;              ///< 8B — EBBO identifier.

    types::TimestampNano           m_TrdRegTimestamp_0;      ///< 8B — Time in (Reception),       implied Type=2,  Origin=P.
    types::TimestampNano           m_TrdRegTimestamp_1;      ///< 8B — Publicly reported (Diss.), implied Type=11, Origin=P.
    types::TimestampNano           m_TrdRegTimestamp_2;      ///< 8B — Publicly reported (Pub.),  implied Type=11, Origin=C.
    types::TimestampNano           m_TrdRegTimestamp_3;      ///< 8B — Reference time for NBBO,   implied Type=34, Origin=P.
    types::TimestampNano           m_TrdRegTimestamp_4;      ///< 8B — Update time,               implied Type=36, Origin=C.
};

#pragma pack(pop)

static_assert(sizeof(EbboStruct) == 121,
              "EbboStruct size mismatch with spec (121 bytes)");
static_assert(std::is_standard_layout<EbboStruct>::value,
              "EbboStruct must be standard-layout");
static_assert(std::is_trivially_copyable<EbboStruct>::value,
              "EbboStruct must be trivially copyable");

/// European Best Bid and Offer (EBBO) application message (§4.2.8).
class Ebbo
{
    MessageHeader   const   m_MessageHeader;
    EbboStruct      const & m_Struct;

public:
    static constexpr unsigned int TYPE = MSG_ID_EBBO;
    static constexpr unsigned int SIZE = MessageHeader::SIZE + sizeof(EbboStruct);

    /// @param data Raw byte buffer that MUST start at the FramingHeader and
    ///             remain alive for the lifetime of this Ebbo instance.
    explicit Ebbo(InputData const & data)
    : m_MessageHeader(data)
    , m_Struct(*reinterpret_cast<const EbboStruct*>(data.data() + MessageHeader::SIZE))
    {
    }

    MessageHeader const & getMessageHeader() const { return m_MessageHeader; }

    types::SeqNum                 getMsgSeqNum()           const { return m_Struct.m_MsgSeqNum; }
    std::string                   getSecurityID()          const { return std::string(m_Struct.m_SecurityID,          types::ISIN_SIZE); }
    std::string                   getSecurityExchange()    const { return std::string(m_Struct.m_SecurityExchange,    types::MIC_SIZE); }
    std::string                   getMostLiquidMarketID()  const { return std::string(m_Struct.m_MostLiquidMarketID,  types::MIC_SIZE); }

    types::MdEntryType            getMDEntryType_0()       const { return m_Struct.m_MDEntryType_0; }
    types::Decimal                getMDEntryPx_0()         const { return m_Struct.m_MDEntryPx_0; }
    std::string                   getCurrency_0()          const { return std::string(m_Struct.m_Currency_0, types::CURRENCY_SIZE); }
    types::Decimal                getMDEntrySize_0()       const { return m_Struct.m_MDEntrySize_0; }

    types::MdEntryType            getMDEntryType_1()       const { return m_Struct.m_MDEntryType_1; }
    types::Decimal                getMDEntryPx_1()         const { return m_Struct.m_MDEntryPx_1; }
    std::string                   getCurrency_1()          const { return std::string(m_Struct.m_Currency_1, types::CURRENCY_SIZE); }
    types::Decimal                getMDEntrySize_1()       const { return m_Struct.m_MDEntrySize_1; }

    types::QuoteConditionsBitmap  getQuoteConditions()     const { return m_Struct.m_QuoteConditions; }
    types::InternalIdentifier     getMDEntryID()           const { return m_Struct.m_MDEntryID; }
    types::TimestampNano          getTrdRegTimestamp_0()   const { return m_Struct.m_TrdRegTimestamp_0; }
    types::TimestampNano          getTrdRegTimestamp_1()   const { return m_Struct.m_TrdRegTimestamp_1; }
    types::TimestampNano          getTrdRegTimestamp_2()   const { return m_Struct.m_TrdRegTimestamp_2; }
    types::TimestampNano          getTrdRegTimestamp_3()   const { return m_Struct.m_TrdRegTimestamp_3; }
    types::TimestampNano          getTrdRegTimestamp_4()   const { return m_Struct.m_TrdRegTimestamp_4; }

    void dump(std::ostream & os) const
    {
        os << "Ebbo:\n";
        m_MessageHeader.dump(os);
        os
        DUMP_FIELD(MsgSeqNum)
        DUMP_FIELD(SecurityID)
        DUMP_FIELD(SecurityExchange)
        DUMP_FIELD(MostLiquidMarketID)
        ;
        os << "  MDEntryType_0=" << static_cast<char>(m_Struct.m_MDEntryType_0) << "\n";
        os
        DUMP_FIELD(MDEntryPx_0)
        DUMP_FIELD(Currency_0)
        DUMP_FIELD(MDEntrySize_0)
        ;
        os << "  MDEntryType_1=" << static_cast<char>(m_Struct.m_MDEntryType_1) << "\n";
        os
        DUMP_FIELD(MDEntryPx_1)
        DUMP_FIELD(Currency_1)
        DUMP_FIELD(MDEntrySize_1)
        DUMP_FIELD_AS_INT(QuoteConditions)
        DUMP_FIELD(MDEntryID)
        DUMP_FIELD(TrdRegTimestamp_0)
        DUMP_FIELD(TrdRegTimestamp_1)
        DUMP_FIELD(TrdRegTimestamp_2)
        DUMP_FIELD(TrdRegTimestamp_3)
        DUMP_FIELD(TrdRegTimestamp_4)
        ;
    }
};

inline std::ostream& operator<<(std::ostream& out, Ebbo const& obj)
{
    obj.dump(out);
    return out;
}

} // namespace protocol::messages
