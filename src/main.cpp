#include <Arduino.h>
#include <painlessMesh.h>
#include <string.h>

#include "data.hpp"

#define MESSAGE_COUNTER_SIZE    3
#define OFFSET_SIZE             10

void received_callback(const uint32_t &from, const String &msg);

char count[MESSAGE_COUNTER_SIZE + 1];
painlessMesh mesh;
uint16_t message_counter = 0;
uint32_t prev_millis = 0;
uint32_t interval_ms = 1000;
String message_offset;
void setup() {
	Serial.begin(BAUDRATE);

	//mesh
	// mesh.setDebugMsgTypes(ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE);   // all types on

	mesh.init(ssid, password, PORT);
	mesh.setContainsRoot(true);
	mesh.onReceive(&received_callback);

    //message_offset
    for (int i = 0; i < OFFSET_SIZE; i++)
        message_offset += String(':');

	//dht
	timer1_attachInterrupt(timer_overflow);
	timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);         //5MHz
	timer1_write(OVERFLOW_LIMIT);
}

void loop() {
	mesh.update();
    uint32_t curr_millis = millis();
	if (curr_millis - prev_millis > interval_ms) {
        prev_millis = curr_millis;
        
        sprintf(count, "%03x", message_counter);
		mesh.sendBroadcast(String(ESP.getChipId()) + String('\t') + String(count) + message_offset);
        message_counter++;
	}
}

void received_callback(const uint32_t &from, const String &msg) {
}
