#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 WiFi Scanner</title>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial, sans-serif; max-width: 800px; margin: 0 auto; padding: 20px; background-color: #f5f5f5; }
    h1 { text-align: center; color: #333; }
    .button-container { text-align: center; margin: 20px 0; }
    button { background-color: #4CAF50; border: none; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; cursor: pointer; border-radius: 4px; }
    button:hover { background-color: #45a049; }
    button:disabled { background-color: #cccccc; cursor: not-allowed; }
    table { width: 100%; border-collapse: collapse; margin-top: 20px; background-color: white; }
    th, td { padding: 12px; text-align: left; border-bottom: 1px solid #ddd; }
    th { background-color: #4CAF50; color: white; }
    tr:hover { background-color: #f5f5f5; }
    .status { text-align: center; margin: 10px 0; padding: 10px; border-radius: 4px; }
    .status.scanning { background-color: #fff3cd; color: #856404; }
    .status.done { background-color: #d4edda; color: #155724; }
  </style>
</head>
<body>
  <h1>ESP32 WiFi Scanner</h1>
  <div class="button-container">
    <button id="scanBtn" onclick="startScan()">Scan WiFi Networks</button>
  </div>
  <div id="status" class="status" style="display: none;"></div>
  <table id="resultsTable" style="display: none;">
    <thead>
      <tr>
        <th>SSID</th>
        <th>Signal (dBm)</th>
        <th>Encryption</th>
        <th>Channel</th>
      </tr>
    </thead>
    <tbody id="resultsBody"></tbody>
  </table>
  <script>
    function startScan() {
      var btn = document.getElementById('scanBtn');
      var status = document.getElementById('status');
      var table = document.getElementById('resultsTable');
      var tbody = document.getElementById('resultsBody');
      btn.disabled = true;
      btn.textContent = 'Scanning...';
      status.style.display = 'block';
      status.className = 'status scanning';
      status.textContent = 'Scanning for WiFi networks...';
      table.style.display = 'none';
      tbody.innerHTML = '';
      fetch('/scan')
        .then(response => response.json())
        .then(data => {
          status.className = 'status done';
          status.textContent = 'Found ' + data.networks.length + ' networks';
          if (data.networks.length > 0) {
            var html = '';
            data.networks.forEach(function(network) {
              html += '<tr>';
              html += '<td>' + network.ssid + '</td>';
              html += '<td>' + network.rssi + '</td>';
              html += '<td>' + network.encryption + '</td>';
              html += '<td>' + network.channel + '</td>';
              html += '</tr>';
            });
            tbody.innerHTML = html;
            table.style.display = 'table';
          }
          btn.disabled = false;
          btn.textContent = 'Scan WiFi Networks';
        })
        .catch(function(error) {
          status.textContent = 'Error: ' + error;
          btn.disabled = false;
          btn.textContent = 'Scan WiFi Networks';
        });
    }
  </script>
</body>
</html>
)rawliteral";

String getEncryptionType(wifi_auth_mode_t encryption) {
  switch (encryption) {
    case WIFI_AUTH_OPEN: return "Open";
    case WIFI_AUTH_WEP: return "WEP";
    case WIFI_AUTH_WPA_PSK: return "WPA";
    case WIFI_AUTH_WPA2_PSK: return "WPA2";
    case WIFI_AUTH_WPA_WPA2_PSK: return "WPA/WPA2";
    case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2-Enterprise";
    case WIFI_AUTH_WPA3_PSK: return "WPA3";
    default: return "Unknown";
  }
}

void handleRoot() {
  server.send(200, "text/html", index_html);
}

void handleScan() {
  Serial.println("Starting WiFi scan...");
  int n = WiFi.scanNetworks(false, true);
  
  struct NetworkInfo {
    String ssid;
    int rssi;
    String encryption;
    int channel;
  };
  
  NetworkInfo networks[50];
  int networkCount = 0;
  
  for (int i = 0; i < n && i < 50; i++) {
    networks[i].ssid = WiFi.SSID(i);
    networks[i].rssi = WiFi.RSSI(i);
    networks[i].encryption = getEncryptionType(WiFi.encryptionType(i));
    networks[i].channel = WiFi.channel(i);
    networkCount++;
  }
  
  for (int i = 0; i < networkCount - 1; i++) {
    for (int j = 0; j < networkCount - i - 1; j++) {
      if (networks[j].rssi < networks[j + 1].rssi) {
        NetworkInfo temp = networks[j];
        networks[j] = networks[j + 1];
        networks[j + 1] = temp;
      }
    }
  }
  
  String json = "{\"networks\":[";
  for (int i = 0; i < networkCount; i++) {
    if (i > 0) json += ",";
    json += "{\"ssid\":\"" + networks[i].ssid + "\",";
    json += "\"rssi\":" + String(networks[i].rssi) + ",";
    json += "\"encryption\":\"" + networks[i].encryption + "\",";
    json += "\"channel\":" + String(networks[i].channel) + "}";
  }
  json += "]}";
  
  Serial.print("Scan complete. Found ");
  Serial.print(n);
  Serial.println(" networks");
  
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Booting ESP32...");
  
  delay(500);
  Serial.println("Setting WiFi mode...");
  WiFi.mode(WIFI_AP);
  delay(500);
  
  Serial.println("Starting AP...");
  bool apResult = WiFi.softAP("ESP32-Scanner");
  Serial.print("AP started: ");
  Serial.println(apResult ? "OK" : "FAILED");
  
  delay(1000);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP: ");
  Serial.println(IP);
  
  Serial.println("Starting web server...");
  server.on("/", handleRoot);
  server.on("/scan", handleScan);
  server.begin();
  Serial.println("Web server ready!");
}

void loop() {
  server.handleClient();
  delay(1);
}
