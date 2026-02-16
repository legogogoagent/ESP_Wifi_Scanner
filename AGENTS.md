# AGENTS.md - ESP32 WiFi Scanner Project

## Project Overview

This is an ESP32-C3 Super Mini WiFi Scanner project using PlatformIO with Arduino framework. The device creates a WiFi AP and serves a web interface where users can scan for nearby WiFi networks.

## Build Commands

### Standard Build
```bash
pio run
```

### Upload to Device
```bash
pio run --target upload
```

### Monitor Serial Output
```bash
pio device monitor
```

### Build and Upload (Combined)
```bash
pio run --target upload --target monitor
```

### Clean Build
```bash
pio run --target clean
```

### Run Specific Task
```bash
pio run --target <task>
```

## Code Style Guidelines

### File Structure
- Keep `main.cpp` as the entry point
- Use descriptive variable and function names
- Group related functionality together

### Naming Conventions
- Functions: `camelCase` (e.g., `startScan()`, `getEncryptionType()`)
- Variables: `camelCase` (e.g., `networkCount`, `jsonResponse`)
- Constants: `UPPER_SNAKE_CASE` (e.g., `MAX_NETWORKS`, `DEFAULT_PORT`)
- Classes/Types: `PascalCase` (e.g., `AsyncWebServer`)

### HTML/JS in C++
- Embed simple HTML/CSS/JS inline using `PROGMEM` and raw string literals
- Keep HTML minimal and use inline CSS for embedded projects
- Use vanilla JavaScript (no frameworks) to minimize memory usage

### ESP32 Specific
- Use `Serial.begin(115200)` for debug output
- Keep HTML strings in PROGMEM to save RAM
- Use `AsyncWebServer` for non-blocking web server
- WiFi scanning is blocking - handle UI accordingly

### Error Handling
- Always check return values from WiFi operations
- Use Serial.println() for debug messages
- Provide meaningful error messages to web clients

### Memory Optimization
- Use `PROGMEM` for constant strings (HTML, messages)
- Limit array sizes (e.g., `networks[50]`)
- Free memory after scanning if needed
- Avoid dynamic memory allocation in loops

### Dependencies
- ESPAsyncWebServer: Async HTTP server
- AsyncTCP: Required dependency for ESPAsyncWebServer
- Both installed via lib_deps in platformio.ini

## Project Structure

```
ESP_Wifi_Scanner/
├── .github/
│   └── workflows/
│       └── ci.yml          # GitHub Actions CI
├── scripts/
│   └── ci.sh               # Unified test script
├── src/
│   └── main.cpp            # Main application code
├── AGENTS.md               # This file - agent behavior rules
├── TODO.md                 # Task planning and tracking
├── CHANGELOG.md            # Change log (required when modifying TODO)
├── RELEASE.md              # Release process documentation
└── platformio.ini          # PlatformIO configuration
```

## TODO List Workflow

### Before Any Change
1. Review `TODO.md` for existing plan
2. Evaluate proposed changes against TODO plan
3. Check for conflicts or misalignment
4. If changes affect TODO plan → document in `CHANGELOG.md`:
   - Reason for change
   - Impact assessment
   - Solution/approach

### CHANGELOG.md Required Fields (when modifying TODO)
```markdown
## [Date] - Change Description

### Reason
Why the TODO plan needs modification

### Impact
What this change affects

### Solution
How to implement the change
```

### After Task Completion
- Mark completed tasks in TODO.md as `[x]`
- Add new tasks if scope expanded
- Update TODO.md in the same commit as code changes

### Commit Pattern
- Update TODO.md together with code changes
- If TODO changed → commit message must reference CHANGELOG entry

## CI/CD Workflow

### CI Trigger
- Push to `develop` or `main` branches triggers `.github/workflows/ci.yml`
- Concurrent runs: new push cancels old in-progress runs

### Test Entry Point
- Always use: `bash scripts/ci.sh`
- This script auto-detects project type and runs appropriate tests
- For this project: runs `pio run`

## Release Process

See `RELEASE.md` for full release workflow:
1. Agent prepares release on `develop` branch
2. Human confirms version and release notes
3. Merge `develop` → `main` + create tag
4. Create GitHub Release
5. Rollback: use `git revert` only (never force push)

## Known Limitations

- WiFi.scanNetworks() is blocking (~2-5 seconds)
- Bandwidth detection may not work on all ESP32 variants
- Maximum 50 networks stored due to memory constraints

## Development Notes

- AP SSID is hardcoded as "ESP32-Scanner" (no password)
- Web server runs on port 80
- Results sorted by signal strength (strongest first)
- JSON API endpoint: `/scan` returns scan results
