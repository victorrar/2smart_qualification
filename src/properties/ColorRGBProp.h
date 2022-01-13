//
// Created by Victor on 13.01.2022.
//

#ifndef INC_2SMART_QUALIFICATION_COLORRGBPROP_H
#define INC_2SMART_QUALIFICATION_COLORRGBPROP_H


#include "property/property.h"

class ColorRGBProp : public Property {
public:
    struct Color {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };

    ColorRGBProp(const char *name, const char *id, Node *node, PROPERTY_TYPE type, bool settable,
                 bool retained);

    bool Init(Homie *homie) override;

    void HandleCurrentState() override;

    //Called both on receive and send
    //I think it should have params of old/new value and invoker (user app or framework)
    void HandleSettingNewValue() override;

    Color GetColorStruct();

    void SetColorStruct(Color colorStruct);

protected:
    Color color = {};

private:
    Color DecodeColor(const String &colorString);

    String EncodeColor(Color colorStruct);
};


#endif //INC_2SMART_QUALIFICATION_COLORRGBPROP_H
