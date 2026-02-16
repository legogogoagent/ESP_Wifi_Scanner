# KNOWLEDGE - 项目经验总结

本文档记录项目开发过程中遇到的坑和解决方案，供未来参考。

---

## 环境搭建

### GitHub 访问

**问题**: GitHub 连接超时或无法访问

**解决**: 配置 HTTP 代理

```bash
export HTTP_PROXY=http://100.65.220.56:5228
export HTTPS_PROXY=http://100.65.220.56:5228

# Git 单独配置
git config --local http.proxy http://100.65.220.56:5228
git config --local https.proxy http://100.65.220.56:5228

# GitHub CLI 配置
gh config set http_proxy http://100.65.220.56:5228
```

---

## 开发

### PlatformIO

**问题**: ESP32 开发需要特定框架和库

**解决**: 使用 platformio.ini 管理依赖

```ini
[env:esp32-c3-devkitm-1]
platform = espressif32
board = esp32-c3-devkitm-1
framework = arduino
lib_deps =
    https://github.com/me-no-dev/ESPAsyncWebServer.git
    https://github.com/me-no-dev/AsyncTCP.git
```

### 内存优化

**问题**: ESP32 内存有限，HTML 字符串占用 RAM

**解决**: 使用 PROGMEM 将常量字符串存储在 Flash

```cpp
const char index_html[] PROGMEM = R"rawliteral(... )rawliteral";
```

### ESP32-C3 编译兼容性

**问题**: 使用 espressif32@5.4.0 或更高版本时，`WIFI_AUTH_WPA3_WPA2_PSK` 未定义

**解决**: 移除该枚举值，或使用条件编译

```cpp
// 移除不兼容的加密类型
case WIFI_AUTH_WPA3_PSK: return "WPA3";
// 删除: case WIFI_AUTH_WPA3_WPA2_PSK: return "WPA3/WPA2";
default: return "Unknown";
```

### Web服务器库选择

**问题**: ESPAsyncWebServer 在 ESP32-C3 上编译困难

**解决**: 使用 Arduino 内置的 WebServer 库（同步但更稳定）

```cpp
#include <WebServer.h>
WebServer server(80);
```

### ESP32 固件烧录

**问题**: 只烧录 firmware.bin 导致 ESP32 一直重启

**解决**: 必须烧录完整的三个文件

```bash
# 正确的烧录命令
esptool --chip esp32c3 --port /dev/ttyACM0 --baud 460800 write_flash \
  0x0 bootloader.bin \
  0x8000 partitions.bin \
  0x10000 firmware.bin
```

**GitHub Actions 上传配置**:
```yaml
- name: Upload firmware
  uses: actions/upload-artifact@v4
  with:
    name: firmware
    path: |
      .pio/build/esp32-c3-devkitm-1/bootloader.bin
      .pio/build/esp32-c3-devkitm-1/partitions.bin
      .pio/build/esp32-c3-devkitm-1/firmware.bin
```

---

## 部署

### CI/CD

**问题**: 未知项目类型导致 CI 失败

**解决**: scripts/ci.sh 自动检测项目类型并运行对应测试

```bash
# PlatformIO 项目
if file_exists "platformio.ini"; then
    pio run
fi
```

---

## 待补充

随着项目推进，持续添加新的经验记录。
