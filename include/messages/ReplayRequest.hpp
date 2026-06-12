// SPDX-License-Identifier: TODO
// Auto-generated from specs/euro_ctp_spec.pdf — §4.2.4 Replay Request message
// Spec version: 1.3   Generated: 2026-06-12
// TemplateID = 20 (Administrative, SchemaID=2). Payload = 19 bytes.

#pragma once

#include <ostream>
#include <type_traits>

#include "messages/Common.hpp"
#include "messages/MessageHeader.hpp"
#include "messages/MessageIds.hpp"
#include "messages/types/Types.hpp"

namespace protocol::messages {

#pragma pack(push, 1)

/// ReplayRequest payload — issued by clients on the TCP replay channel
/// (§4.2.4).
struct ReplayRequestStruct
{
    uint8           m_FeedID;             ///< 1B — feed for which the replay is requested.
    uint8           m_ChannelID;          ///< 1B — channel within the feed.
    uint8           m_PacketSeqVersion;   ///< 1B — must match the most recent SequenceReset value.
    types::SeqNum   m_BeginSeqNo;         ///< 8B — first packet sequence to replay.
    types::SeqNum   m_EndSeqNo;           ///< 8B — last packet sequence to replay.
};

#pragma pack(pop)

static_assert(sizeof(ReplayRequestStruct) == 19,
              "ReplayRequestStruct size mismatch with spec (19 bytes = 1+1+1+8+8)");
static_assert(std::is_standard_layout<ReplayRequestStruct>::value,
              "ReplayRequestStruct must be standard-layout");
static_assert(std::is_trivially_copyable<ReplayRequestStruct>::value,
              "ReplayRequestStruct must be trivially copyable");

/// ReplayRequest administrative message — asks EuroCTP to retransmit a range
/// of packets for a given channel (§4.2.4).
class ReplayRequest
{
    MessageHeader        const   m_MessageHeader;
    ReplayRequestStruct  const & m_Struct;

public:
    static constexpr unsigned int TYPE = MSG_ID_REPLAY_REQUEST;
    static constexpr unsigned int SIZE = MessageHeader::SIZE + sizeof(ReplayRequestStruct);

    /// @param data Raw byte buffer that MUST start at the FramingHeader and
    ///             remain alive for the lifetime of this ReplayRequest instance.
    explicit ReplayRequest(InputData const & data)
    : m_MessageHeader(data)
    , m_Struct(*reinterpret_cast<const ReplayRequestStruct*>(data.data() + MessageHeader::SIZE))
    {
    }

    MessageHeader const & getMessageHeader() const { return m_MessageHeader; }

    uint8           getFeedID()           const { return m_Struct.m_FeedID; }
    uint8           getChannelID()        const { return m_Struct.m_ChannelID; }
    uint8           getPacketSeqVersion() const { return m_Struct.m_PacketSeqVersion; }
    types::SeqNum   getBeginSeqNo()       const { return m_Struct.m_BeginSeqNo; }
    types::SeqNum   getEndSeqNo()         const { return m_Struct.m_EndSeqNo; }

    void dump(std::ostream & os) const
    {
        os << "ReplayRequest:\n";
        m_MessageHeader.dump(os);
        os
        DUMP_FIELD_AS_INT(FeedID)
        DUMP_FIELD_AS_INT(ChannelID)
        DUMP_FIELD_AS_INT(PacketSeqVersion)
        DUMP_FIELD(BeginSeqNo)
        DUMP_FIELD(EndSeqNo)
        ;
    }
};

inline std::ostream& operator<<(std::ostream& out, ReplayRequest const& obj)
{
    obj.dump(out);
    return out;
}

} // namespace protocol::messages
