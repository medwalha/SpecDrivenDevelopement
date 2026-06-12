// SPDX-License-Identifier: TODO
// Auto-generated from specs/euro_ctp_spec.pdf — §4.2.10 European Frequent Batch Auction (EFBA)
// Spec version: 1.3   Generated: 2026-06-12
// TemplateID = 103 (Application, SchemaID=1). Payload = 117 bytes.
//
// FIX semantic: MarketDataSnapshotFullRefresh [W]
// Constants NOT on the wire: Symbol="[N/A]", SecurityIDSource=4, NoMDEntries=1,
//                            TradingSessionID=1, TradingSessionSubID=14 (ODAU),
//                            NoTrdRegTimestamps=5,
//                            TrdRegTimestampType_0..4 / TrdRegTimestampOrigin_0..4
//                            (always 2/P, 11/P, 11/C, 34/P, 36/C).
//
// NB: EFBA shares the exact same wire layout as EBAP (§4.2.9), but with the
// constraint that the auction is always a Frequent Batch Auction (ODAU).

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

/// EFBA payload (§4.2.10). Layout identical to EbapStruct.
struct EfbaStruct
{
    types::SeqNum                  m_MsgSeqNum;              ///< 8B — instrument-level sequence number.
    types::Isin                    m_SecurityID;             ///< 12B — ISO 6166 ISIN.
    types::Mic                     m_SecurityExchange;       ///< 4B — instrument place-of-listing MIC.
    types::Mic                     m_MostLiquidMarketID;     ///< 4B — most relevant market MIC (MRMTL).
    types::MdEntryType             m_MDEntryType;            ///< 1B — 'Q' (Auction clearing price) or 'J' (Empty book).
    types::Decimal                 m_MDEntryPx;              ///< 9B — volume-weighted auction price.
    types::Currency                m_Currency;               ///< 3B — ISO 4217 price currency.
    types::Decimal                 m_MDEntrySize;            ///< 9B — auction volume.
    types::Decimal                 m_LowPx;                  ///< 9B — lowest auction price.
    types::Decimal                 m_HighPx;                 ///< 9B — highest auction price.
    types::QuoteConditionsBitmap   m_QuoteConditions;        ///< 1B — bitmap (0=none, 2=Out of sequence).
    types::InternalIdentifier      m_MDEntryID;              ///< 8B — EFBA identifier.
    types::TimestampNano           m_TrdRegTimestamp_0;      ///< 8B — Time in (Reception),       implied Type=2,  Origin=P.
    types::TimestampNano           m_TrdRegTimestamp_1;      ///< 8B — Publicly reported (Diss.), implied Type=11, Origin=P.
    types::TimestampNano           m_TrdRegTimestamp_2;      ///< 8B — Publicly reported (Pub.),  implied Type=11, Origin=C.
    types::TimestampNano           m_TrdRegTimestamp_3;      ///< 8B — Reference time for NBBO,   implied Type=34, Origin=P.
    types::TimestampNano           m_TrdRegTimestamp_4;      ///< 8B — Update time (Indicative),  implied Type=36, Origin=C.
};

#pragma pack(pop)

static_assert(sizeof(EfbaStruct) == 117,
              "EfbaStruct size mismatch with spec (117 bytes)");
static_assert(std::is_standard_layout<EfbaStruct>::value,
              "EfbaStruct must be standard-layout");
static_assert(std::is_trivially_copyable<EfbaStruct>::value,
              "EfbaStruct must be trivially copyable");

/// European Frequent Batch Auction (EFBA) application message (§4.2.10).
class Efba
{
    MessageHeader   const   m_MessageHeader;
    EfbaStruct      const & m_Struct;

public:
    static constexpr unsigned int TYPE = MSG_ID_EFBA;
    static constexpr unsigned int SIZE = MessageHeader::SIZE + sizeof(EfbaStruct);

    /// @param data Raw byte buffer that MUST start at the FramingHeader and
    ///             remain alive for the lifetime of this Efba instance.
    explicit Efba(InputData const & data)
    : m_MessageHeader(data)
    , m_Struct(*reinterpret_cast<const EfbaStruct*>(data.data() + MessageHeader::SIZE))
    {
    }

    MessageHeader const & getMessageHeader() const { return m_MessageHeader; }

    types::SeqNum                 getMsgSeqNum()           const { return m_Struct.m_MsgSeqNum; }
    std::string                   getSecurityID()          const { return std::string(m_Struct.m_SecurityID,          types::ISIN_SIZE); }
    std::string                   getSecurityExchange()    const { return std::string(m_Struct.m_SecurityExchange,    types::MIC_SIZE); }
    std::string                   getMostLiquidMarketID()  const { return std::string(m_Struct.m_MostLiquidMarketID,  types::MIC_SIZE); }
    types::MdEntryType            getMDEntryType()         const { return m_Struct.m_MDEntryType; }
    types::Decimal                getMDEntryPx()           const { return m_Struct.m_MDEntryPx; }
    std::string                   getCurrency()            const { return std::string(m_Struct.m_Currency,            types::CURRENCY_SIZE); }
    types::Decimal                getMDEntrySize()         const { return m_Struct.m_MDEntrySize; }
    types::Decimal                getLowPx()               const { return m_Struct.m_LowPx; }
    types::Decimal                getHighPx()              const { return m_Struct.m_HighPx; }
    types::QuoteConditionsBitmap  getQuoteConditions()     const { return m_Struct.m_QuoteConditions; }
    types::InternalIdentifier     getMDEntryID()           const { return m_Struct.m_MDEntryID; }
    types::TimestampNano          getTrdRegTimestamp_0()   const { return m_Struct.m_TrdRegTimestamp_0; }
    types::TimestampNano          getTrdRegTimestamp_1()   const { return m_Struct.m_TrdRegTimestamp_1; }
    types::TimestampNano          getTrdRegTimestamp_2()   const { return m_Struct.m_TrdRegTimestamp_2; }
    types::TimestampNano          getTrdRegTimestamp_3()   const { return m_Struct.m_TrdRegTimestamp_3; }
    types::TimestampNano          getTrdRegTimestamp_4()   const { return m_Struct.m_TrdRegTimestamp_4; }

    void dump(std::ostream & os) const
    {
        os << "Efba:\n";
        m_MessageHeader.dump(os);
        os
        DUMP_FIELD(MsgSeqNum)
        DUMP_FIELD(SecurityID)
        DUMP_FIELD(SecurityExchange)
        DUMP_FIELD(MostLiquidMarketID)
        ;
        os << "  MDEntryType=" << static_cast<char>(m_Struct.m_MDEntryType) << "\n";
        os
        DUMP_FIELD(MDEntryPx)
        DUMP_FIELD(Currency)
        DUMP_FIELD(MDEntrySize)
        DUMP_FIELD(LowPx)
        DUMP_FIELD(HighPx)
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

inline std::ostream& operator<<(std::ostream& out, Efba const& obj)
{
    obj.dump(out);
    return out;
}

} // namespace protocol::messages
