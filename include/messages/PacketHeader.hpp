// SPDX-License-Identifier: TODO
// PacketHeader — concrete implementation for EuroCTP Consolidated Tape feed.
//
// One PacketHeader prefixes every multicast packet and may be followed by one
// or more (FramingHeader + UnitHeader + payload) tuples. Wire layout is
// little-endian (§4.1.4).
//
// Source : specs/euro_ctp_spec.pdf — section §4.1.5 (Packet header)
// Spec version: 1.3   Generated: 2026-06-12

#pragma once

#include <ostream>
#include <type_traits>

#include "messages/Common.hpp"

namespace protocol::messages {

/// Magic value of the EncodingType field, in little-endian byte order
/// (§4.1.5 — "Value is 0xEC79 (60537) in little-endian encoding").
inline constexpr uint16 PACKET_HEADER_ENCODING_TYPE = 0xEC79;

#pragma pack(push, 1)

/// EuroCTP packet header on the wire.
/// Wire layout (little-endian):
///   uint16 EncodingType | uint16 PacketSize | uint8 FeedID | uint8 ChannelID |
///   uint8  PacketSeqVersion | uint64 PacketSeqNum | uint64 DisseminationTime
/// Total: 23 bytes.
struct PacketHeaderStruct
{
    uint16 m_EncodingType;        ///< Encoding identifier — always 0xEC79 (little-endian).
    uint16 m_PacketSize;          ///< Overall packet size including this header.
    uint8  m_FeedID;              ///< Feed ID (see types::FeedId, §3.1.1).
    uint8  m_ChannelID;           ///< Channel ID — partition within the feed (spec spelling: "ChanelID").
    uint8  m_PacketSeqVersion;    ///< Version of the packet-level sequence number.
    uint64 m_PacketSeqNum;        ///< Monotonically-increasing packet sequence number.
    uint64 m_DisseminationTime;   ///< Nanoseconds since Epoch (publication time).
};

#pragma pack(pop)

static_assert(sizeof(PacketHeaderStruct) == 23,
              "PacketHeaderStruct size mismatch with spec (23 bytes)");
static_assert(std::is_standard_layout<PacketHeaderStruct>::value,
              "PacketHeaderStruct must be standard-layout");
static_assert(std::is_trivially_copyable<PacketHeaderStruct>::value,
              "PacketHeaderStruct must be trivially copyable");

class PacketHeader
{
    PacketHeaderStruct const & m_Struct;
public:
    static constexpr unsigned int SIZE = sizeof(PacketHeaderStruct);

    /// @param data Raw byte buffer that MUST start at the PacketHeader and
    ///             remain alive for the lifetime of this PacketHeader instance.
    explicit PacketHeader(InputData const & data)
    : m_Struct(*reinterpret_cast<const PacketHeaderStruct*>(data.data()))
    {
    }

    uint16 getEncodingType()      const { return m_Struct.m_EncodingType; }
    uint16 getPacketSize()        const { return m_Struct.m_PacketSize; }
    uint8  getFeedID()            const { return m_Struct.m_FeedID; }
    uint8  getChannelID()         const { return m_Struct.m_ChannelID; }
    uint8  getPacketSeqVersion()  const { return m_Struct.m_PacketSeqVersion; }
    uint64 getPacketSeqNum()      const { return m_Struct.m_PacketSeqNum; }
    uint64 getDisseminationTime() const { return m_Struct.m_DisseminationTime; }

    void dump(std::ostream & os) const
    {
        os
        DUMP_FIELD(EncodingType)
        DUMP_FIELD(PacketSize)
        DUMP_FIELD_AS_INT(FeedID)
        DUMP_FIELD_AS_INT(ChannelID)
        DUMP_FIELD_AS_INT(PacketSeqVersion)
        DUMP_FIELD(PacketSeqNum)
        DUMP_FIELD(DisseminationTime)
        ;
    }
};

inline std::ostream& operator<<(std::ostream& out, PacketHeader const& obj)
{
    obj.dump(out);
    return out;
}

} // namespace protocol::messages
