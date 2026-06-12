// SPDX-License-Identifier: TODO
// Auto-generated example — illustrates the house style produced by
// /pdf-to-cpp-messages. Replace this with messages from your real PDF.
//
// Source : sample_spec.pdf — section §5.2 (PostTrade)
// Spec version: 1.0   Generated: 2026-06-12

#pragma once

#include <ostream>
#include <string>
#include <type_traits>

#include "messages/Common.hpp"
#include "messages/MessageHeader.hpp"
#include "messages/MessageIds.hpp"

namespace protocol::messages {

#pragma pack(push, 1)

/// Post-trade publication message.
/// @see sample_spec.pdf §5.2
struct PostTradeStruct
{
    uint8   m_MDUpdateAction;
    int64   m_MsgSeqNum;

    char    m_SecurityID[12];
    char    m_SecurityExchange[4];
    char    m_LastMkt[4];

    Decimal m_MDEntryPx;
    char    m_Currency[3];
    Decimal m_MDEntrySize;
    uint8   m_MDOriginType;
    uint8   m_AlgorithmicTradeIndicator;
    uint8   m_MDQualityIndicator;
    char    m_RegulatoryTradeID[52];
    uint8   m_TradeType;
    uint8   m_TradePriceCondition;
    uint8   m_TrdRegPublicationReason;
    char    m_MmtCode[14];
    char    m_TradeCondition;

    TimestampNano m_TrdRegTimestamp_0;
    TimestampNano m_TrdRegTimestamp_1;
    TimestampNano m_TrdRegTimestamp_2;
    TimestampNano m_TrdRegTimestamp_3;
    TimestampNano m_TrdRegTimestamp_4;

    int16 m_NoPartyIDs;
    char  m_PartyID[4];
    uint8 m_PartyRole;
};

#pragma pack(pop)

static_assert(std::is_standard_layout<PostTradeStruct>::value,
              "PostTradeStruct must be standard-layout");
static_assert(std::is_trivially_copyable<PostTradeStruct>::value,
              "PostTradeStruct must be trivially copyable");

class PostTrade
{
    MessageHeader   const   m_MessageHeader;
    PostTradeStruct const & m_Struct;
public:
    static constexpr const uint16  TYPE = MSG_ID_POST_TRADE;
    static constexpr const char*   NAME = "PostTrade";
    static const unsigned int      SIZE = MessageHeader::SIZE + sizeof(PostTradeStruct);

    explicit PostTrade(InputData const & data)
    : m_MessageHeader(data)
    , m_Struct(*reinterpret_cast<const PostTradeStruct*>(data.data() + MessageHeader::SIZE))
    {
    }

    uint8        getMDUpdateAction()           const { return m_Struct.m_MDUpdateAction; }
    int64        getMsgSeqNum()                const { return m_Struct.m_MsgSeqNum; }
    std::string  getSecurityID()               const { return std::string(m_Struct.m_SecurityID, 12); }
    std::string  getSecurityExchange()         const { return std::string(m_Struct.m_SecurityExchange, 4); }
    std::string  getLastMkt()                  const { return std::string(m_Struct.m_LastMkt, 4); }
    Decimal      getMDEntryPx()                const { return m_Struct.m_MDEntryPx; }
    std::string  getCurrency()                 const { return std::string(m_Struct.m_Currency, 3); }
    Decimal      getMDEntrySize()              const { return m_Struct.m_MDEntrySize; }
    uint8        getMDOriginType()             const { return m_Struct.m_MDOriginType; }
    uint8        getAlgorithmicTradeIndicator()const { return m_Struct.m_AlgorithmicTradeIndicator; }
    uint8        getMDQualityIndicator()       const { return m_Struct.m_MDQualityIndicator; }
    std::string  getRegulatoryTradeID()        const { return std::string(m_Struct.m_RegulatoryTradeID, 52); }
    uint8        getTradeType()                const { return m_Struct.m_TradeType; }
    uint8        getTradePriceCondition()      const { return m_Struct.m_TradePriceCondition; }
    uint8        getTrdRegPublicationReason()  const { return m_Struct.m_TrdRegPublicationReason; }
    std::string  getMmtCode()                  const { return std::string(m_Struct.m_MmtCode, 14); }
    char         getTradeCondition()           const { return m_Struct.m_TradeCondition; }
    TimestampNano getTrdRegTimestamp_0()       const { return m_Struct.m_TrdRegTimestamp_0; }
    TimestampNano getTrdRegTimestamp_1()       const { return m_Struct.m_TrdRegTimestamp_1; }
    TimestampNano getTrdRegTimestamp_2()       const { return m_Struct.m_TrdRegTimestamp_2; }
    TimestampNano getTrdRegTimestamp_3()       const { return m_Struct.m_TrdRegTimestamp_3; }
    TimestampNano getTrdRegTimestamp_4()       const { return m_Struct.m_TrdRegTimestamp_4; }
    int16        getNoPartyIDs()               const { return m_Struct.m_NoPartyIDs; }
    std::string  getPartyID()                  const { return std::string(m_Struct.m_PartyID, 4); }
    uint8        getPartyRole()                const { return m_Struct.m_PartyRole; }

    virtual void dump(std::ostream & os) const
    {
        m_MessageHeader.dump(os);
        os
        DUMP_FIELD_AS_INT(MDUpdateAction)
        DUMP_FIELD(MsgSeqNum)
        DUMP_FIELD(SecurityID)
        DUMP_FIELD(SecurityExchange)
        DUMP_FIELD(LastMkt)
        DUMP_FIELD(MDEntryPx)
        DUMP_FIELD(Currency)
        DUMP_FIELD(MDEntrySize)
        DUMP_FIELD_AS_INT(MDOriginType)
        DUMP_FIELD_AS_INT(AlgorithmicTradeIndicator)
        DUMP_FIELD_AS_INT(MDQualityIndicator)
        DUMP_FIELD(RegulatoryTradeID)
        DUMP_FIELD_AS_INT(TradeType)
        DUMP_FIELD_AS_INT(TradePriceCondition)
        DUMP_FIELD_AS_INT(TrdRegPublicationReason)
        DUMP_FIELD(MmtCode)
        DUMP_FIELD(TradeCondition)
        DUMP_FIELD(TrdRegTimestamp_0)
        DUMP_FIELD(TrdRegTimestamp_1)
        DUMP_FIELD(TrdRegTimestamp_2)
        DUMP_FIELD(TrdRegTimestamp_3)
        DUMP_FIELD(TrdRegTimestamp_4)
        DUMP_FIELD(NoPartyIDs)
        DUMP_FIELD(PartyID)
        DUMP_FIELD_AS_INT(PartyRole)
        ;
    }
};

inline std::ostream& operator<<(std::ostream& out, const PostTrade& obj)
{
    obj.dump(out);
    return out;
}

} // namespace protocol::messages
