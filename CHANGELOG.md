# CHANGELOG

All notable changes to this project will be documented in this file.

## [2026-02-16] Fix firmware flashing

### Reason
ESP32 kept rebooting after flashing because only firmware.bin was uploaded, missing bootloader and partitions.

### Impact
Device was non-functional, USB CDC kept reconnecting.

### Solution
- Updated CI to upload all three binary files
- Documented proper flash commands in KNOWLEDGE.md

---

## [2026-02-16] Add KNOWLEDGE.md

### Reason
Need to record project experience and pitfalls for future reference and to help AI agents avoid repeating mistakes.

### Impact
Added new file KNOWLEDGE.md, updated AGENTS.md project structure.

### Solution
Created KNOWLEDGE.md with categories: Environment Setup, Development, Deployment. Each entry includes problem description and solution.

---

## [2026-02-16] Initial Project Setup

### Reason
N/A - Initial project creation

### Impact
N/A

### Solution
Created ESP32-C3 WiFi Scanner project with:
- PlatformIO configuration
- WiFi AP mode (SSID: ESP32-Scanner)
- Web interface with scan button
- Scan results display (SSID, RSSI, encryption, channel, bandwidth)
- Sorted by signal strength
- CI/CD workflow (GitHub Actions)
- Release process documentation
- AGENTS.md behavior rules
- TODO.md task tracking
