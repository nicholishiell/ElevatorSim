#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cmath>

#include "include/Utility.h"
#include "include/Floor.h"
#include "include/Elevator.h"

#include "include/FloorPanel.h"
#include "include/ElevatorPanel.h"
#include "include/BuildingPanel.h"

class Controller : public QObject
{
    Q_OBJECT

public:
    Controller(); 
    virtual ~Controller(); 

    void AddServiceRequest(const ServiceRequest request);  

    void AddFloor(FloorSharedPtr floor) {floors_.emplace_back(floor);}
    void AddElevator(ElevatorSharedPtr elevator) {elevators_.emplace_back(elevator);}

    virtual void Step(const float timeStep) = 0;
    
    virtual void HandleFireAlarm(const int level) = 0;

    virtual void HandlePowerOutageAlarm() = 0;

protected:
   
    // Protected Members

    ServiceRequestVector pendingRequests_;

    ElevatorSharedPtrVector elevators_;
    
    FloorSharedPtrVector floors_;
};

#endif