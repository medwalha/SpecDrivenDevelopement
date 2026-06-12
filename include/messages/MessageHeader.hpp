// SPDX-License-Identifier: TODO
// MessageHeader — concrete implementation for EuroCTP Consolidated Tape feed.
// Composition of:
//   - FramingHeader (§4.1.6) — 2 bytes
//   - UnitHeader    (§4.1.7) — 8 bytes
// Total: 10 bytes, little-endian (§4.1.4).
//
// Source : specs/euro_ctp_spec.pdf — sections §4.1.6 and §4.1.7
// Spec version: 1.3   Generated: 2026-06-12

#pragma once

#include <ostream>
#include <type_traits>

#include "messages/Common.hpp"
#include "messages/types/Structs.hpp"   // FramingHeader + UnitHeader

namespace protocol::messages {

#pragma pack(push, 1)

/// EuroCTP message header on the wire = FramingHeader || UnitHeader.
/// Wire layout (little-endian):
///   uint16 MessageLength | uint16 MsgSize | uint16 TemplateID | uint16 SchemaID | uint16 Version
struct MessageHeaderStruct
{
    types::FramingHeader m_FramingHeader;    ///< §4.1.6 — outer SBE framing.
    types::UnitHeader    m_UnitHeader;       ///< §4.1.7 — SBE unit / template descriptor.
};

#pragma pack(pop)

static_assert(sizeof(MessageHeaderStruct) == 10,
              "MessageHeaderStruct size mismatch with spec (10 bytes = 2 + 8)");
static_assert(std::is_standard_layout<MessageHeaderStruct>::value,
              "MessageHeaderStruct must be standard-layout");
static_assert(std::is_trivially_copyable<MessageHeaderStruct>::value,
              "MessageHeaderStruct must be trivially copyable");

class MessageHeader
{
    MessageHeaderStruct const & m_Struct;
public:
    static constexpr unsigned int SIZE = sizeof(MessageHeaderStruct);

    /// @param data Raw byte buffer that MUST start at the FramingHeader and
    ///             remain alive for the lifetime of this MessageHeader instance.
    explicit MessageHeader(InputData const & data)
    : m_Struct(*reinterpret_cast<const MessageHeaderStruct*>(data.data()))
    {
    }

    // Framing header accessor ------------------------------------------------
    uint16 getMessageLength() const { return m_Struct.m_FramingHeader.m_MessageLength; }

    // Unit header accessors --------------------------------------------------
    uint16 getMsgSize()       const { return m_Struct.m_UnitHeader.m_MsgSize; }
    uint16 getTemplateID()    const { return m_Struct.m_UnitHeader.m_TemplateID; }
    uint16 getSchemaID()      const { return m_Struct.m_UnitHeader.m_SchemaID; }
    uint16 getVersion()       const { return m_Struct.m_UnitHeader.m_Version; }

    void dump(std::ostream & os) const
    {
        os
        DUMP_FIELD(MessageLength)
        DUMP_FIELD(MsgSize)
        DUMP_FIELD(TemplateID)
        DUMP_FIELD(SchemaID)
        DUMP_FIELD(Version)
        ;
    }
};

} // namespace protocol::messages
