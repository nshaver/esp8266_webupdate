/*
 * ESP826612E web upload demo
 * allows update of arduino program via ESP8266HttpUpdateServer /update
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

// wifi
const char* host= "esp8266";
const char* ssid = "Tux24";
const char* password = "a47a47a47a";
MDNSResponder mdns;
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
unsigned long lastHandleClient=0;

bool debug=true;

void setup(void){
	if (debug) Serial.begin(115200);
	pinMode(BUILTIN_LED, OUTPUT);
	digitalWrite(BUILTIN_LED, LOW);

	// start wifi, get IP
	WiFi.mode(WIFI_AP_STA);
	WiFi.begin(ssid, password);
	
	if (debug) Serial.println("");

	// Wait for wifi connection
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		if (debug) Serial.print(".");
	}

	// turn off LED
	digitalWrite(BUILTIN_LED, HIGH);
	
	if (debug) {
		Serial.println("");
		Serial.print("Connected to ");
		Serial.println(ssid);
		Serial.print("IP address: ");
		Serial.println(WiFi.localIP());
	}

	// register hostname on network
	if (mdns.begin(host, WiFi.localIP())) {
		if (debug) Serial.println("MDNS responder started");
	}

	// establish http bootloader updater
	httpUpdater.setup(&server);

	server.on("/", handleRoot);
	server.onNotFound(handleNotFound);
	
	server.begin();
	if (debug) Serial.println("HTTP server started");
}
 
void loop(void){
	if (WiFi.status() != WL_CONNECTED) {
		WiFi.begin(ssid, password);
		delay(500);
	}

	// optional - handle webpages every x ms
	server.handleClient();

	// blink LED
	digitalWrite(BUILTIN_LED, LOW);
	delay(100);
	digitalWrite(BUILTIN_LED, HIGH);
	delay(100);
	digitalWrite(BUILTIN_LED, LOW);
	delay(100);
	digitalWrite(BUILTIN_LED, HIGH);
	delay(1000);
} 

/*
 * handleRoot
 *
 * create and transmit a webpage
 */
void handleRoot() {
	String o="<html><head><title>ESP8266</title></head>\
<body>\
Welcome to your ESP8266\
</body></html>";
	server.send(200, "text/html", o);
}

/*
 * handleNotFound
 *
 * tell the browser that the page is not found
 */
void handleNotFound(){
	server.send(404, "text/plain", "File not found");
}
