# TODO - ESP32 WiFi Scanner

## Current Status
- [x] Create project structure with PlatformIO
- [x] Implement WiFi AP mode with hardcoded SSID
- [x] Implement web server with landing page
- [x] Implement WiFi scan functionality
- [x] Display scan results (SSID, RSSI, encryption, channel)
- [x] Sort results by signal strength
- [x] Add CI/CD workflow with GitHub Actions
- [x] Add release process documentation
- [x] Create AGENTS.md with behavior rules
- [x] Add KNOWLEDGE.md for recording pitfalls
- [x] Add CHANGELOG.md for tracking changes
- [x] Verify firmware flashes correctly with full binary

## Future Enhancements
- [ ] Add BLE provisioning for configurable AP credentials
- [ ] Add scan history storage
- [ ] Add mobile-friendly UI improvements
- [ ] Add signal strength visualization (charts/graphs)
- [ ] Add export results functionality (CSV/JSON)
- [ ] Add multi-language support

## Technical Debt
- [ ] Bandwidth detection reliability improvement
- [ ] Memory usage optimization for large scan results
- [ ] Async scan to avoid blocking web server
