// SPDX-License-Identifier: TODO
// Auto-generated from specs/euro_ctp_spec.pdf — §4.2.7 Sequence reset message
// Spec version: 1.3   Generated: 2026-06-12
// TemplateID = 2 (Administrative, SchemaID=2). Payload = 1 byte.

#pragma once

#include <ostream>
#include <type_traits>

#include "messages/Common.hpp"
#include "messages/MessageHeader.hpp"
#include "messages/MessageIds.hpp"
#include "messages/types/Types.hpp"

namespace protocol::messages {

#pragma pack(push, 1)

/// SequenceReset payload — single byte indicating the new packet-level
/// sequence-number version for the channel (§4.2.7).
struct SequenceResetStruct
{
    uint8 m_PacketSeqVersion;   ///< Version of the packet-level sequence number.
};

#pragma pack(pop)

static_assert(sizeof(SequenceResetStruct) == 1,
              "SequenceResetStruct size mismatch with spec (1 byte)");
static_assert(std::is_standard_layout<SequenceResetStruct>::value,
              "SequenceResetStruct must be standard-layout");
static_assert(std::is_trivially_copyable<SequenceResetStruct>::value,
              "SequenceResetStruct must be trivially copyable");

/// SequenceReset administrative message — announces the new packet-level
/// sequence-number version for a given channel (§4.2.7).
class SequenceReset
{
    MessageHeader        const   m_MessageHeader;
    SequenceResetStruct  const & m_Struct;

public:
    static constexpr unsigned int TYPE = MSG_ID_SEQUENCE_RESET;
    static constexpr unsigned int SIZE = MessageHeader::SIZE + sizeof(SequenceResetStruct);

    /// @param data Raw byte buffer that MUST start at the FramingHeader and
    ///             remain alive for the lifetime of this SequenceReset instance.
    explicit SequenceReset(InputData const & data)
    : m_MessageHeader(data)
    , m_Struct(*reinterpret_cast<const SequenceResetStruct*>(data.data() + MessageHeader::SIZE))
    {
    }

    MessageHeader const & getMessageHeader() const { return m_MessageHeader; }

    uint8 getPacketSeqVersion() const { return m_Struct.m_PacketSeqVersion; }

    void dump(std::ostream & os) const
    {
        os << "SequenceReset:\n";
        m_MessageHeader.dump(os);
        os
        DUMP_FIELD_AS_INT(PacketSeqVersion)
        ;
    }
};

inline std::ostream& operator<<(std::ostream& out, SequenceReset const& obj)
{
    obj.dump(out);
    return out;
}

} // namespace protocol::messages
