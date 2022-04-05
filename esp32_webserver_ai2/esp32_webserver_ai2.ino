// Load Wi-Fi library
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define RXD2 16
#define TXD2 17

// Replace with your network credentials
const char* ssid = "CG2271-B01-01";
const char* password = "nicaicaikan";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String response, ip_address;

// Auxiliar variables to store the current output state
String output26State = "off";

// Assign output variables to GPIO pins
const int output26 = 26;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
int wait30 = 30000; // time to reconnect when connection is lost.

// This is your Static IP
IPAddress local_IP(192, 168, 4, 1); 
// Gateway IP address
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4); 

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  // Initialize the output variables as outputs
  pinMode(output26, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output26, LOW);

  //Configure Static IP
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}

void loop() {
  // Check if a client has connected..
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
   
  Serial.print("New client: ");
  Serial.println(client.remoteIP());
  
  /////////////////////////////////////////////////////
  // Read the information sent by the client.
  String req = client.readStringUntil('\r');
  Serial.println(req);

  if(req.indexOf("cmd") == -1)
  {
    client.stop();
    return;
  }
  
  int cmdIndex = req.indexOf("cmd")+3;
  uint8_t msg1 = req.charAt(cmdIndex) - '0';
  uint8_t msg2 = req.charAt(cmdIndex+1) - '0';
  uint8_t result = (msg1 * 10) + msg2;
  Serial.println(result);

  response = result;
  Serial2.write(result);
  
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  client.println(response); //  Return status.

  client.flush();
  client.stop();
  Serial.println("Client disconnected.");

}
