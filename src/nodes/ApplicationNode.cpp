//
// Created by Victor on 13.01.2022.
//

#include "ApplicationNode.h"
#include "node/node.h"
#include "property/property.h"
#include "properties/ColorRGBProp.h"
#include "gamma.h"


void sonarCallbackStub();

extern ApplicationNode *appNode;

const std::map<ApplicationNode::modes, String> ApplicationNode::modesMap = {
        {modes::Static,    "Static"},
        {modes::Threshold, "Threshold"},
        {modes::Doppler,   "Doppler"},
};

bool ApplicationNode::Init(Homie *homie) {
    bool state = Node::Init(homie);

    pinMode(GPIO_LED_R, OUTPUT);
    pinMode(GPIO_LED_G, OUTPUT);
    pinMode(GPIO_LED_B, OUTPUT);

    ledcSetup(LEDC_CHANNEL_R, 5000, 12);
    ledcSetup(LEDC_CHANNEL_G, 5000, 12);
    ledcSetup(LEDC_CHANNEL_B, 5000, 12);

    ledcAttachPin(GPIO_LED_R, LEDC_CHANNEL_R);
    ledcAttachPin(GPIO_LED_G, LEDC_CHANNEL_G);
    ledcAttachPin(GPIO_LED_B, LEDC_CHANNEL_B);

    sonarTimer = new SimpleTimer();
    hc = new UltraSonicDistanceSensor(GPIO_NUM_16, GPIO_NUM_17);

    sonarTimer->setInterval(100, sonarCallbackStub);
    return state;
}

void ApplicationNode::HandleCurrentState() {
    Node::HandleCurrentState();

    ColorRGBProp *pColor = (ColorRGBProp *) (properties_.find("color")->second);
    ColorRGBProp *pMode = (ColorRGBProp *) (properties_.find("mode")->second);

    if (pMode->HasNewValue()) {

        auto it = find_if(modesMap.begin(), modesMap.end(),
                          [pMode](const std::pair<modes, String> &p) {
                              return p.second == pMode->GetValue();
                          });
        if (it == modesMap.end()) {
            Serial.println("mode out of range");
        } else {
            currentMode = it->first;
        }
    }
    switch (currentMode) {

        case modes::Static:
            if (pColor->HasNewValue()) {
                pColor->SetHasNewValue(false);
                ledcWrite(LEDC_CHANNEL_R, gamma_lut[pColor->GetColorStruct().red]);
                ledcWrite(LEDC_CHANNEL_G, gamma_lut[pColor->GetColorStruct().green]);
                ledcWrite(LEDC_CHANNEL_B, gamma_lut[pColor->GetColorStruct().blue]);
            }
            break;
        case modes::Threshold:
            if (distanceDeque.back() > HIGH_THRESHOLD) {
                ledcWrite(LEDC_CHANNEL_R, 4095);
                ledcWrite(LEDC_CHANNEL_G, 0);
                ledcWrite(LEDC_CHANNEL_B, 0);
            } else if (distanceDeque.back() < LOW_THRESHOLD) {
                ledcWrite(LEDC_CHANNEL_R, 0);
                ledcWrite(LEDC_CHANNEL_G, 4095);
                ledcWrite(LEDC_CHANNEL_B, 0);
            } else {
                auto pwmVal = map(distanceDeque.back(), LOW_THRESHOLD, HIGH_THRESHOLD, 0, 255);
                pwmVal = constrain(pwmVal, 0, 255);
                ledcWrite(LEDC_CHANNEL_R, gamma_lut[pwmVal]);
                ledcWrite(LEDC_CHANNEL_G, gamma_lut[255 - pwmVal]);
                ledcWrite(LEDC_CHANNEL_B, 0);
            }
            break;
        case modes::Doppler:

            if (sonarFlag) {
                sonarFlag = false;
                float oldAvg = std::accumulate(distanceDeque.begin(), distanceDeque.begin() + distanceDeque.size() / 2,
                                               0);
                float newAvg = std::accumulate(distanceDeque.begin() + distanceDeque.size() / 2, distanceDeque.end(),
                                               0);

                Serial.print(oldAvg);
                Serial.print(" ");
                Serial.println(newAvg);
                auto pwmVal = map((long) ((newAvg - oldAvg) * 10), -300, 300, -127, 127);
                pwmVal = constrain(pwmVal, -127, 127);

                ledcWrite(LEDC_CHANNEL_R, gamma_lut[127 + pwmVal]);
                ledcWrite(LEDC_CHANNEL_G, 0);
                ledcWrite(LEDC_CHANNEL_B, gamma_lut[127 - pwmVal]);
            }
            break;
    }


    sonarTimer->run();
}

void ApplicationNode::sonarCallback() {
    float dist = hc->measureDistanceCm();

    if (dist < 0)
        return;

    distanceDeque.push_back(dist);
    if (distanceDeque.size() > DISTANCE_BUF_SIZE)
        distanceDeque.pop_front();

    Property *pDist = properties_.find("dist")->second;
    pDist->SetValue(String(dist));
    sonarFlag = true;

}

ApplicationNode::~ApplicationNode() {
    delete sonarTimer;
    delete hc;
}

String ApplicationNode::GetModes() {
    String ModeFormatValue = "";
    for (const auto &it: modesMap) {
        ModeFormatValue += it.second;
        ModeFormatValue += ",";
    }
    return ModeFormatValue.substring(0, ModeFormatValue.length() - 1);
}

void sonarCallbackStub() {
    if (appNode != nullptr)
        appNode->sonarCallback();
}
