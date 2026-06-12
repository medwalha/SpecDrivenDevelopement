// SPDX-License-Identifier: TODO
// PacketHeader — STUB.
//
// ⚠️  Some protocols wrap one or more MessageHeader inside a PacketHeader
// (transport-level framing). The /pdf-to-cpp-messages workflow MUST ask the
// user whether a PacketHeader exists and, if so, replace this stub with the
// real layout.

#pragma once

#include <ostream>

#include "messages/Common.hpp"

namespace protocol::messages {

#pragma pack(push, 1)

/// TODO: replace fields below with the real PacketHeader described in the spec
/// (or delete this file if the protocol has no packet-level framing).
struct PacketHeaderStruct
{
    uint32 m_PacketSeqNum;   ///< Packet sequence number.
    uint64 m_SendingTime;    ///< Sending time (ns since epoch, mod 2^64).
};

#pragma pack(pop)

class PacketHeader
{
    PacketHeaderStruct const & m_Struct;
public:
    static constexpr unsigned int SIZE = sizeof(PacketHeaderStruct);

    explicit PacketHeader(InputData const & data)
    : m_Struct(*reinterpret_cast<const PacketHeaderStruct*>(data.data()))
    {
    }

    uint32 getPacketSeqNum() const { return m_Struct.m_PacketSeqNum; }
    uint64 getSendingTime()  const { return m_Struct.m_SendingTime; }

    void dump(std::ostream & os) const
    {
        os
        DUMP_FIELD(PacketSeqNum)
        DUMP_FIELD(SendingTime)
        ;
    }
};

} // namespace protocol::messages
