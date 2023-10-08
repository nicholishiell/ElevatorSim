#ifndef CONTROLLER_H
#define  CONTROLLER_H

#include <QObject>

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cmath>

#include "include/Utility.h"
#include "include/Floor.h"
#include "include/FloorPanel.h"
#include "include/Elevator.h"

class Controller : public QObject
{
    Q_OBJECT

public:
    Controller(); 
    virtual ~Controller();   

    void AddFloor(std::string label);
    void AddElevator(std::string label, int floor, DoorState state);

    int GetNumberOfFloors() const {return static_cast<int>(floors_.size());}
    int GetNumberOfElevators() const {return static_cast<int>(elevators_.size());}

    ElevatorSharedPtr GetElevator(const int i) const {return elevators_[i];}
    FloorSharedPtr GetFloor(const int i) const {return floors_[i];}

    void Update(const float timeStep);

public slots:

    void HandleUpRequest(const int level);
    void HandleDownRequest(const int level);

private:

    void updateFloorPanels();

    void assignServiceRequests();

    ElevatorSharedPtrVector elevators_;
    FloorSharedPtrVector floors_;
    
    ServiceRequestVector pendingRequests_;
};

#endif