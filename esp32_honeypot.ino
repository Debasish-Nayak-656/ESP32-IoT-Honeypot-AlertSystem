/*
 * ============================================================
 *  ESP32 IoT Cybersecurity Honeypot
 *  Project: ESP32 IoT Honeypot with Arduino Uno Alert System
 *  Author:  Debasish Nayak  |  GitHub: Debasish-Nayak-656
 *  Board:   ESP32 Dev Module
 *  Purpose: Simulates a vulnerable IoT camera to attract, log,
 *           and analyze unauthorized connection attempts.
 * ============================================================
 *  WIRING (ESP32 → Arduino Uno)
 *    ESP32 TX2 (GPIO17)  →  Arduino RX (Pin 0) via voltage divider
 *    ESP32 GND           →  Arduino GND
 *  NOTE: ESP32 is 3.3V logic; Arduino RX is 5V tolerant via divider
 * ============================================================
 */

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include "honeypot_pages.h"   // HTML page builders

// ─── Access Point Configuration ─────────────────────────────
const char* AP_SSID     = "SmartCamera-IoT";   // Bait SSID
const char* AP_PASSWORD = "";                   // Open network
IPAddress   AP_IP(192, 168, 4, 1);
IPAddress   AP_SUBNET(255, 255, 255, 0);

// ─── Serial2 to Arduino (TX2=GPIO17, RX2=GPIO16) ────────────
#define ARDUINO_SERIAL  Serial2
#define ARDUINO_BAUD    9600

// ─── Web + DNS Servers ───────────────────────────────────────
WebServer server(80);
DNSServer dnsServer;
const byte DNS_PORT = 53;

// ─── Log Data Structures ─────────────────────────────────────
#define MAX_LOG   50
#define MAX_IPS   30

struct LogEntry {
  String ip, url, ua, ts;
  int    requests, score;
  String level;
};

struct IPTracker {
  String ip;
  int    requestCount, threatScore;
  bool   visitedAdmin, visitedLogin, visitedCamera, visitedSettings;
};

LogEntry  attackLog[MAX_LOG];
IPTracker ipTrack[MAX_IPS];
int logHead = 0, totalLogs = 0, trackedCount = 0;

String   currentLevel = "LOW";
int      globalScore  = 0;
unsigned long lastAlertMs = 0;
const    unsigned long ALERT_GAP = 3000;

// ─── Prototypes ───────────────────────────────────────────────
void     logRequest(String url);
int      updateScore(String ip, String url);
String   levelFor(int score);
IPTracker* getTracker(String ip);
void     alertArduino(String level);
String   uptime();

// ════════════════════════════════════════════════════════════
void setup() {
  Serial.begin(115200);
  ARDUINO_SERIAL.begin(ARDUINO_BAUD, SERIAL_8N1, 16, 17);

  Serial.println("\n[BOOT] ESP32 IoT Honeypot starting...");

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(AP_IP, AP_IP, AP_SUBNET);
  WiFi.softAP(AP_SSID, AP_PASSWORD);
  Serial.printf("[AP]   SSID: %s  IP: %s\n", AP_SSID,
                WiFi.softAPIP().toString().c_str());

  dnsServer.start(DNS_PORT, "*", AP_IP);

  // ── Route definitions ────────────────────────────────────
  server.on("/",               HTTP_GET,  [](){logRequest("/");        server.send(200,"text/html",buildHomePage());});
  server.on("/login",          HTTP_GET,  [](){logRequest("/login");   server.send(200,"text/html",buildLoginPage(false));});
  server.on("/login",          HTTP_POST, [](){
    String u=server.arg("username"), p=server.arg("password");
    Serial.printf("[CRED] IP:%-15s user=%s pass=%s\n",
      server.client().remoteIP().toString().c_str(),u.c_str(),p.c_str());
    logRequest("/login");
    server.send(200,"text/html",buildLoginPage(true));
  });
  server.on("/admin",          HTTP_GET,  [](){logRequest("/admin");   server.send(200,"text/html",buildAdminPage());});
  server.on("/camera",         HTTP_GET,  [](){logRequest("/camera");  server.send(200,"text/html",buildCameraPage());});
  server.on("/settings",       HTTP_GET,  [](){logRequest("/settings");server.send(200,"text/html",buildSettingsPage());});
  server.on("/dashboard",      HTTP_GET,  [](){server.send(200,"text/html",buildDashboard());});
  server.on("/api/logs",       HTTP_GET,  handleAPILogs);
  server.on("/api/stats",      HTTP_GET,  handleAPIStats);
  server.onNotFound(           [](){logRequest(server.uri()); server.send(404,"text/html",buildLoginPage(false));});

  server.begin();
  Serial.println("[WEB]  HTTP server on port 80");
  Serial.println("[SYS]  Honeypot ACTIVE\n");
  alertArduino("LOW");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}

