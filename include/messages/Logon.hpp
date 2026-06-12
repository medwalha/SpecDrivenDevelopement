// SPDX-License-Identifier: TODO
// Auto-generated from specs/euro_ctp_spec.pdf — §4.2.2 Logon message
// Spec version: 1.3   Generated: 2026-06-12
// TemplateID = 10 (Administrative, SchemaID=2). Payload = 112 bytes.

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

/// Logon payload — sent by clients connecting to the TCP replay channel
/// (§4.2.2).
struct LogonStruct
{
    types::Username    m_Username;          ///< 32B — client username for authentication.
    types::Password    m_Password;          ///< 32B — client password for authentication.
    types::AppId       m_ClientAppId;       ///< 32B — client application identifier.
    types::AppVersion  m_ClientAppVersion;  ///< 16B — client application version.
};

#pragma pack(pop)

static_assert(sizeof(LogonStruct) == 112,
              "LogonStruct size mismatch with spec (112 bytes = 32+32+32+16)");
static_assert(std::is_standard_layout<LogonStruct>::value,
              "LogonStruct must be standard-layout");
static_assert(std::is_trivially_copyable<LogonStruct>::value,
              "LogonStruct must be trivially copyable");

/// Logon administrative message — authenticates a client onto the replay
/// channel and conveys the client's app identity (§4.2.2).
class Logon
{
    MessageHeader  const   m_MessageHeader;
    LogonStruct    const & m_Struct;

public:
    static constexpr unsigned int TYPE = MSG_ID_LOGON;
    static constexpr unsigned int SIZE = MessageHeader::SIZE + sizeof(LogonStruct);

    /// @param data Raw byte buffer that MUST start at the FramingHeader and
    ///             remain alive for the lifetime of this Logon instance.
    explicit Logon(InputData const & data)
    : m_MessageHeader(data)
    , m_Struct(*reinterpret_cast<const LogonStruct*>(data.data() + MessageHeader::SIZE))
    {
    }

    MessageHeader const & getMessageHeader() const { return m_MessageHeader; }

    std::string getUsername()         const { return std::string(m_Struct.m_Username,         types::USERNAME_SIZE); }
    std::string getPassword()         const { return std::string(m_Struct.m_Password,         types::PASSWORD_SIZE); }
    std::string getClientAppId()      const { return std::string(m_Struct.m_ClientAppId,      types::APP_ID_SIZE); }
    std::string getClientAppVersion() const { return std::string(m_Struct.m_ClientAppVersion, types::APP_VERSION_SIZE); }

    void dump(std::ostream & os) const
    {
        os << "Logon:\n";
        m_MessageHeader.dump(os);
        os
        DUMP_FIELD(Username)
        DUMP_FIELD(Password)
        DUMP_FIELD(ClientAppId)
        DUMP_FIELD(ClientAppVersion)
        ;
    }
};

inline std::ostream& operator<<(std::ostream& out, Logon const& obj)
{
    obj.dump(out);
    return out;
}

} // namespace protocol::messages
