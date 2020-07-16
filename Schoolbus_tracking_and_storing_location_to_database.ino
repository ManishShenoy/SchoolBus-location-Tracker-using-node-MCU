#include <ESP8266WiFi.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
/* Create object named bt of the class SoftwareSerial */
SoftwareSerial GPS_SoftSerial(D4, D3);/* (Rx, Tx) *///connect D4 and D3 to NEO-6M GPS module
/* Create an object named gps of the class TinyGPSPlus */
TinyGPSPlus gps;

void ICACHE_RAM_ATTR ISRoutine ();
void ICACHE_RAM_ATTR ISR_stop ();
void ICACHE_RAM_ATTR ISR_start ();

const int startt = D5; //push Button to D5 node MCU
const int nextt = D6; //push Button to D6 node MCU
const int stopp  = D7; //push Button to D7 node MCU

const char* ssid     = ""; //enter ssid name
const char* password = ""; //enter password
const char* host = ""; //enter host name

String data = "";
String loc_name = "";
double lat1;
double lon;
int count, c;
int flag = 0;
int stno = 0;
int contd =0;

void setup()
{
  pinMode(startt, INPUT_PULLUP);
  pinMode(nextt, INPUT_PULLUP);
  pinMode(stopp, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(nextt),ISRoutine,FALLING);
  attachInterrupt(digitalPinToInterrupt(stopp),ISR_stop,FALLING);
  attachInterrupt(digitalPinToInterrupt(startt),ISR_start,FALLING);

  Serial.begin(115200);
  GPS_SoftSerial.begin(9600);
  delay(100);
  Serial.print("connecting to ");
  Serial.println(ssid);
  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }

  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("WiFi Connected");
}

void loop() {
  if (contd==3) {
    loc_name = "School";
    get_GPS();
    Serial.println(loc_name);
    Serial.println(lat1);
    Serial.println(lon);
    delay(500);
    upload_data();
    Serial.println("Uploading current positionQ");
    loc_name = "currentP";
    upload_data();
    count = 1;
    contd =0;
  }
  else if (contd == 1) {
    if (stno == 0) {
      loc_name = "point1";
      get_GPS();
      Serial.println(loc_name);
      Serial.println(lat1);
      Serial.println(lon);
      delay(500);
      upload_data();
      count = 1;
      contd = 0;
      stno = 1;
    }
    else if (stno == 1) {
      loc_name = "point2";
      get_GPS();
      Serial.println(loc_name);
      Serial.println(lat1);
      Serial.println(lon);
      delay(500);
      upload_data();
      count = 1;
      contd = 0;
      stno = 2;
    } else {
      loc_name = "point3";
      get_GPS();
      Serial.println(loc_name);
      Serial.println(lat1);
      Serial.println(lon);
      delay(500);
      upload_data();
      count = 1;
      stno = 3;
      contd = 0;
    }

  }
  else if (contd == 2) {
    Serial.println("End");
    loc_name = "end";
    get_GPS();
    Serial.println(loc_name);
    Serial.println(lat1);
    Serial.println(lon);
    delay(500);
    upload_data();

  }
  else {
    while (c == 0) {
      Serial.println("Welcome");
      c = 1;
    }
    if (count == 1) {
      Serial.println("Update location");
      loc_name = "currentP";
      get_GPS();
      update_data();
      delay(5000);
    }
  }
}


void upload_data() {
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort))
  {
    Serial.println("Failed to connect WiFi client");
    return;
  }

  String url = "/folder name/insert_data.php?dst_pt=" + loc_name + "&latitude=" + lat1 + "&longitude=" + lon;//enter floder name in which insert_data.php is present

  Serial.println("S");

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(2000);

  while (client.available())
  {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
}

void update_data() {
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort))
  {
    Serial.println("Failed to connect WiFi client");
    return;
  }

  String url = "/folder name/update_data.php?dst_pt=" + loc_name + "&latitude=" + lat1 + "&longitude=" + lon; //enter floder name in which update_data.php is present

  Serial.println("S");

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(2000);

  while (client.available())
  {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
}

void get_GPS() {
  smartDelay(1000); /* Generate precise delay of 1ms */
  unsigned long start;
  double lat_val, lng_val;
  bool loc_valid;
  lat1 = gps.location.lat(); /* Get latitude data */
  loc_valid = gps.location.isValid(); /* Check if valid location data is available */
  lon = gps.location.lng(); /* Get longtitude data */

  if (!loc_valid) {
    Serial.print("******** ");
    Serial.println("********");
  }
  else {
    Serial.print(lat1, 6);
    Serial.print(' ');
    Serial.println(lon, 6);
  }
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do {
    while (GPS_SoftSerial.available())  /* Encode data read from GPS while data is available on serial port */
      gps.encode(GPS_SoftSerial.read());
    /* Encode basically is used to parse the string received by the GPS and to store it in a buffer so that information can be extracted from it */
  } while (millis() - start < ms);
}

void ISR_start () {
  contd = 3;
}

void ISRoutine () {
  contd = 1;
}

void ISR_stop () {
  contd = 2;
}
