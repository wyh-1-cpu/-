#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "HONOR 200";
const char* password = "12345678";
const int ledPin = 2;

WebServer server(80);

const char* htmlPage = R"(
<!DOCTYPE html>
<html>
<body style="text-align:center;padding:50px;">
<h2>调光器</h2>
<input type="range" id="s" min="0" max="255" value="0" style="width:80%;" 
 oninput="fetch('/set?val='+this.value);document.getElementById('v').innerText=this.value;">
<p>亮度: <span id="v">0</span></p>
</body>
</html>
)";

void setup() {
    Serial.begin(115200);
    
    // 新版API：直接绑定引脚
    ledcAttach(ledPin, 5000, 8);
    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) delay(500);
    Serial.println(WiFi.localIP());
    
    server.on("/", [](){ server.send(200, "text/html", htmlPage); });
    server.on("/set", [](){
        ledcWrite(ledPin, server.arg("val").toInt());  // 直接写引脚
        server.send(200, "text/plain", "OK");
    });
    
    server.begin();
}

void loop() {
    server.handleClient();
}