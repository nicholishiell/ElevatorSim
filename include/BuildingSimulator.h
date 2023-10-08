#ifndef BUILDING_SIM_H
#define BUILDING_SIM_H

#include <QObject>

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cmath>

#include "include/Utility.h"
#include "include/Floor.h"
#include "include/Elevator.h"
#include "include/Controller.h"
#include "include/FloorPanel.h"
#include "include/ElevatorPanel.h"
#include "include/BuildingPanel.h"

class BuildingSimulator : public QObject
{
    Q_OBJECT

public:
    BuildingSimulator(ControllerSharedPtr controller); 
    virtual ~BuildingSimulator();   

    void AddFloor(std::string label);
    void AddElevator(std::string label, int floor, DoorState state);

    int GetNumberOfFloors() const {return static_cast<int>(floors_.size());}
    int GetNumberOfElevators() const {return static_cast<int>(elevators_.size());}

    ElevatorSharedPtr GetElevator(const int i) const {return elevators_[i];}
    FloorSharedPtr GetFloor(const int i) const {return floors_[i];}

    ControllerSharedPtr GetController() const {return controller_;}

    void Step(const float timeStep);

public slots:

    void HandleServiceRequest(const ServiceRequest request);
    void HandleFireAlarm(const int level);
    void HandlePowerOutageAlarm();

private:

    // Private Methods
    void addElevator(ElevatorSharedPtr elevator);
    void addFloor(FloorSharedPtr floor);

    void mandatoryStep(const float timeStep);
    
    // Private Members
    ElevatorSharedPtrVector elevators_;
    FloorSharedPtrVector floors_;

    BuildingPanelSharedPtr buildingPanel_;

    ControllerSharedPtr controller_;
};

#endif