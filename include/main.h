#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

// =========== LoRa ===============
#include <LoRa.h>
#include <SPI.h>
const long LORA_FREQ = 915E6;

/**
 * @brief Check if a specified interval of time has elapsed since the last call.
 *
 * @param[in] interval The time interval in milliseconds to check for.
 * @return True if the specified interval has elapsed since the last call,
 *         otherwise false.
 */
bool runEvery(unsigned long interval);

/**
 * @brief Callback function called automatically when the node has finished
 * transmitting data to the LoRa gateway.
 *
 * This function is triggered upon successful transmission completion.
 *
 * @note This function is typically registered as a callback with the LoRa library.
 */
void onTxDoneLoRa();

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
void initLoRa();

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
void LoRa_sendMessage(String message);

#ifdef LORA_NODE_ID
const int NODE_ID = int(LORA_NODE_ID);
#else
const int NODE_ID = 0;
#endif
#endif