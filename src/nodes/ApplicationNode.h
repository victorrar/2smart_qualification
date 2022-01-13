//
// Created by Victor on 13.01.2022.
//

#ifndef INC_2SMART_QUALIFICATION_APPLICATIONNODE_H
#define INC_2SMART_QUALIFICATION_APPLICATIONNODE_H

#include <SimpleTimer.h>
#include <HCSR04.h>
#include <deque>

#include "config.h"
#include "node/node.h"


class ApplicationNode : public Node {
    using Node::Node;
public:
    virtual ~ApplicationNode();

    static String GetModes();

public:
    bool Init(Homie *homie) override;

    void HandleCurrentState() override;

    void sonarCallback();

    enum class modes {
        Static,
        Threshold,
        Doppler,
    };
    const static std::map<modes, String> modesMap;

protected:
    SimpleTimer *sonarTimer;
    UltraSonicDistanceSensor *hc;
    ApplicationNode::modes currentMode = modes::Static;
    std::deque<float> distanceDeque;
    bool sonarFlag = false;

};


#endif //INC_2SMART_QUALIFICATION_APPLICATIONNODE_H
