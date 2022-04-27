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
String chip_id = String(ESP.getChipId());
String message_offset;

void setup() {
	//mesh
	// mesh.setDebugMsgTypes(ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE);   // all types on

	mesh.init(ssid, password, PORT);
	mesh.setContainsRoot(true);
	mesh.onReceive(&received_callback);

    //message_offset
    for (int i = 0; i < OFFSET_SIZE; i++)
        message_offset += String(':');
}

void loop() {
	mesh.update();
    uint32_t curr_millis = millis();
	if (curr_millis - prev_millis > interval_ms) {
        prev_millis = curr_millis;
        
        sprintf(count, "%03x", message_counter);
		mesh.sendBroadcast(chip_id + String('\t') + String(count) + message_offset);
        message_counter++;
		message_counter = message_counter > 1000 ? 0 : message_counter;		
	}
}

void received_callback(const uint32_t &from, const String &msg) {
}
