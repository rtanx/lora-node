#include "main.h"

#include <ArduinoJson.h>

const int NODE_ID = 1;
unsigned long timeDelay = 30 * 1000;  // 30 seconds

/**
 * @brief Check if a specified interval of time has elapsed since the last call.
 *
 * @param[in] interval The time interval in milliseconds to check for.
 * @return True if the specified interval has elapsed since the last call,
 *         otherwise false.
 */
bool runEvery(unsigned long interval) {
    static unsigned long previousMillis = 0;  // Stores the timestamp of the last call
    unsigned long currentMillis = millis();   // Retrieves the current time

    // Checks if the difference between the current time and the previous time
    // is greater than or equal to the specified interval
    // then, updates the previous time to the current time if true
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        return true;
    }
    return false;
}

/**
 * @brief Callback function called automatically when the node has finished
 * transmitting data to the LoRa gateway.
 *
 * This function is triggered upon successful transmission completion.
 *
 * @note This function is typically registered as a callback with the LoRa library.
 */
void onTxDoneLoRa() {
    Serial.println("TxDone");
}

/**
 * @brief Configures and initializes the LoRa functionality on the microcontroller.
 *        This function sets up the LoRa transceiver module, initializes communication
 *        with the LoRa gateway, and sets necessary parameters.
 *
 * @note - This function should be called once during the setup phase of the
 *       microcontroller program. The LoRa transceiver module is configured to operate at the specified
 *       frequency (LORA_FREQ) during initialization.
 *
 *@note - This function utilizes LoRa.begin() to establish communication with the
 *       LoRa gateway. It retries the initialization process with a maximum of
 *       10 attempts if unsuccessful.
 */
void initLoRa() {
    // setup LoRa transceiver module
    // using 915E6 freq
    int conn_try = 1;
    int lora_begin_ok = 0;
    Serial.println("Initializing LoRa Node");
    while (!lora_begin_ok) {
        lora_begin_ok = LoRa.begin(LORA_FREQ);
        Serial.print(".");
        if (conn_try >= 10) {
            Serial.println();
            Serial.println("LoRa init failed. Check your connections.");
            while (true);
        }
        conn_try++;
        delay(500);
    }
    Serial.println();
    Serial.println("LoRa Initializing OK!");
    Serial.println("Only transmit messages to gateway");
    Serial.println();
    // Change sync word (0xF3) to match the receiver
    // The sync word assures you don't get LoRa messages from other LoRa transceivers
    // ranges from 0-0xFF
    LoRa.setSyncWord(0xF3);

    // Registers a callback function to handle the completion of data transmission to the LoRa gateway
    LoRa.onTxDone(onTxDoneLoRa);
}

/**
 * Sends a message via LoRa communication to the LoRa gateway.
 *
 * @brief This function constructs a LoRa packet containing the provided message
 * payload and transmits it to the LoRa gateway.
 *
 * @param[in] message The message payload to be transmitted. It should be of type
 *                String.
 *
 * @note This function initiates the transmission process by starting a new
 *       packet with `LoRa.beginPacket()`, and finishing the packet transmission with
 *       `LoRa.endPacket(true)`.
 *
 * @warning Ensure that the LoRa communication parameters, such as frequency
 *          and sync word, have been properly configured before calling this
 *          function.
 */
void LoRa_sendMessage(String message) {
    LoRa.beginPacket();    // start packet
    LoRa.print(message);   // add payload
    LoRa.endPacket(true);  // finish packet and send it
}

void setup() {
    Serial.flush();
    Serial.begin(9600);
    Serial.println("Initializing...");
    delay(3000);
    // init random seed for dummy random number generator for testing purpose
    randomSeed(analogRead(0));
    Serial.println("================= LoRa Node (Transmitter) =================");
    // configure and initialized LoRa module
    initLoRa();
}

void loop() {
    if (runEvery(timeDelay)) {
        JsonDocument doc;  // Creates a JSON document object to store sensor data.

        // map every sensor reading result to the json object
        doc["node_id"] = NODE_ID;
        doc["humidity"] = float(random(0, 5001) / float(100.0));
        doc["temperature"] = float(random(0, 5001) / float(100.0));
        doc["wind_speed"] = float(random(0, 5001) / float(100.0));
        doc["water_level"] = float(random(0, 5001) / float(100.0));

        String data;

        serializeJson(doc, data);  // do a serialization from json object to string literal

        Serial.println("Send data to gateway: ");

        LoRa_sendMessage(data);            // send the serializated json string literal to the gateway
        serializeJsonPretty(doc, Serial);  // print serialized json strin literal to the serial monitor
        Serial.println();
    }
}