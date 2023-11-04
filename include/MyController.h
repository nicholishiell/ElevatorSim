#ifndef MY_CONTROLLER_H
#define MY_CONTROLLER_H

#include <QObject>

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cmath>
#include <chrono>

#include "include/Controller.h"

class MyController : public Controller
{

public:
    MyController(); 
    virtual ~MyController();   

    // Concrete definition 
    void Step(  const float timeStep, 
                ElevatorSharedPtrVector elevators,
                BuildingPanelSharedPtr buildingPanel);
    
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
    void HandleHelpRequest( ElevatorPanelSharedPtr elevatorPanel,
                            BuildingPanelSharedPtr buildingPanel);
    
    // Concrete definition 
    void HandleAnswerHelp(ElevatorPanelSharedPtr elevatorPanel);

private:

    ServiceRequestVector pendingRequests_;

    TimeStamp calledTimeStamp_;
    bool helpRequested_;

    void assignServiceRequests( ElevatorSharedPtrVector elevators);
};

#endif