// ════════════════════════════════════════════════════════════
//  API HANDLERS
// ════════════════════════════════════════════════════════════

void handleAPILogs() {
  String j = "[";
  int count = min(totalLogs, MAX_LOG);
  for (int i = 0; i < count; i++) {
    int idx = (totalLogs > MAX_LOG)
              ? (logHead + i) % MAX_LOG
              : i;
    if (i > 0) j += ",";
    j += "{\"ip\":\""+attackLog[idx].ip+"\","
         "\"url\":\""+attackLog[idx].url+"\","
         "\"ts\":\""+attackLog[idx].ts+"\","
         "\"ua\":\""+attackLog[idx].ua+"\","
         "\"requests\":"+attackLog[idx].requests+","
         "\"score\":"+attackLog[idx].score+","
         "\"level\":\""+attackLog[idx].level+"\"}";
  }
  j += "]";
  server.sendHeader("Access-Control-Allow-Origin","*");
  server.send(200,"application/json",j);
}

void handleAPIStats() {
  String j = "{\"total\":"+String(totalLogs)+","
             "\"active\":"+String(WiFi.softAPgetStationNum())+","
             "\"level\":\""+currentLevel+"\","
             "\"score\":"+String(globalScore)+","
             "\"tracked\":"+String(trackedCount)+"}";
  server.sendHeader("Access-Control-Allow-Origin","*");
  server.send(200,"application/json",j);
}

// ════════════════════════════════════════════════════════════
//  THREAT ENGINE
// ════════════════════════════════════════════════════════════

void logRequest(String url) {
  String ip = server.client().remoteIP().toString();
  String ua = server.header("User-Agent");
  if (!ua.length()) ua = "Unknown";

  int score    = updateScore(ip, url);
  String level = levelFor(score);
  if (score > globalScore) { globalScore = score; currentLevel = level; }

  attackLog[logHead] = {ip, url, ua.substring(0,60), uptime(),
                        getTracker(ip)->requestCount, score, level};
  logHead = (logHead + 1) % MAX_LOG;
  totalLogs++;

  Serial.printf("[LOG] %-15s %-12s score=%d %s\n",
                ip.c_str(), url.c_str(), score, level.c_str());

  if (millis() - lastAlertMs > ALERT_GAP) {
    alertArduino(currentLevel);
    lastAlertMs = millis();
  }
}

int updateScore(String ip, String url) {
  IPTracker* t = getTracker(ip);
  t->requestCount++;

  int pts = 0;
  if (url=="/" || url=="/index.html") pts += 10;
  if (url=="/login")                  pts += 20;
  if (url=="/admin")                  pts += 30;
  if (url=="/camera")                 pts += 15;
  if (url=="/settings")               pts += 15;
  if (t->requestCount > 5)            pts += 30;

  if (url=="/admin")    t->visitedAdmin    = true;
  if (url=="/login")    t->visitedLogin    = true;
  if (url=="/camera")   t->visitedCamera   = true;
  if (url=="/settings") t->visitedSettings = true;

  int sens = t->visitedAdmin + t->visitedLogin +
             t->visitedCamera + t->visitedSettings;
  if (sens >= 3) pts += 50;

  t->threatScore += pts;
  return t->threatScore;
}

String levelFor(int s) {
  if (s >= 150) return "CRITICAL";
  if (s >= 80)  return "HIGH";
  if (s >= 40)  return "MEDIUM";
  return "LOW";
}

IPTracker* getTracker(String ip) {
  for (int i = 0; i < trackedCount; i++)
    if (ipTrack[i].ip == ip) return &ipTrack[i];
  if (trackedCount < MAX_IPS)
    ipTrack[trackedCount] = {ip,0,0,false,false,false,false};
  else
    ipTrack[0] = {ip,0,0,false,false,false,false};
  return &ipTrack[trackedCount < MAX_IPS ? trackedCount++ : 0];
}

// ════════════════════════════════════════════════════════════
//  SERIAL ALERT TO ARDUINO
// ════════════════════════════════════════════════════════════
// Protocol: single ASCII char terminated by newline
//   'L' = LOW    'M' = MEDIUM    'H' = HIGH    'C' = CRITICAL

void alertArduino(String level) {
  char cmd = 'L';
  if (level=="MEDIUM")   cmd='M';
  else if (level=="HIGH")     cmd='H';
  else if (level=="CRITICAL") cmd='C';
  ARDUINO_SERIAL.println(cmd);
  Serial.printf("[UART] Sent '%c' (%s) to Arduino\n", cmd, level.c_str());
}

String uptime() {
  unsigned long s = millis()/1000;
  char buf[10];
  snprintf(buf,sizeof(buf),"%02lu:%02lu:%02lu",(s/3600)%24,(s/60)%60,s%60);
  return String(buf);
}
