// SPDX-License-Identifier: TODO
// Auto-generated from specs/euro_ctp_spec.pdf — §4.2.3 Logout message
// Spec version: 1.3   Generated: 2026-06-12
// TemplateID = 11 (Administrative, SchemaID=2). Payload = 258 bytes.

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

/// Logout payload — server-initiated session termination notice (§4.2.3).
struct LogoutStruct
{
    uint16              m_LogoutCode;     ///< 2B — root cause code (see §<Logout Reason Codes>).
    types::LogoutReason m_LogoutReason;   ///< 256B — free-form reason text (alphanumeric).
};

#pragma pack(pop)

static_assert(sizeof(LogoutStruct) == 258,
              "LogoutStruct size mismatch with spec (258 bytes = 2+256)");
static_assert(std::is_standard_layout<LogoutStruct>::value,
              "LogoutStruct must be standard-layout");
static_assert(std::is_trivially_copyable<LogoutStruct>::value,
              "LogoutStruct must be trivially copyable");

/// Logout administrative message — sent by the server before disconnecting
/// the session (§4.2.3).
class Logout
{
    MessageHeader  const   m_MessageHeader;
    LogoutStruct   const & m_Struct;

public:
    static constexpr unsigned int TYPE = MSG_ID_LOGOUT;
    static constexpr unsigned int SIZE = MessageHeader::SIZE + sizeof(LogoutStruct);

    /// @param data Raw byte buffer that MUST start at the FramingHeader and
    ///             remain alive for the lifetime of this Logout instance.
    explicit Logout(InputData const & data)
    : m_MessageHeader(data)
    , m_Struct(*reinterpret_cast<const LogoutStruct*>(data.data() + MessageHeader::SIZE))
    {
    }

    MessageHeader const & getMessageHeader() const { return m_MessageHeader; }

    uint16      getLogoutCode()   const { return m_Struct.m_LogoutCode; }
    std::string getLogoutReason() const { return std::string(m_Struct.m_LogoutReason, types::LOGOUT_REASON_SIZE); }

    void dump(std::ostream & os) const
    {
        os << "Logout:\n";
        m_MessageHeader.dump(os);
        os
        DUMP_FIELD(LogoutCode)
        DUMP_FIELD(LogoutReason)
        ;
    }
};

inline std::ostream& operator<<(std::ostream& out, Logout const& obj)
{
    obj.dump(out);
    return out;
}

} // namespace protocol::messages
