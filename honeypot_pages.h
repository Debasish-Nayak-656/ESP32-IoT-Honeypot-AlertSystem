/*
 * honeypot_pages.h
 * HTML page builders for the ESP32 IoT Honeypot
 * All pages mimic a "SmartCamera Pro" IoT device UI
 */

#pragma once
#include <Arduino.h>

// ─── Shared CSS & Head ────────────────────────────────────────
static const String PAGE_CSS = R"(
<style>
*{margin:0;padding:0;box-sizing:border-box;}
body{background:#0f1117;color:#c9d1d9;font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',sans-serif;
     min-height:100vh;display:flex;flex-direction:column;}
.topbar{background:#161b22;border-bottom:1px solid #30363d;
        padding:12px 20px;display:flex;align-items:center;gap:10px;}
.brand{color:#58a6ff;font-weight:700;font-size:1.1rem;letter-spacing:1px;}
.brand-icon{width:28px;height:28px;background:#1f6feb;border-radius:6px;
            display:flex;align-items:center;justify-content:center;font-size:14px;}
.nav{margin-left:auto;display:flex;gap:20px;}
.nav a{color:#8b949e;font-size:.85rem;text-decoration:none;padding:4px 8px;
       border-radius:4px;transition:.2s;}
.nav a:hover{color:#c9d1d9;background:#21262d;}
.container{max-width:900px;margin:40px auto;padding:0 20px;width:100%;}
.card{background:#161b22;border:1px solid #30363d;border-radius:10px;
      padding:28px;margin-bottom:20px;}
.card-title{font-size:1rem;font-weight:600;color:#e6edf3;margin-bottom:18px;
            padding-bottom:12px;border-bottom:1px solid #21262d;}
label{display:block;font-size:.83rem;color:#8b949e;margin-bottom:6px;}
input{width:100%;background:#0d1117;border:1px solid #30363d;color:#c9d1d9;
      padding:10px 14px;border-radius:6px;font-size:.9rem;margin-bottom:14px;
      outline:none;transition:.2s;}
input:focus{border-color:#58a6ff;box-shadow:0 0 0 3px #1f6feb33;}
.btn{background:#238636;color:#fff;border:none;padding:10px 20px;
     border-radius:6px;font-size:.9rem;font-weight:600;cursor:pointer;
     width:100%;transition:.2s;}
.btn:hover{background:#2ea043;}
.btn-secondary{background:#21262d;border:1px solid #30363d;}
.btn-secondary:hover{background:#30363d;}
.alert{padding:10px 14px;border-radius:6px;font-size:.83rem;margin-bottom:14px;}
.alert-err{background:#3d1f1f;border:1px solid #f85149;color:#f85149;}
.status-row{display:flex;gap:10px;margin-bottom:14px;flex-wrap:wrap;}
.badge{padding:4px 10px;border-radius:20px;font-size:.75rem;font-weight:600;}
.badge-green{background:#14532d;color:#4ade80;}
.badge-blue{background:#1e3a5f;color:#60a5fa;}
.badge-yellow{background:#422006;color:#fbbf24;}
.info-grid{display:grid;grid-template-columns:1fr 1fr;gap:12px;}
.info-item{background:#0d1117;border:1px solid #21262d;border-radius:6px;padding:12px;}
.info-item .key{font-size:.75rem;color:#8b949e;margin-bottom:4px;}
.info-item .val{font-size:.9rem;color:#e6edf3;font-weight:600;}
footer{margin-top:auto;text-align:center;padding:16px;color:#484f58;font-size:.75rem;
       border-top:1px solid #21262d;}
</style>
)";

static const String buildNav(String active) {
  return "<nav class='nav'>"
         "<a href='/'>Home</a>"
         "<a href='/camera'>Camera</a>"
         "<a href='/settings'>Settings</a>"
         "<a href='/admin'>Admin</a>"
         "</nav>";
}

// ─── Home Page ─────────────────────────────────────────────────
String buildHomePage() {
  return "<!DOCTYPE html><html><head><meta charset='UTF-8'>"
         "<meta name='viewport' content='width=device-width,initial-scale=1'>"
         "<title>SmartCamera Pro</title>" + PAGE_CSS + "</head><body>"
         "<div class='topbar'><div class='brand-icon'>📷</div>"
         "<span class='brand'>SmartCamera Pro</span>" + buildNav("home") + "</div>"
         "<div class='container'>"
         "<div class='card'>"
         "<div class='card-title'>Device Status</div>"
         "<div class='status-row'>"
         "<span class='badge badge-green'>● Online</span>"
         "<span class='badge badge-blue'>HD 1080p</span>"
         "<span class='badge badge-yellow'>Motion Detected</span></div>"
         "<div class='info-grid'>"
         "<div class='info-item'><div class='key'>Device Model</div>"
         "<div class='val'>SCPro-4K v2.1</div></div>"
         "<div class='info-item'><div class='key'>Firmware</div>"
         "<div class='val'>v3.8.2-stable</div></div>"
         "<div class='info-item'><div class='key'>IP Address</div>"
         "<div class='val'>192.168.4.1</div></div>"
         "<div class='info-item'><div class='key'>Uptime</div>"
         "<div class='val'>14d 6h 22m</div></div>"
         "<div class='info-item'><div class='key'>Storage</div>"
         "<div class='val'>128 GB (62% used)</div></div>"
         "<div class='info-item'><div class='key'>Last Motion</div>"
         "<div class='val'>2 minutes ago</div></div>"
         "</div></div>"
         "<div class='card'>"
         "<div class='card-title'>Quick Access</div>"
         "<div class='info-grid'>"
         "<a href='/camera' style='text-decoration:none'>"
         "<div class='info-item' style='cursor:pointer;border-color:#30363d'>"
         "<div class='key'>📹 Live Feed</div><div class='val'>View Camera</div></div></a>"
         "<a href='/login' style='text-decoration:none'>"
         "<div class='info-item' style='cursor:pointer'>"
         "<div class='key'>🔐 Account</div><div class='val'>Login / Manage</div></div></a>"
         "<a href='/settings' style='text-decoration:none'>"
         "<div class='info-item' style='cursor:pointer'>"
         "<div class='key'>⚙ Settings</div><div class='val'>Configure</div></div></a>"
         "<a href='/admin' style='text-decoration:none'>"
         "<div class='info-item' style='cursor:pointer'>"
         "<div class='key'>🛡 Admin Panel</div><div class='val'>System Admin</div></div></a>"
         "</div></div></div>"
         "<footer>SmartCamera Pro &copy; 2024 &nbsp;|&nbsp; Firmware v3.8.2</footer>"
         "</body></html>";
}

// ─── Login Page ─────────────────────────────────────────────────
String buildLoginPage(bool showError) {
  String errHtml = showError ?
    "<div class='alert alert-err'>❌ Invalid credentials. Access denied.</div>" : "";
  return "<!DOCTYPE html><html><head><meta charset='UTF-8'>"
         "<meta name='viewport' content='width=device-width,initial-scale=1'>"
         "<title>SmartCamera — Login</title>" + PAGE_CSS + "</head><body>"
         "<div class='topbar'><div class='brand-icon'>📷</div>"
         "<span class='brand'>SmartCamera Pro</span>" + buildNav("login") + "</div>"
         "<div class='container' style='max-width:420px'>"
         "<div class='card'>"
         "<div class='card-title'>🔐 Device Login</div>"
         + errHtml +
         "<form method='POST' action='/login'>"
         "<label>Username</label>"
         "<input type='text' name='username' placeholder='admin' autocomplete='username'>"
         "<label>Password</label>"
         "<input type='password' name='password' placeholder='••••••••' autocomplete='current-password'>"
         "<button class='btn' type='submit'>Sign In</button>"
         "</form>"
         "<p style='margin-top:14px;font-size:.78rem;color:#8b949e;text-align:center'>"
         "Default: admin / admin123</p>"
         "</div></div>"
         "<footer>SmartCamera Pro &copy; 2024</footer>"
         "</body></html>";
}

// ─── Admin Page ─────────────────────────────────────────────────
String buildAdminPage() {
  return "<!DOCTYPE html><html><head><meta charset='UTF-8'>"
         "<meta name='viewport' content='width=device-width,initial-scale=1'>"
         "<title>SmartCamera — Admin</title>" + PAGE_CSS + "</head><body>"
         "<div class='topbar'><div class='brand-icon'>📷</div>"
         "<span class='brand'>SmartCamera Pro</span>" + buildNav("admin") + "</div>"
         "<div class='container'>"
         "<div class='card'>"
         "<div class='card-title'>🛡 Administration Panel</div>"
         "<div class='alert' style='background:#1c2b3a;border:1px solid #58a6ff;color:#58a6ff;margin-bottom:18px'>"
         "⚠ Session active. Administrator privileges granted.</div>"
         "<div class='info-grid'>"
         "<div class='info-item'><div class='key'>CPU Usage</div><div class='val'>12%</div></div>"
         "<div class='info-item'><div class='key'>RAM</div><div class='val'>48 MB / 128 MB</div></div>"
         "<div class='info-item'><div class='key'>API Key</div><div class='val'>sk-cam-XXXXX</div></div>"
         "<div class='info-item'><div class='key'>RTSP Stream</div><div class='val'>Port 554</div></div>"
         "<div class='info-item'><div class='key'>SSH Access</div><div class='val'>Port 22 Open</div></div>"
         "<div class='info-item'><div class='key'>Telnet</div><div class='val'>Port 23 Open</div></div>"
         "</div></div>"
         "<div class='card'>"
         "<div class='card-title'>System Actions</div>"
         "<div style='display:flex;gap:10px;flex-wrap:wrap'>"
         "<button class='btn btn-secondary' style='width:auto'>Restart Device</button>"
         "<button class='btn btn-secondary' style='width:auto'>Reset to Factory</button>"
         "<button class='btn btn-secondary' style='width:auto'>Export Logs</button>"
         "<button class='btn btn-secondary' style='width:auto'>Update Firmware</button>"
         "</div></div></div>"
         "<footer>SmartCamera Pro &copy; 2024</footer>"
         "</body></html>";
}

// ─── Camera Page ─────────────────────────────────────────────────
String buildCameraPage() {
  return "<!DOCTYPE html><html><head><meta charset='UTF-8'>"
         "<meta name='viewport' content='width=device-width,initial-scale=1'>"
         "<title>SmartCamera — Live Feed</title>" + PAGE_CSS + R"(
<style>
.cam-placeholder{background:#000;border:1px solid #30363d;border-radius:8px;
  height:240px;display:flex;flex-direction:column;align-items:center;
  justify-content:center;color:#484f58;gap:8px;margin-bottom:14px;}
.rec-dot{width:10px;height:10px;border-radius:50%;background:#f85149;
  animation:blink 1s infinite;}
@keyframes blink{0%,100%{opacity:1}50%{opacity:0}}
.cam-bar{display:flex;align-items:center;justify-content:space-between;
  font-size:.8rem;color:#8b949e;padding:8px 0;}
</style>
)" + "</head><body>"
         "<div class='topbar'><div class='brand-icon'>📷</div>"
         "<span class='brand'>SmartCamera Pro</span>" + buildNav("camera") + "</div>"
         "<div class='container'>"
         "<div class='card'>"
         "<div class='card-title'>📹 Live Camera Feed</div>"
         "<div class='cam-placeholder'>"
         "<span style='font-size:3rem'>📷</span>"
         "<span>LIVE FEED</span>"
         "<div style='display:flex;align-items:center;gap:6px'>"
         "<div class='rec-dot'></div><span style='color:#f85149;font-size:.8rem'>REC</span></div>"
         "</div>"
         "<div class='cam-bar'>"
         "<span>Channel: Front Door</span>"
         "<span>1080p HD &nbsp;|&nbsp; 30fps</span>"
         "<span>IR Night Vision: ON</span></div>"
         "<div style='display:flex;gap:10px;margin-top:10px;flex-wrap:wrap'>"
         "<button class='btn btn-secondary' style='width:auto;font-size:.8rem'>⏺ Record</button>"
         "<button class='btn btn-secondary' style='width:auto;font-size:.8rem'>📸 Snapshot</button>"
         "<button class='btn btn-secondary' style='width:auto;font-size:.8rem'>🔊 Audio</button>"
         "<button class='btn btn-secondary' style='width:auto;font-size:.8rem'>↔ PTZ Control</button>"
         "</div></div></div>"
         "<footer>SmartCamera Pro &copy; 2024</footer>"
         "</body></html>";
}

// ─── Settings Page ─────────────────────────────────────────────────
String buildSettingsPage() {
  return "<!DOCTYPE html><html><head><meta charset='UTF-8'>"
         "<meta name='viewport' content='width=device-width,initial-scale=1'>"
         "<title>SmartCamera — Settings</title>" + PAGE_CSS + "</head><body>"
         "<div class='topbar'><div class='brand-icon'>📷</div>"
         "<span class='brand'>SmartCamera Pro</span>" + buildNav("settings") + "</div>"
         "<div class='container'>"
         "<div class='card'>"
         "<div class='card-title'>⚙ Network Settings</div>"
         "<label>Device Name</label>"
         "<input type='text' value='SmartCamera-IoT'>"
         "<label>SSID</label>"
         "<input type='text' value='HomeNetwork_5G'>"
         "<label>Password</label>"
         "<input type='password' value='supersecret123'>"
         "<button class='btn' style='width:auto'>Save Network</button></div>"
         "<div class='card'>"
         "<div class='card-title'>🔔 Alert Settings</div>"
         "<label>Alert Email</label>"
         "<input type='email' value='admin@home.com'>"
         "<label>Motion Sensitivity (1-10)</label>"
         "<input type='number' value='7' min='1' max='10'>"
         "<button class='btn' style='width:auto'>Save Alerts</button></div>"
         "</div>"
         "<footer>SmartCamera Pro &copy; 2024</footer>"
         "</body></html>";
}

// ─── Honeypot Dashboard ─────────────────────────────────────────────────
String buildDashboard() {
  return R"(<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Honeypot Command Center</title>
<style>
:root{--bg:#060b14;--card:#0d1524;--border:#1a2840;--accent:#00d4ff;
      --red:#ff3b5c;--yellow:#ffb703;--green:#06d6a0;--purple:#7c3aed;
      --text:#cdd6f4;--muted:#6c7a96;}
*{margin:0;padding:0;box-sizing:border-box;}
body{background:var(--bg);color:var(--text);
     font-family:'Courier New',Consolas,monospace;min-height:100vh;}
.topbar{background:var(--card);border-bottom:2px solid var(--accent);
  padding:12px 24px;display:flex;align-items:center;gap:12px;}
.logo{color:var(--accent);font-size:1.15rem;font-weight:700;letter-spacing:3px;}
.live{display:flex;align-items:center;gap:6px;margin-left:auto;font-size:.78rem;color:var(--muted);}
.dot{width:8px;height:8px;border-radius:50%;background:var(--green);
     box-shadow:0 0 8px var(--green);animation:blink 1.5s infinite;}
@keyframes blink{0%,100%{opacity:1}50%{opacity:.2}}
#bar{height:3px;background:linear-gradient(90deg,var(--accent),var(--purple));
     animation:shrink 5s linear infinite;}
@keyframes shrink{from{width:100%}to{width:0%}}
.grid4{display:grid;grid-template-columns:repeat(4,1fr);gap:14px;
       padding:20px 24px;}
@media(max-width:700px){.grid4{grid-template-columns:1fr 1fr;}}
.stat{background:var(--card);border:1px solid var(--border);border-radius:8px;
      padding:18px;position:relative;overflow:hidden;}
.stat::after{content:'';position:absolute;bottom:0;left:0;right:0;height:1px;}
.stat.c1::after{background:var(--accent);}
.stat.c2::after{background:var(--yellow);}
.stat.c3::after{background:var(--red);}
.stat.c4::after{background:var(--green);}
.slabel{font-size:.68rem;color:var(--muted);letter-spacing:1.5px;
        text-transform:uppercase;margin-bottom:8px;}
.sval{font-size:1.9rem;font-weight:700;}
.c1 .sval{color:var(--accent);}
.c2 .sval{color:var(--yellow);}
.c3 .sval{color:var(--red);}
.c4 .sval{color:var(--green);}
.panel{padding:0 24px 24px;}
.ptitle{font-size:.75rem;color:var(--muted);letter-spacing:2px;
  text-transform:uppercase;margin-bottom:12px;padding-bottom:8px;
  border-bottom:1px solid var(--border);}
table{width:100%;border-collapse:collapse;font-size:.78rem;}
th{background:#0a1220;color:var(--muted);font-weight:400;
   padding:9px 12px;text-align:left;border-bottom:1px solid var(--border);}
td{padding:9px 12px;border-bottom:1px solid #0f1c2e;vertical-align:middle;}
tr:hover td{background:#0d1a30;}
.ip{color:var(--accent);}
.url-cell{color:#94a3b8;}
.tbadge{padding:3px 9px;border-radius:3px;font-size:.7rem;font-weight:700;
        letter-spacing:.5px;}
.LOW{background:#05200f;color:var(--green);border:1px solid var(--green);}
.MEDIUM{background:#211500;color:var(--yellow);border:1px solid var(--yellow);}
.HIGH{background:#200010;color:var(--red);border:1px solid var(--red);}
.CRITICAL{background:#1a0030;color:#c084fc;border:1px solid #c084fc;
          animation:blink .7s infinite;}
.foot{text-align:center;padding:14px;color:var(--muted);font-size:.72rem;
      border-top:1px solid var(--border);}
</style>
</head>
<body>
<div id="bar"></div>
<div class="topbar">
  <span class="logo">⚠ HONEYPOT COMMAND CENTER</span>
  <span class="live"><span class="dot"></span> LIVE &nbsp;|&nbsp; Auto-refresh 5s</span>
</div>

<div class="grid4">
  <div class="stat c1"><div class="slabel">Total Visitors</div>
    <div class="sval" id="sTotal">—</div></div>
  <div class="stat c2"><div class="slabel">Active Stations</div>
    <div class="sval" id="sActive">—</div></div>
  <div class="stat c3"><div class="slabel">Threat Level</div>
    <div class="sval" id="sLevel" style="font-size:1.1rem;margin-top:4px">—</div></div>
  <div class="stat c4"><div class="slabel">Tracked IPs</div>
    <div class="sval" id="sTracked">—</div></div>
</div>

<div class="panel">
  <div class="ptitle">⚡ Attack Log</div>
  <table>
    <thead><tr>
      <th>IP Address</th><th>URL</th><th>Time</th>
      <th>Reqs</th><th>Score</th><th>Level</th><th>User-Agent</th>
    </tr></thead>
    <tbody id="logBody">
      <tr><td colspan="7" style="text-align:center;color:var(--muted);padding:30px">
        Waiting for attackers...</td></tr>
    </tbody>
  </table>
</div>

<div class="foot">
  ESP32 IoT Honeypot &nbsp;·&nbsp; Educational Use Only &nbsp;·&nbsp;
  Debasish Nayak &nbsp;·&nbsp; <span id="clk"></span>
</div>

<script>
async function load(){
  try{
    const [sr,lr]=await Promise.all([fetch('/api/stats'),fetch('/api/logs')]);
    const s=await sr.json(), logs=await lr.json();
    document.getElementById('sTotal').textContent=s.total;
    document.getElementById('sActive').textContent=s.active;
    document.getElementById('sTracked').textContent=s.tracked;
    const el=document.getElementById('sLevel');
    el.textContent=s.level;
    el.className='sval tbadge '+s.level;

    const tb=document.getElementById('logBody');
    if(!logs.length){
      tb.innerHTML='<tr><td colspan="7" style="text-align:center;color:var(--muted);padding:30px">No entries yet.</td></tr>';
      return;
    }
    tb.innerHTML=[...logs].reverse().slice(0,30).map(e=>`
      <tr>
        <td class="ip">${e.ip}</td>
        <td class="url-cell">${e.url}</td>
        <td style="color:var(--muted)">${e.ts}</td>
        <td>${e.requests}</td>
        <td>${e.score}</td>
        <td><span class="tbadge ${e.level}">${e.level}</span></td>
        <td style="color:var(--muted);max-width:200px;overflow:hidden;white-space:nowrap;text-overflow:ellipsis">${e.ua}</td>
      </tr>`).join('');
  }catch(e){console.error(e);}
}
function tick(){document.getElementById('clk').textContent=new Date().toLocaleTimeString('en-GB');}
load(); setInterval(load,5000); setInterval(tick,1000);
</script>
</body></html>)";
}
