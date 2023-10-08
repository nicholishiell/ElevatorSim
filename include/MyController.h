#ifndef MY_CONTROLLER_H
#define MY_CONTROLLER_H

#include <QObject>

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cmath>

#include "include/Controller.h"

class MyController : public Controller
{

public:
    MyController(); 
    virtual ~MyController();   

    // Concrete definitions of pure virtual functions inherited from Controller Interface
    void Step(const float timeStep);

    void HandleFireAlarm(const int level);

    void HandlePowerOutageAlarm();

private:

    // Helper functions for Step implementation
    void updateFloorPanels();

    void assignServiceRequests();

};

#endif