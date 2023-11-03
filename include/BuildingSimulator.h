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

class GraphicsGenerator;

class BuildingSimulator : public QObject
{
    Q_OBJECT

public:
    BuildingSimulator(ControllerSharedPtr controller); 
    virtual ~BuildingSimulator();   

    // These functions are used to "construct" the building 
    void AddFloor(std::string label);
    void AddElevator(std::string label, DoorState state, const float height);

    // Run the simulation for a single time step
    void Update(const float timeStep);

    // All the methods under here are used by generate the graphics
    void SetGraphicsGenerator(GraphicsGenerator* gg){graphicsObserver_ = gg;}

    int GetNumberOfFloors() const {return static_cast<int>(floors_.size());}
    int GetNumberOfElevators() const {return static_cast<int>(elevators_.size());}

    ElevatorSharedPtr GetElevator(const int i) const {return elevators_[i];}
    FloorSharedPtr GetFloor(const int i) const {return floors_[i];}

    void Initialize();
    bool IsInitialized(){return initialized_;}

public slots:

    // These functions are supplied by the controller passed in (strategy pattern)
    void HandleServiceRequest(const ServiceRequest request);
    
    void HandleEmergencyRequest(const EmergencyRequest request);
    
    void HandleEnableElevators();
    
private:
   
    bool initialized_;

    void updateFloors(const float timeStep);
    void updateElevators(const float timeStep);
    void updatePeople(const float timeStep);

    // Private Members
    ElevatorSharedPtrVector elevators_;
    FloorSharedPtrVector floors_;

    BuildingPanelSharedPtr buildingPanel_;

    ControllerSharedPtr controller_;

    GraphicsGenerator* graphicsObserver_;
};

#endif