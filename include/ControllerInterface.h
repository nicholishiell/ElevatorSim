#ifndef CONTROLLER_INTERFACE_H
#define  CONTROLLER_INTERFACE_H

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

class ControllerInterface : public QObject
{
    Q_OBJECT

public:
    ControllerInterface(); 
    virtual ~ControllerInterface();   

    void AddFloor(std::string label);
    void AddElevator(std::string label, int floor, DoorState state);

    int GetNumberOfFloors() const {return static_cast<int>(floors_.size());}
    int GetNumberOfElevators() const {return static_cast<int>(elevators_.size());}

    ElevatorSharedPtr GetElevator(const int i) const {return elevators_[i];}
    FloorSharedPtr GetFloor(const int i) const {return floors_[i];}

    void Step(const float timeStep);

public slots:

    void HandleServiceRequest(const ServiceRequest request);

    void HandleFireAlarm(const int level);

    void HandlePowerOutageAlarm();

protected:

    ElevatorSharedPtrVector elevators_;
    FloorSharedPtrVector floors_;

    BuildingPanelSharedPtr buildingPanel_;

    ServiceRequestVector pendingRequests_;

private:

    void mandatoryStep(const float timeStep);
    
    virtual void userStep(const float timeStep) = 0;

    virtual void handleFireAlarm(const int level) = 0;

    virtual void handlePowerOutageAlarm() = 0;
};

#endif