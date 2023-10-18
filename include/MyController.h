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

    // Concrete definition 
    void Step(  const float timeStep, 
                ElevatorSharedPtrVector elevators,
                FloorSharedPtrVector floors);
    
    // Concrete definition 
    void HandleServiceRequest(  const ServiceRequest request, 
                                ElevatorSharedPtrVector elevators,
                                FloorSharedPtrVector floors);
    
    // Concrete definition 
    void HandleFireAlarm(   const int level,
                            ElevatorSharedPtrVector elevators,
                            FloorSharedPtrVector floors);

    // Concrete definition 
    void HandlePowerOutageAlarm(ElevatorSharedPtrVector elevators,
                                FloorSharedPtrVector floors);

private:

    ServiceRequestVector pendingRequests_;

    // Helper functions for Step implementation
    void updateFloorPanels( ElevatorSharedPtrVector elevators,
                            FloorSharedPtrVector floors);

    void assignServiceRequests( ElevatorSharedPtrVector elevators,
                                FloorSharedPtrVector floors);
};

#endif