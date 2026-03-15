#ifndef NET_CONTROL_CLIENT_H
#define NET_CONTROL_CLIENT_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include <cstdint>

struct ControlResponse {
    bool ok{true};
    std::string response;
};

// Minimal stub transport for UI wiring. Replace with real UDP/TCP control path.
ControlResponse send_control_command(const std::string& server_ip,
                                     const std::string& command,
                                     int timeout_ms = 1000);

std::vector<std::string> parse_client_list_response(const std::string& resp);

// Broadcast presence and check if name already taken on the LAN/host.
bool ensure_unique_name(const std::string& client_id, uint16_t audio_port);
std::set<std::string> discovered_clients();
uint16_t port_for_client(const std::string& client_id);
std::string getClientEndpoint(const std::string& client_id);

#endif
