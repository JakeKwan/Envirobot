#include "WebSerialMonitor.h"

// Global definitions
const char* ssid = "ESP32-Data-Stream";
const char* password = "12345678";

AsyncWebServer server(80);
AsyncWebSocket ws("/serialws");

const int controlLedPin = 2;
const int controlPinA = 12;
const int controlPinB = 14;

void broadcastSerial(const String& message) {
  ws.cleanupClients();
  ws.textAll(message);
}

void setupControlPins() {
  pinMode(controlLedPin, OUTPUT);
  pinMode(controlPinA, OUTPUT);
  pinMode(controlPinB, OUTPUT);
  digitalWrite(controlLedPin, LOW);
  digitalWrite(controlPinA, LOW);
  digitalWrite(controlPinB, LOW);
}

void setControlState(bool active) {
  digitalWrite(controlLedPin, active ? HIGH : LOW);
  digitalWrite(controlPinA, active ? HIGH : LOW);
  digitalWrite(controlPinB, active ? LOW : HIGH);
}

void processWebCommand(const String& command) {
  if (command.startsWith("btn:")) {
    int delim = command.indexOf(':', 4);
    if (delim > 4) {
      String button = command.substring(4, delim);
      String value = command.substring(delim + 1);
      int buttonIndex = button.toInt();
      bool pressed = (value == "1");
    //   Serial.println("[WEB] BUTTON " + button + " = " + value);
      updateRoverControlFromButton(buttonIndex, pressed);
    //   updateGamepadButton(buttonIndex, pressed); 

      // Button functions
      if (buttonIndex == 0) {
        setControlState(pressed);
      }
      return;
    }
  }

  if (command.startsWith("axis:")) {
    int delim = command.indexOf(':', 5);
    if (delim > 5) {
      String axis = command.substring(5, delim);
      String value = command.substring(delim + 1);
      int axisIndex = axis.toInt();
      float normalized = value.toFloat();
    //   Serial.println("[WEB] AXIS " + axis + " = " + value);
      updateRoverControlFromAxis(axisIndex, normalized);
    //   updateGamepadAxis(axisIndex, normalized); 
      return;
    }
  }

  Serial.println("[WEB] " + command);
}

void onWsEvent(AsyncWebSocket* socket, AsyncWebSocketClient* client, AwsEventType type,
               void* arg, uint8_t* data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    client->text("Connected to ESP32 serial monitor\n");
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.println("Web client disconnected");
  } else if (type == WS_EVT_DATA) {
    AwsFrameInfo* info = (AwsFrameInfo*)arg;
    if (info->opcode == WS_TEXT && info->final && info->index == 0 && info->len == len) {
      char payload[len + 1];
      memcpy(payload, data, len);
      payload[len] = 0;
      String message = String(payload);
      message.trim();
      if (message.length() > 0) {
        processWebCommand(message);
        client->text(">>> " + message + "\n");
      }
    }
  }
}

