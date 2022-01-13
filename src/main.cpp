#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
#include <Arduino.h>
#include <2smart.h>

#include "config.h"
#include "credentals.h"
#include "properties/ColorRGBProp.h"
#include "nodes/ApplicationNode.h"


String product_id = CRED_PRODUCT_ID;  // production id
const char *device_name = "Qualification device";
const char *device_version = "1";


Cloud2Smart smartcloud;
ApplicationNode *appNode = nullptr;

void setup() {
    Serial.begin(115200);
    appNode = new ApplicationNode("Lighter", "lighter", smartcloud.GetDevice());  // (name, id,device)

    auto *led_state = new Property("Blinker", "state", appNode, SENSOR, true, true, "boolean");
    auto *pLedColor = new ColorRGBProp("Led color", "color", appNode, SENSOR, true, true);
    auto *pDistance = new Property("Measured distance", "dist", appNode, SENSOR, false, false, "", "");
    auto *pMode = new Property("Mode", "mode", appNode, SENSOR, true, true, "enum", ApplicationNode::GetModes());
    smartcloud.setup();
}

void loop() {
    smartcloud.loop();
}

#pragma clang diagnostic pop