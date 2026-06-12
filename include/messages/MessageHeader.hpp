// SPDX-License-Identifier: TODO
// MessageHeader — STUB.
//
// ⚠️  The real layout depends on the protocol spec. The /pdf-to-cpp-messages
// workflow MUST ask the user for the actual MessageHeader definition before
// generating any messages and replace this stub.

#pragma once

#include <ostream>

#include "messages/Common.hpp"

namespace protocol::messages {

#pragma pack(push, 1)

/// TODO: replace fields below with the real MessageHeader described in the spec.
struct MessageHeaderStruct
{
    uint16 m_MessageLength;  ///< Total length of the message in bytes (incl. header).
    uint16 m_MessageType;    ///< Opcode of the payload.
};

#pragma pack(pop)

class MessageHeader
{
    MessageHeaderStruct const & m_Struct;
public:
    static constexpr unsigned int SIZE = sizeof(MessageHeaderStruct);

    explicit MessageHeader(InputData const & data)
    : m_Struct(*reinterpret_cast<const MessageHeaderStruct*>(data.data()))
    {
    }

    uint16 getMessageLength() const { return m_Struct.m_MessageLength; }
    uint16 getMessageType()   const { return m_Struct.m_MessageType; }

    void dump(std::ostream & os) const
    {
        os
        DUMP_FIELD(MessageLength)
        DUMP_FIELD(MessageType)
        ;
    }
};

} // namespace protocol::messages