void setup_serial() {
  Serial.begin(115200);
  setupControlPins();

  Serial.println("Setting up Access Point...");
  WiFi.softAP(ssid, password);

  IPAddress ip = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(ip);

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/html", R"HTML(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <title>ESP32 Serial Monitor</title>
    <style>
      body { font-family: Arial, sans-serif; background: #111; color: #eee; margin: 0; padding: 16px; }
      #log { background: #000; border: 1px solid #444; border-radius: 8px; padding: 12px; height: 40vh; overflow: auto; white-space: pre-wrap; font-family: Consolas, monospace; }
      #sensorViz { margin-top: 12px; }
      .row { display: flex; gap: 8px; margin-top: 12px; }
      input { flex: 1; padding: 8px; border-radius: 6px; border: 1px solid #666; }
      button { padding: 8px 12px; border-radius: 6px; border: 0; background: #2f80ed; color: white; cursor: pointer; }

      /* sensor viz styles */
      .sensor { display: flex; align-items: center; gap: 8px; margin-top: 8px; }
      .sensor .label { width: 90px; text-transform: capitalize; }
      .barWrap { flex: 1; background: #222; border-radius: 6px; padding: 4px; }
      .barInner { height: 16px; background: #2f80ed; width: 0%; border-radius: 4px; transition: width 120ms linear; }
      .value { width: 60px; text-align: right; font-family: Consolas, monospace; }
    </style>
  </head>
  <body>
    <h1>ESP32 Web Serial Monitor</h1>
    <div id="log">Waiting for serial data...</div>
    <div id="sensorViz"></div>
    <div id="gpStatus">Gamepad status: not connected</div>
    <div id="gpButtons"></div>
    <div id="gpAxes"></div>
    <div class="row">
      <input id="cmd" placeholder="Type a command and press Enter" />
      <button onclick="sendCommand()">Send</button>
    </div>
    <script>
      const log = document.getElementById('log');
      const input = document.getElementById('cmd');
      const status = document.getElementById('gpStatus');
      const buttons = document.getElementById('gpButtons');
      const axes = document.getElementById('gpAxes');
      const ws = new WebSocket(`ws://${location.host}/serialws`);
      let prevButtons = [];
      let prevAxes = [];
      let pollingTimer = null;
      let lastConnectedGamepadId = null;
      let lastConnectedState = false;

      function getConnectedGamepads() {
        const gamepads = navigator.getGamepads ? navigator.getGamepads() : [];
        if (!gamepads) return [];
        return Array.from(gamepads).filter((gp) => gp && gp.connected);
      }

      function startPolling() {
        if (pollingTimer) return;
        pollingTimer = setInterval(pollGamepad, 50);
      }

      function stopPolling() {
        if (pollingTimer) {
          clearInterval(pollingTimer);
          pollingTimer = null;
        }
      }

      ws.onopen = () => append('Connected to ESP32 serial monitor');
      ws.onmessage = (event) => { processSerial(event.data); append(event.data); };
      ws.onerror = () => append('WebSocket error');
      ws.onclose = () => append('Disconnected');

      function append(text) {
        log.textContent += text + (text.endsWith('\n') ? '' : '\n');
        log.scrollTop = log.scrollHeight;
      }

      function processSerial(raw) {
        // strip common prefixes and whitespace
        let text = raw.replace(/^\s*/, '').replace(/^<<<\s*/, '').replace(/^>>>\s*/, '').trim();
        // try to match simple sensor readings like "name:123" or "name: 123"
        const m = text.match(/([a-zA-Z0-9_]+)\s*:\s*(\d+)/);
        if (m) {
          const name = m[1];
          const value = parseInt(m[2], 10);
          updateSensor(name, value);
        }
      }

      function updateSensor(name, value) {
        const viz = document.getElementById('sensorViz');
        let el = document.getElementById('sensor-' + name);
        if (!el) {
          el = document.createElement('div');
          el.id = 'sensor-' + name;
          el.className = 'sensor';
          el.innerHTML = `<div class="label">${name}</div><div class="barWrap"><div class="barInner"></div></div><div class="value"></div>`;
          viz.appendChild(el);
        }
        const bar = el.querySelector('.barInner');
        const valueEl = el.querySelector('.value');
        const pct = Math.min(100, Math.round((value / 1023) * 100));
        bar.style.width = pct + '%';
        valueEl.textContent = value;
      }

      function sendCommand() {
        const value = input.value.trim();
        if (!value) return;
        ws.send(value);
        input.value = '';
      }

      input.addEventListener('keydown', (event) => {
        if (event.key === 'Enter') {
          event.preventDefault();
          sendCommand();
        }
      });

      window.addEventListener('gamepadconnected', (event) => {
        lastConnectedGamepadId = event.gamepad.id;
        lastConnectedState = true;
        status.textContent = `Gamepad connected: ${event.gamepad.id}`;
        startPolling();
      });

      window.addEventListener('gamepaddisconnected', () => {
        lastConnectedState = false;
        lastConnectedGamepadId = null;
        status.textContent = 'Gamepad disconnected. Reconnect and press any button.';
        buttons.textContent = '';
        axes.textContent = '';
        prevButtons = [];
        prevAxes = [];
        stopPolling();
      });

      function pollGamepad() {
        const gps = getConnectedGamepads();
        const gp = gps[0];

        if (!gp) {
          if (lastConnectedState) {
            lastConnectedState = false;
            status.textContent = 'Gamepad disconnected. Reconnect and press any button.';
            buttons.textContent = '';
            axes.textContent = '';
            prevButtons = [];
            prevAxes = [];
          }
          return;
        }

        if (!lastConnectedState || lastConnectedGamepadId !== gp.id) {
          lastConnectedState = true;
          lastConnectedGamepadId = gp.id;
        }

        status.textContent = `Gamepad: ${gp.id}`;

        const buttonStates = gp.buttons.map((btn) => btn.pressed ? 1 : 0);
        const axisStates = gp.axes.map((axis) => axis.toFixed(2));

        buttons.textContent = `Buttons: ${buttonStates.join(', ')}`;
        axes.textContent = `Axes: ${axisStates.join(', ')}`;

        if (prevButtons.length === 0) {
          prevButtons = buttonStates.slice();
        }
        if (prevAxes.length === 0) {
          prevAxes = axisStates.slice();
        }

        buttonStates.forEach((value, index) => {
          if (value !== prevButtons[index]) {
            ws.send(`btn:${index}:${value}`);
            prevButtons[index] = value;
          }
        });

        axisStates.forEach((value, index) => {
          if (value !== prevAxes[index]) {
            ws.send(`axis:${index}:${value}`);
            prevAxes[index] = value;
          }
        });
      }

      startPolling();
    </script>
  </body>
</html>
)HTML");
  });

  server.begin();
}

void handle_serial() {
  ws.cleanupClients();

  static String serialBuffer;
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\n') {
      if (serialBuffer.length() > 0) {
        broadcastSerial("<<< " + serialBuffer + "\n");
        serialBuffer = "";
      }
    } else if (c != '\r') {
      serialBuffer += c;
    }
  }
}
