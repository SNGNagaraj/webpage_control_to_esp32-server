#include <WiFi.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
const char* ssid = "ESIL_UG_2G";
const char* password = "Hild#098";
//char ssid[50];
//char pass[50];

const int ledPin1 = 2;  // LED connected to GPIO 26
const int ledPin2 = 27;  // LED connected to GPIO 27

DHT dht(15, DHT22);  // DHT11 sensor connected to GPIO 14
LiquidCrystal_I2C lcd(0x27, 16, 2);

WiFiServer server(80);

String header;
String output26State = "off";
String output27State = "off";

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 10000;

// static ip  address  set
IPAddress local_IP(192, 168, 0, 5);
// set your Gateway 
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);
IPAddress primaryDNS(0,0,0,0);
IPAddress secondaryDNS(0,0,0,0);

void setup() {
  Serial.begin(115200);
  lcd.begin();
  lcd.clear();
  lcd.backlight();
  dht.begin();
  display();
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  // CONFIGURE STATIC ip address
  if(!WiFi.config(local_IP,gateway,subnet,primaryDNS,secondaryDNS)){
   Serial.println("STA failed to configure");
  }
  //connect to  wi_fi  network with  SSID and password
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print("Connecting to ");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.setCursor(0,1);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  uint32_t signalrssi = WiFi.localIP();
  lcdwifi();
  delay(1000);
  Serial.println(WiFi.localIP());
  Serial.println("Receiving Signal strength indicator : \t\t"); Serial.print(WiFi.RSSI());Serial.print("dB");
 // ipaddresschanges();
  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    currentTime = millis();
    previousTime = currentTime;
    // lcd.setCursor(0,0);
     //lcd.clear();
    // lcd.print("NEW CLIENT !");
    Serial.println("New Client.");
    String currentLine = "";

    while (client.connected() && currentTime - previousTime <= timeoutTime)
    //while (client.connected())
     {
      currentTime = millis();
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

          
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              output26State = "on";
              digitalWrite(ledPin1, HIGH);
              String MotorDrive = "{\"MotorDriveStatus\":"+ String(output26State)+"}";
              client.println(MotorDrive);
              lcdmotordrivestatus();
              
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              digitalWrite(ledPin1, LOW);
              String MotorDrive_status = "{\"MotorDriveStatus\":"+ String(output26State)+"}";
              client.println(MotorDrive_status);
              lcdmotordrivestatus();
              
            }

            if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              output27State = "on";
              digitalWrite(ledPin2, HIGH);
              String BrushDrive = "{\"BrushDriveStatus\":"+ String(output27State)+"}";
              client.println(BrushDrive);
              lcdbrushdrivestatus();
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              output27State = "off";
              digitalWrite(ledPin2, LOW);
              String BrushDrive_status = "{\"BrushDriveStatus\":"+ String(output27State)+"}";
              client.println(BrushDrive_status);
             lcdbrushdrivestatus();
            }

             if (header.indexOf("GET /index") >= 0) {
              float temperature = dht.readTemperature(true);
              float humidity = dht.readHumidity();
              int RSSI = WiFi.RSSI();
              String responseData = "{\"temp\": "+String(temperature)+",\"humidity\":"+ String(humidity) +"}";
              client.println(responseData);
              // output26State = "on";
               // lcd.setCursor(0,0);
               // lcd.clear();
               // lcd.print("WiFi.RSSI");
               // lcd.setCursor(11,0);
                //lcd.clear();
                //lcd.print(WiFi.RSSI());
             }


            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");//view port of webpage 
            client.println("<link rel=\"icon\" href=\"data:,\">"); // link define part
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");

            client.println("<body><h1>Robot Communication Diagnostic Tool </h1>");

            client.println("<p>Motordrive - State " + output26State + "</p>");
            if (output26State == "off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<p>Brush drive - State " + output27State + "</p>");
            if (output27State == "off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
          
            
            client.println("<p><a href=\"/readDHT\"><button class=\"button\">Read DHT</button></a></p>");
             if (header.indexOf("GET /readDHT") >= 0) {
              float temperature = dht.readTemperature(true);
              float humidity = dht.readHumidity();
              int RSSI = WiFi.RSSI();

              client.println("<p>Temperature: " + String(temperature) + " °C</p>");
              client.println("<p>Humidity: " + String(humidity) + " %</p>");
              client.println("<p>RSSI : "+String(RSSI)+"dB</p>");
                 delay(1000);
             }
            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    header = "";
    client.stop();
    //delay(1000);
    lcd.setCursor(5,0);
    lcd.clear();
    lcd.print("client");
    lcd.setCursor(2,1);
    //lcd.clear();
    lcd.print("Disconnected !");
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
 




