#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

// =========== LoRa ===============
#include <LoRa.h>
#include <SPI.h>
const long LORA_FREQ = 915E6;

bool runEvery(unsigned long interval);
void initLoRa();
void LoRa_rxMode();
void LoRa_txMode();
void onReceiveLoRa(int packet_size);
void onTxDoneLoRa();
#endif