#include <WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define RELAY_PIN_1 14
#define RELAY_PIN_2 13
#define RELAY_PIN_3 12
#define RELAY_PIN_4 11
#define RELAY_PIN_5 10
#define RELAY_PIN_6 16
#define RELAY_PIN_7 17
#define RELAY_PIN_8 18
#define ONE_WIRE_BUS 4

const char *ssid = "ssid";
const char *password = "pass";
const char *mqtt_server = "server";
const char *mqtt_user = "mosquitto";
const char *mqtt_topic = "homeassistant/temperature/boileris";

WiFiClient espClient;
PubSubClient client(espClient);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

unsigned long lastTempRead = 0;
const long tempInterval = 10000; // Read every 10 seconds

void setup_wifi() {
    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, password);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        if (++attempts >= 60) {
            Serial.println("\nFailed to connect to WiFi.");
            return;
        }
    }
    Serial.println("\nConnected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length)
{
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] ");

	String message = "";
	for (int i = 0; i < length; i++)
	{
		message += (char)payload[i];
	}

	Serial.println(message);

	if (String(topic) == "homeassistant/relay/relay_1/set") {
		if (message == "ON") {
			digitalWrite(RELAY_PIN_1, LOW);							   // Turn relay 1 ON (open relay)
			client.publish("homeassistant/relay/relay_1", "ON", true); // Publish relay 1 state to MQTT
		} 	else if (message == "OFF") {
			digitalWrite(RELAY_PIN_1, HIGH);							// Turn relay 1 OFF (close relay)
			client.publish("homeassistant/relay/relay_1", "OFF", true); // Publish relay 1 state to MQTT
		}
	}

	if (String(topic) == "homeassistant/relay/relay_2/set") {
		if (message == "ON") {
			digitalWrite(RELAY_PIN_2, LOW);							   // Turn relay 2 ON (open relay)
			client.publish("homeassistant/relay/relay_2", "ON", true); // Publish relay 2 state to MQTT
		} else if (message == "OFF") {
			digitalWrite(RELAY_PIN_2, HIGH);							// Turn relay 2 OFF (close relay)
			client.publish("homeassistant/relay/relay_2", "OFF", true); // Publish relay 2 state to MQTT
		}
	}

	if (String(topic) == "homeassistant/relay/relay_3/set") {
		if (message == "ON") {
			digitalWrite(RELAY_PIN_3, LOW);							   // Turn relay 3 ON (open relay)
			client.publish("homeassistant/relay/relay_3", "ON", true); // Publish relay 3 state to MQTT
		} else if (message == "OFF") {
			digitalWrite(RELAY_PIN_3, HIGH);							// Turn relay 3 OFF (close relay)
			client.publish("homeassistant/relay/relay_3", "OFF", true); // Publish relay 3 state to MQTT
		}
	}

	if (String(topic) == "homeassistant/relay/relay_4/set") {
		if (message == "ON") {
			digitalWrite(RELAY_PIN_4, LOW);							   // Turn relay 4 ON (open relay)
			client.publish("homeassistant/relay/relay_4", "ON", true); // Publish relay 4 state to MQTT
		} else if (message == "OFF") {
			digitalWrite(RELAY_PIN_4, HIGH);							// Turn relay 4 OFF (close relay)
			client.publish("homeassistant/relay/relay_4", "OFF", true); // Publish relay 4 state to MQTT
		}
	}

	if (String(topic) == "homeassistant/relay/relay_5/set") {
		if (message == "ON") {
			digitalWrite(RELAY_PIN_5, LOW);							   // Turn relay 4 ON (open relay)
			client.publish("homeassistant/relay/relay_5", "ON", true); // Publish relay 4 state to MQTT
		} else if (message == "OFF") {
			digitalWrite(RELAY_PIN_5, HIGH);							// Turn relay 4 OFF (close relay)
			client.publish("homeassistant/relay/relay_5", "OFF", true); // Publish relay 4 state to MQTT
		}
	}

	if (String(topic) == "homeassistant/relay/relay_6/set")	{
		if (message == "ON") {
			digitalWrite(RELAY_PIN_6, LOW);							   // Turn relay 4 ON (open relay)
			client.publish("homeassistant/relay/relay_6", "ON", true); // Publish relay 4 state to MQTT
		} else if (message == "OFF") {
			digitalWrite(RELAY_PIN_6, HIGH);							// Turn relay 4 OFF (close relay)
			client.publish("homeassistant/relay/relay_6", "OFF", true); // Publish relay 4 state to MQTT
		}
	}

	if (String(topic) == "homeassistant/relay/relay_7/set") {
		if (message == "ON") {
			digitalWrite(RELAY_PIN_7, LOW);							   // Turn relay 4 ON (open relay)
			client.publish("homeassistant/relay/relay_7", "ON", true); // Publish relay 4 state to MQTT
		}
		else if (message == "OFF") {
			digitalWrite(RELAY_PIN_7, HIGH);							// Turn relay 4 OFF (close relay)
			client.publish("homeassistant/relay/relay_7", "OFF", true); // Publish relay 4 state to MQTT
		}
	}

	if (String(topic) == "homeassistant/relay/relay_8/set") {
		if (message == "ON") 	{
			digitalWrite(RELAY_PIN_8, LOW);							   // Turn relay 4 ON (open relay)
			client.publish("homeassistant/relay/relay_8", "ON", true); // Publish relay 4 state to MQTT
		}
		else if (message == "OFF") 	{
			digitalWrite(RELAY_PIN_8, HIGH);							// Turn relay 4 OFF (close relay)
			client.publish("homeassistant/relay/relay_8", "OFF", true); // Publish relay 4 state to MQTT
		}
	}
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("ESP32RelayModule", mqtt_user, NULL)) {
            Serial.println("connected");
            client.subscribe(mqtt_topic);
            client.subscribe("homeassistant/relay/relay_1/set");
			client.subscribe("homeassistant/relay/relay_2/set");
			client.subscribe("homeassistant/relay/relay_3/set");
			client.subscribe("homeassistant/relay/relay_4/set");
			client.subscribe("homeassistant/relay/relay_5/set");
			client.subscribe("homeassistant/relay/relay_6/set");
			client.subscribe("homeassistant/relay/relay_7/set");
			client.subscribe("homeassistant/relay/relay_8/set");

        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" retrying in 5 seconds...");
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    pinMode(ONE_WIRE_BUS, INPUT_PULLUP);
    pinMode(RELAY_PIN_1, OUTPUT);
    pinMode(RELAY_PIN_2, OUTPUT);
    pinMode(RELAY_PIN_3, OUTPUT);
    pinMode(RELAY_PIN_4, OUTPUT);
    pinMode(RELAY_PIN_5, OUTPUT);
    pinMode(RELAY_PIN_6, OUTPUT);
    pinMode(RELAY_PIN_7, OUTPUT);
    pinMode(RELAY_PIN_8, OUTPUT);
    digitalWrite(RELAY_PIN_1, HIGH);
    digitalWrite(RELAY_PIN_2, HIGH);
    digitalWrite(RELAY_PIN_3, HIGH);
    digitalWrite(RELAY_PIN_4, HIGH);
    digitalWrite(RELAY_PIN_5, HIGH);
    digitalWrite(RELAY_PIN_6, HIGH);
    digitalWrite(RELAY_PIN_7, HIGH);
    digitalWrite(RELAY_PIN_8, HIGH);
    sensors.begin();

	if (digitalRead(RELAY_PIN_1) == HIGH) {
		client.publish("homeassistant/relay/relay_1", "OFF", true); // Relay 1 is off (closed)
	} else {
		client.publish("homeassistant/relay/relay_1", "ON", true); // Relay 1 is on (open)
	}

	if (digitalRead(RELAY_PIN_2) == HIGH) {
		client.publish("homeassistant/relay/relay_2", "OFF", true); // Relay 2 is off (closed)
	} else {
		client.publish("homeassistant/relay/relay_2", "ON", true); // Relay 2 is on (open)
	}

	if (digitalRead(RELAY_PIN_3) == HIGH) {
		client.publish("homeassistant/relay/relay_3", "OFF", true); // Relay 3 is off (closed)
	} else {
		client.publish("homeassistant/relay/relay_3", "ON", true); // Relay 3 is on (open)
	}

	if (digitalRead(RELAY_PIN_4) == HIGH) {
		client.publish("homeassistant/relay/relay_4", "OFF", true); // Relay 4 is off (closed)
	} else 	{
		client.publish("homeassistant/relay/relay_4", "ON", true); // Relay 4 is on (open)
	}

	if (digitalRead(RELAY_PIN_5) == HIGH) {
		client.publish("homeassistant/relay/relay_5", "OFF", true); // Relay 4 is off (closed)
	} else {
		client.publish("homeassistant/relay/relay_5", "ON", true); // Relay 4 is on (open)
	}

	if (digitalRead(RELAY_PIN_6) == HIGH) {
		client.publish("homeassistant/relay/relay_6", "OFF", true); // Relay 4 is off (closed)
	} else {
		client.publish("homeassistant/relay/relay_6", "ON", true); // Relay 4 is on (open)
	}

	if (digitalRead(RELAY_PIN_7) == HIGH) {
		client.publish("homeassistant/relay/relay_7", "OFF", true); // Relay 4 is off (closed)
	} else {
		client.publish("homeassistant/relay/relay_7", "ON", true); // Relay 4 is on (open)
	}

	if (digitalRead(RELAY_PIN_8) == HIGH) {
		client.publish("homeassistant/relay/relay_8", "OFF", true); // Relay 4 is off (closed)
	} else {
		client.publish("homeassistant/relay/relay_8", "ON", true); // Relay 4 is on (open)
	}
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    
    static unsigned long lastTempRead = 0;
    const long tempInterval = 10000; // Send temperature every 10 seconds

    unsigned long currentMillis = millis();
    if (currentMillis - lastTempRead >= tempInterval) {
        lastTempRead = currentMillis;

        sensors.requestTemperatures();
        delay(200); 
        float temperature = sensors.getTempCByIndex(0);

         Serial.print("Temperature: ");
            Serial.print(temperature);
            Serial.println(" °C");

            char tempMsg[8];
            dtostrf(temperature, 1, 2, tempMsg); // Format temperature to string
            client.publish(mqtt_topic, tempMsg);
    }
}

