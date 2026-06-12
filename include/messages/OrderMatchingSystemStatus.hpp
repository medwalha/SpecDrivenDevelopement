// SPDX-License-Identifier: TODO
// Auto-generated from specs/euro_ctp_spec.pdf — §4.2.13 Order matching system status
// Spec version: 1.3   Generated: 2026-06-12
// TemplateID = 302 (Application, SchemaID=1). Payload = 38 bytes.
//
// FIX semantic: TradingSessionStatus [h]

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

/// OrderMatchingSystemStatus payload (§4.2.13).
/// ⚠️ NOTE on m_MarketID : the spec table contradicts itself — column "SBE
/// data type" reads "uint8" while the "#bytes" column reads "4" and the
/// Format column requires an ISO 10383 MIC code (4 ASCII letters). The wire
/// reality is 4 bytes, so we type the field as `types::Mic` (char[4]).
struct OrderMatchingSystemStatusStruct
{
    types::SeqNum               m_MsgSeqNum;          ///< 8B — instrument-level sequence number.
    types::Mic                  m_MarketID;           ///< 4B — trading venue MIC (see note above).
    types::VenueType            m_VenueType;          ///< 1B — char enum (B/Q/A/N/H/z).
    types::TradSesStatus        m_TradSesStatus;      ///< 1B — uint8 enum (2/7/8).
    types::TimestampNano        m_TransactTime;       ///< 8B — dissemination time (ns since Epoch).
    types::TimestampNano        m_EffectiveTime;      ///< 8B — system status start time (ns since Epoch).
    types::InternalIdentifier   m_MDEntryID;          ///< 8B — instrument status identifier.
};

#pragma pack(pop)

static_assert(sizeof(OrderMatchingSystemStatusStruct) == 38,
              "OrderMatchingSystemStatusStruct size mismatch with spec (38 bytes)");
static_assert(std::is_standard_layout<OrderMatchingSystemStatusStruct>::value,
              "OrderMatchingSystemStatusStruct must be standard-layout");
static_assert(std::is_trivially_copyable<OrderMatchingSystemStatusStruct>::value,
              "OrderMatchingSystemStatusStruct must be trivially copyable");

/// OrderMatchingSystemStatus application message — disseminates the status of
/// EuroCTP's order-matching system (§4.2.13).
class OrderMatchingSystemStatus
{
    MessageHeader                       const   m_MessageHeader;
    OrderMatchingSystemStatusStruct     const & m_Struct;

public:
    static constexpr unsigned int TYPE = MSG_ID_ORDER_MATCHING_SYSTEM_STATUS;
    static constexpr unsigned int SIZE = MessageHeader::SIZE + sizeof(OrderMatchingSystemStatusStruct);

    /// @param data Raw byte buffer that MUST start at the FramingHeader and
    ///             remain alive for the lifetime of this instance.
    explicit OrderMatchingSystemStatus(InputData const & data)
    : m_MessageHeader(data)
    , m_Struct(*reinterpret_cast<const OrderMatchingSystemStatusStruct*>(data.data() + MessageHeader::SIZE))
    {
    }

    MessageHeader const & getMessageHeader() const { return m_MessageHeader; }

    types::SeqNum               getMsgSeqNum()     const { return m_Struct.m_MsgSeqNum; }
    std::string                 getMarketID()      const { return std::string(m_Struct.m_MarketID, types::MIC_SIZE); }
    types::VenueType            getVenueType()     const { return m_Struct.m_VenueType; }
    types::TradSesStatus        getTradSesStatus() const { return m_Struct.m_TradSesStatus; }
    types::TimestampNano        getTransactTime()  const { return m_Struct.m_TransactTime; }
    types::TimestampNano        getEffectiveTime() const { return m_Struct.m_EffectiveTime; }
    types::InternalIdentifier   getMDEntryID()     const { return m_Struct.m_MDEntryID; }

    void dump(std::ostream & os) const
    {
        os << "OrderMatchingSystemStatus:\n";
        m_MessageHeader.dump(os);
        os
        DUMP_FIELD(MsgSeqNum)
        DUMP_FIELD(MarketID)
        ;
        os << "  VenueType="     << static_cast<char>(m_Struct.m_VenueType)        << "\n";
        os << "  TradSesStatus=" << static_cast<int>(m_Struct.m_TradSesStatus)     << "\n";
        os
        DUMP_FIELD(TransactTime)
        DUMP_FIELD(EffectiveTime)
        DUMP_FIELD(MDEntryID)
        ;
    }
};

inline std::ostream& operator<<(std::ostream& out, OrderMatchingSystemStatus const& obj)
{
    obj.dump(out);
    return out;
}

} // namespace protocol::messages
