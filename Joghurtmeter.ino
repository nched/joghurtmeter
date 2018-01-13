#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <OneWire.h>
//Get DallasTemperature Library here:  http://milesburton.com/Main_Page?title=Dallas_Temperature_Control_Library
#include <DallasTemperature.h>
// Declare Constants and Pin Numbers
#define ONE_WIRE_BUS_PIN 2
// Setup a oneWire instance to communicate with any OneWire devicesgb
OneWire oneWire(ONE_WIRE_BUS_PIN);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
// Create an array
DeviceAddress One;
// Define Variables - as array so that we can hav more sensors
int probeval[1];

const char *ssid = "ssid";
const char *password = "password";

String webString = "";
int sensorcount;

ESP8266WebServer server ( 80 );

void setup ( void ) {
	Serial.begin ( 115200 );
	WiFi.begin ( ssid, password );
	Serial.println ( "" );

	// Wait for connection
	while ( WiFi.status() != WL_CONNECTED ) {
		delay ( 500 );
		Serial.print ( "." );
	}

	Serial.println ( "" );
	Serial.print ( "Connected to " );
	Serial.println ( ssid );
	Serial.print ( "IP address: " );
	Serial.println ( WiFi.localIP() );


	//server.on ( "/", handleRoot );
  server.on("/", [](){  // if you add this subdirectory to your webserver call, you get text below :)
    float temp = probeval[0];
    // read sensor
    webString="<meta http-equiv='refresh' content='3'>Temperature: "+String((float)temp)+" C"; 
    server.send(200, "text/html", webString);            // send to someones browser when asked
  });
	server.begin();
	Serial.println ( "HTTP server started" );

  // Initialize the Temperature measurement library
  sensors.begin();
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");
 
  if (!sensors.getAddress(One, 0)) Serial.println("Unable to find address for Device 0"); 
}


void loop ( void ) {
	server.handleClient();
  readtemp();
}


void readtemp() {
  // Command all devices on bus to read temperature
  sensors.requestTemperatures();
  sensorcount = sensors.getDeviceCount(), DEC;
  Serial.print("Amound of Sensors: ");
  Serial.println(sensorcount);
  for(int i=0;i<=sensorcount-1;i++){
    probeval[i] = (sensors.getTempCByIndex(i)); 
    Serial.println(probeval[i]);
  }

}
