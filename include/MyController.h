#ifndef MY_CONTROLLER_H
#define MY_CONTROLLER_H

#include <QObject>

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cmath>

#include "include/ControllerInterface.h"

class MyController : public ControllerInterface
{

public:
    MyController(); 
    virtual ~MyController();   

private:

    // Concrete definitions of pure virtual functions inherited from Controller Interface
    void handleFireAlarm(const int level);

    void handlePowerOutageAlarm();

    void userStep(const float timeStep);

    // Helper functions for userStep implementation
    void updateFloorPanels();

    void assignServiceRequests();

};

#endif