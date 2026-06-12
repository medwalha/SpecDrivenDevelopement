// SPDX-License-Identifier: TODO
// Auto-generated from specs/euro_ctp_spec.pdf — §4.2.6 Snapshot End message
// Spec version: 1.3   Generated: 2026-06-12
// TemplateID = 30 (Administrative, SchemaID=2). Empty payload.

#pragma once

#include <ostream>

#include "messages/Common.hpp"
#include "messages/MessageHeader.hpp"
#include "messages/MessageIds.hpp"
#include "messages/types/Types.hpp"

namespace protocol::messages {

/// SnapshotEnd — marks the end of a snapshot transmission on a snapshot channel.
/// FeedID / ChannelID are provided in the packet header. No payload (§4.2.6).
class SnapshotEnd
{
    MessageHeader const m_MessageHeader;

public:
    static constexpr unsigned int TYPE = MSG_ID_SNAPSHOT_END;
    static constexpr unsigned int SIZE = MessageHeader::SIZE;   // header only, no payload

    /// @param data Raw byte buffer that MUST start at the FramingHeader and
    ///             remain alive for the lifetime of this SnapshotEnd instance.
    explicit SnapshotEnd(InputData const & data)
    : m_MessageHeader(data)
    {
    }

    MessageHeader const & getMessageHeader() const { return m_MessageHeader; }

    void dump(std::ostream & os) const
    {
        os << "SnapshotEnd:\n";
        m_MessageHeader.dump(os);
    }
};

inline std::ostream& operator<<(std::ostream& out, SnapshotEnd const& obj)
{
    obj.dump(out);
    return out;
}

} // namespace protocol::messages
