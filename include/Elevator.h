#ifndef ELEVATOR_H
#define  ELEVATOR_H

#include <QObject>

#include <iostream>
#include <vector>

#include "include/Utility.h"
#include "include/ElevatorPanel.h"

class Elevator : public QObject
{
    Q_OBJECT

public:
    Elevator(const std::string label, DoorState doorState, int level, int numFloors); 
    virtual ~Elevator();   

    // Getters and Setters
    int GetLevel() const {return currentLevel_;}
    
    float GetHeight() const {return height_;}
    
    ElevatorState GetState() const {return elevatorState_;}

    DoorState GetDoorState() const {return doorState_;}

    std::string GetLabel() const {return label_;}

    void SetNumberOfFloors(const int numFloors) {numberOfFloors_ = numFloors;}

    ServiceRequest GetCurrentlyServicing() const {return currentlyServicing_;}

    ElevatorPanelSharedPtr GetPanel() const {return panel_;}

    std::string StateString() const;

    // Functions used to add routes to the elevator  
    void AddToRoute(const ServiceRequest& request);

    // Updates the state and position of the elevator
    void Update(const float timeStep);

private:

    void up(const float timeStep);
    void down(const float timeStep);
    void arrived(const float timeStep);
    void idle(const float timeStep);
    void waiting(const float timeStep);
    void stopped(const float timeStep);
    void leaving(const float timeStep);

    void updateCurrentLevel();

    ServiceRequest popRoute();

    DoorState doorState_;
    int currentLevel_;

    int numberOfFloors_;

    float height_;
    float speed_;
    std::string label_;

    float timeSpentWaiting_ = 0.;

    std::vector<ServiceRequest> route_;
    ServiceRequest currentlyServicing_;

    ElevatorState elevatorState_;

    ElevatorPanelSharedPtr panel_;
};



#endif