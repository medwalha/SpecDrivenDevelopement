// SPDX-License-Identifier: TODO
// Auto-generated from specs/euro_ctp_spec.pdf — §4.2.5 Replay Start message
// Spec version: 1.3   Generated: 2026-06-12
// TemplateID = 21 (Administrative, SchemaID=2). Payload = 17 bytes.

#pragma once

#include <ostream>
#include <type_traits>

#include "messages/Common.hpp"
#include "messages/MessageHeader.hpp"
#include "messages/MessageIds.hpp"
#include "messages/types/Types.hpp"

namespace protocol::messages {

#pragma pack(push, 1)

/// ReplayStart payload — server-side acknowledgement that a ReplayRequest is
/// being processed (§4.2.5).
struct ReplayStartStruct
{
    uint8           m_PacketSeqVersion;   ///< 1B — packet-level sequence number version.
    types::SeqNum   m_BeginSeqNo;         ///< 8B — first packet sequence that will be replayed.
    types::SeqNum   m_EndSeqNo;           ///< 8B — last packet sequence that will be replayed.
};

#pragma pack(pop)

static_assert(sizeof(ReplayStartStruct) == 17,
              "ReplayStartStruct size mismatch with spec (17 bytes = 1+8+8)");
static_assert(std::is_standard_layout<ReplayStartStruct>::value,
              "ReplayStartStruct must be standard-layout");
static_assert(std::is_trivially_copyable<ReplayStartStruct>::value,
              "ReplayStartStruct must be trivially copyable");

/// ReplayStart administrative message — sent by EuroCTP to confirm a replay
/// is about to begin and to advertise the actual range that will be resent
/// (§4.2.5).
class ReplayStart
{
    MessageHeader      const   m_MessageHeader;
    ReplayStartStruct  const & m_Struct;

public:
    static constexpr unsigned int TYPE = MSG_ID_REPLAY_START;
    static constexpr unsigned int SIZE = MessageHeader::SIZE + sizeof(ReplayStartStruct);

    /// @param data Raw byte buffer that MUST start at the FramingHeader and
    ///             remain alive for the lifetime of this ReplayStart instance.
    explicit ReplayStart(InputData const & data)
    : m_MessageHeader(data)
    , m_Struct(*reinterpret_cast<const ReplayStartStruct*>(data.data() + MessageHeader::SIZE))
    {
    }

    MessageHeader const & getMessageHeader() const { return m_MessageHeader; }

    uint8           getPacketSeqVersion() const { return m_Struct.m_PacketSeqVersion; }
    types::SeqNum   getBeginSeqNo()       const { return m_Struct.m_BeginSeqNo; }
    types::SeqNum   getEndSeqNo()         const { return m_Struct.m_EndSeqNo; }

    void dump(std::ostream & os) const
    {
        os << "ReplayStart:\n";
        m_MessageHeader.dump(os);
        os
        DUMP_FIELD_AS_INT(PacketSeqVersion)
        DUMP_FIELD(BeginSeqNo)
        DUMP_FIELD(EndSeqNo)
        ;
    }
};

inline std::ostream& operator<<(std::ostream& out, ReplayStart const& obj)
{
    obj.dump(out);
    return out;
}

} // namespace protocol::messages
