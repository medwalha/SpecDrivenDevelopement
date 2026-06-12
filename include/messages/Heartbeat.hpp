// SPDX-License-Identifier: TODO
// Auto-generated from specs/euro_ctp_spec.pdf — §4.2.1 Heartbeat
// Spec version: 1.3   Generated: 2026-06-12
// TemplateID = 1 (Administrative, SchemaID=2). Empty payload.

#pragma once

#include <ostream>

#include "messages/Common.hpp"
#include "messages/MessageHeader.hpp"
#include "messages/MessageIds.hpp"
#include "messages/types/Types.hpp"

namespace protocol::messages {

/// Heartbeat — sent every second on a channel that is otherwise idle.
/// Conforms to the standard SBE packet/message header structure but carries
/// no payload (§4.2.1).
class Heartbeat
{
    MessageHeader const m_MessageHeader;

public:
    static constexpr unsigned int TYPE = MSG_ID_HEARTBEAT;
    static constexpr unsigned int SIZE = MessageHeader::SIZE;   // header only, no payload

    /// @param data Raw byte buffer that MUST start at the FramingHeader and
    ///             remain alive for the lifetime of this Heartbeat instance.
    explicit Heartbeat(InputData const & data)
    : m_MessageHeader(data)
    {
    }

    MessageHeader const & getMessageHeader() const { return m_MessageHeader; }

    void dump(std::ostream & os) const
    {
        os << "Heartbeat:\n";
        m_MessageHeader.dump(os);
    }
};

inline std::ostream& operator<<(std::ostream& out, Heartbeat const& obj)
{
    obj.dump(out);
    return out;
}

} // namespace protocol::messages
