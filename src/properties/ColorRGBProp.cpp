//
// Created by Victor on 13.01.2022.
//

#include "ColorRGBProp.h"

#include "node/node.h"
#include "property/property.h"

ColorRGBProp::ColorRGBProp(const char *name, const char *id, Node *node, PROPERTY_TYPE type, bool settable,
                           bool retained)
        : Property(name, id, node, type, settable, retained, "color", "rgb") {

}

bool ColorRGBProp::Init(Homie *homie) {
    bool status = true;
    if (!Property::Init(homie)) status = false;

    return status;
}

void ColorRGBProp::HandleCurrentState() {
    Property::HandleCurrentState();
}

void ColorRGBProp::HandleSettingNewValue() {
    //if value is set from MQTT
    if (this->has_new_value_) {
        color = DecodeColor(GetValue());
    }
}

ColorRGBProp::Color ColorRGBProp::GetColorStruct() {
    return color;
}

void ColorRGBProp::SetColorStruct(Color colorStruct) {
    color = colorStruct;

    value_ = EncodeColor(colorStruct);  //prevent receiving has_new_value_ in HandleSettingNewValue
    SetValue(value_);
}

ColorRGBProp::Color ColorRGBProp::DecodeColor(const String &colorString) {
    Color color{};
    color.red = colorString.substring(0, colorString.indexOf(',')).toInt();
    color.green = colorString.substring(colorString.indexOf(',') + 1, colorString.lastIndexOf(',')).toInt();
    color.blue = colorString.substring(colorString.lastIndexOf(',') + 1).toInt();
    return color;
}

String ColorRGBProp::EncodeColor(ColorRGBProp::Color colorStruct) {
    char buff[13];
    sprintf(buff, "%d,%d,%d", colorStruct.red, colorStruct.green, colorStruct.blue);
    return String(buff);
}
