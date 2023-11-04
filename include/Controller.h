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
#include "include/BuildingSimulator.h"

class Controller : public QObject
{
    Q_OBJECT

public:
    Controller(); 
    virtual ~Controller(); 

    virtual void Step(  const float timeStep, 
                        ElevatorSharedPtrVector elevators,
                        BuildingPanelSharedPtr buildingPanel) = 0;
    
    virtual void HandleServiceRequest(  const ServiceRequest request, 
                                        ElevatorSharedPtrVector elevators,
                                        FloorSharedPtrVector floors) = 0;
    
    virtual void HandleFireAlarm(   const int level,
                                    ElevatorSharedPtrVector elevators,
                                    FloorSharedPtrVector floors) = 0;

    virtual void HandlePowerOutageAlarm(ElevatorSharedPtrVector elevators,
                                        FloorSharedPtrVector floors) = 0;
    
    virtual void HandleHelpRequest( ElevatorPanelSharedPtr elevatorPanel,
                                    BuildingPanelSharedPtr buildingPanel) = 0;
    
    virtual void HandleAnswerHelp( ElevatorPanelSharedPtr elevatorPanel) = 0;

};

#endif