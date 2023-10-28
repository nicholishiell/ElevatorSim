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

    // Concrete definition 
    void HandleHelpRequest(const int level);

private:

    ServiceRequestVector pendingRequests_;

    void assignServiceRequests( ElevatorSharedPtrVector elevators);
};

#endif