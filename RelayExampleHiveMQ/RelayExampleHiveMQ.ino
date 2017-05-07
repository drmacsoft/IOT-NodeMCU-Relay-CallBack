#include <ESP8266WiFi.h>// Library for using the NodeMCU
#include <PubSubClient.h> // Library for using the Network protocall MQTT 

// Update these with values suitable for your network.

const char* ssid = "???????"; // Add your SSID(The Name of the Wifi Router you connect to)
const char* password = "???????"; // The PassWord (Wireless Key)
const char* mqtt_server = "broker.hivemq.com"; // The MQTT broker you are connecting to

WiFiClient espClient;
PubSubClient client(espClient);
/*Variables for you broker and Client test*/
long lastMsg = 0; 
char msg[50];
int value = 0;
/******************************************/

/********** Use integer variables to keep track of your relay ports **************************/
int RelayOne = 14;   // GPIO 14 (D5)
int RelayTwo = 12;   // GPIO 12 (D6)
int RelayThree = 4; // GPIO 13 (D2)
int RelayFour = 5;  // GPIO 15 (D1)

void setup() {
  // Initialize the RELAY pins as an output
  pinMode(RelayOne, OUTPUT);
  pinMode(RelayTwo, OUTPUT);
  pinMode(RelayThree, OUTPUT);
  pinMode(RelayFour, OUTPUT);

  /****************** PLAN FOR ON/OFF RELAY STATUS *********************************************
  I would suggest adding code for a fail safe and that suits you to ensure the status you want
  your Relays to be ON or OFF. Right now, when this program boots the default is sets to all
  relays to be ON
  *******************************************************************************************/


  Serial.begin(115200);// Starts the serial monitor at 115200
  setup_wifi(); // add Method for the Wifi setup
  client.setServer(mqtt_server, 1883);// connects to the MQTT broker
  client.setCallback(callback);
}
/********************* Method for Wifi Setup***************************************************/
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
/********************* END OF SETUP WIFI ******************************************************/

/********************* CALLBACK METHOD ********************************************************/
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  /*change to a case later*/
  /******* Switch on the RELAY1 if 1 was received as a first character ************************/
  if ((char)payload[0] == '1') {
    digitalWrite(RelayOne, LOW);   // Turn the RELAY on (Note that LOW is the voltage level)
    client.publish("HomeOutTopic", "Relay 1 is ON");// Turns the relay ON
    Serial.print("Relay 1 is ON"); // Prints to seriel
  }
  if ((char)payload[0] == '0') {
    client.publish("HomeOutTopic", "Relay 1 is OFF");// Turns the relay OFF                       
    digitalWrite(RelayOne, HIGH); // Turns relay OFF
    Serial.print("Relay 1 is OFF"); // Prints to seriel
  }

  /******* Switch on the RELAY2 if 3 was received as a first character ************************/
  if ((char)payload[0] == '3') {
    digitalWrite(RelayTwo, LOW);   // Turn the RELAY on (Note that LOW is the voltage level)
    client.publish("HomeOutTopic", "Relay 2 is ON");// Turns the relay ON
    Serial.print("Relay 2 is ON"); // Prints to seriel
  }
  else if ((char)payload[0] == '2') {
    client.publish("HomeOutTopic", "Relay 2 is OFF");// Turns the relay OFF                          
    digitalWrite(RelayTwo, HIGH); // Turns relay off
    Serial.print("Relay 2 is OFF"); // Prints to seriel
  }

  /******* Switch on the RELAY3 if 5 was received as a first character ************************/
  else if ((char)payload[0] == '5') {
    digitalWrite(RelayThree, LOW);   // Turn the RELAY on (Note that LOW is the voltage level)
    client.publish("HomeOutTopic", "Relay 3 is ON");// Turns the relay ON
    Serial.print("Relay 3 is ON"); // Prints to seriel
  }
  else if ((char)payload[0] == '4') {
    client.publish("HomeOutTopic", "Relay 3 is OFF");// Turns the relay OFF
    digitalWrite(RelayThree, HIGH); // Turns relay off
    Serial.print("Relay 3 is OFF"); // Prints to seriel
  }

  /******* Switch on the RELAY4 if 7 was received as a first character ************************/
  else if ((char)payload[0] == '7') {
    digitalWrite(RelayFour, LOW);   // Turn the RELAY on (Note that LOW is the voltage level)
    client.publish("HomeOutTopic", "Relay 4 is ON");// Turns the relay ON
    Serial.print("Relay 4 is ON"); // Prints to seriel
  }
  else if ((char)payload[0] == '6') {
    client.publish("HomeOutTopic", "Relay 4 is OFF");// Turns the relay OFF                         
    digitalWrite(RelayFour, HIGH); // Turns relay OFF
    Serial.print("Relay 4 is OFF"); // Prints to seriel
  }
}
/********************* END OF CALLBACK METHOD **************************************************/

/********************* RECONNECT METHOD ********************************************************/
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("HomeOutTopic", "hello world");
      // ... and resubscribe
      client.subscribe("HomeOutTopic");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}
/********************* END OF RECONNECT METHOD ************************************************/

/********************* MAIN PROGRAM LOOP ******************************************************/
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  /************MQTT PAYLOAD TEST **********************************
  You can use the code commented out below to test your broker
  and client are working

  long now = millis();
  if (now - lastMsg > 2000) {
  lastMsg = now;
  ++value;
  snprintf(msg, 75, "hello world #%ld", value);
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish("HomeOutTopic", msg);
  }
  ***************************************************************/
}
