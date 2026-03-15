import os

base_dir = r"c:\Users\YUVANESH\Desktop\projects\gem"
folders_and_files = {
    "src/core": ["event_bus.h", "event_bus.cpp", "scheduler.h", "scheduler.cpp", "rtp_scheduler.h", "rtp_scheduler.cpp"],
    "src/protocol": ["message_types.h", "rtp.h", "rtp.cpp", "serializer.h", "serializer.cpp"],
    "src/utils": ["jitter_buffer.h", "jitter_buffer.cpp", "adaptive_jitter_buffer.h", "adaptive_jitter_buffer.cpp", "vad.h", "vad.cpp", "crypto_utils.h", "crypto_utils.cpp"],
    "src/audio": ["engine.h", "engine.cpp", "mixer.h", "mixer.cpp", "agc.h", "agc.cpp", "ns.h", "ns.cpp", "echo.h", "echo.cpp", "codec.h", "codec.cpp", "speaker_detector.h", "speaker_detector.cpp", "stream_stats.h", "stream_stats.cpp"],
    "src/network": ["discovery.h", "discovery.cpp", "stun.h", "stun.cpp", "turn.h", "turn.cpp", "ice_agent.h", "ice_agent.cpp", "keepalive.h", "keepalive.cpp", "tree.h", "tree.cpp", "peer_manager.h", "peer_manager.cpp", "rtcp.h", "rtcp.cpp", "congestion_controller.h", "congestion_controller.cpp", "router_load.h", "router_load.cpp", "subscription_table.h", "subscription_table.cpp", "failover_manager.h", "failover_manager.cpp", "qos.h", "qos.cpp"],
    "src/security": ["dtls_srtp.h", "dtls_srtp.cpp", "key_exchange.h", "key_exchange.cpp", "auth.h", "auth.cpp", "replay_protection.h", "replay_protection.cpp"],
    "src/ui": ["main_window.h", "main_window.cpp", "startup_dialog.h", "startup_dialog.cpp", "settings_dialog.h", "settings_dialog.cpp", "participant_row.h", "participant_row.cpp", "volume_panel.h", "volume_panel.cpp"],
    "src/tools": ["network_graph.h", "network_graph.cpp", "packet_capture.h", "packet_capture.cpp", "metrics_server.h", "metrics_server.cpp"],
    "tests": [],
}

# Create CMakeLists.txt
cmake_path = os.path.join(base_dir, "CMakeLists.txt")
if not os.path.exists(cmake_path):
    with open(cmake_path, "w") as f:
        f.write("cmake_minimum_required(VERSION 3.16)\nproject(VoIP_Serverless)\n\nset(CMAKE_CXX_STANDARD 20)\n\n# Placeholder for finding Qt and adding executable\n")

for folder, files in folders_and_files.items():
    folder_path = os.path.join(base_dir, folder)
    os.makedirs(folder_path, exist_ok=True)
    for file in files:
        file_path = os.path.join(folder_path, file)
        if not os.path.exists(file_path):
            with open(file_path, "w") as f:
                if file.endswith(".h"):
                    guard = f"{file.replace('.', '_').upper()}_"
                    f.write(f"#ifndef {guard}\n#define {guard}\n\n#pragma once\n\n#endif // {guard}\n")
                elif file.endswith(".cpp"):
                    header = file.replace(".cpp", ".h")
                    f.write(f'#include "{header}"\n\n')

print("Scaffold complete.")
