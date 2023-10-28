#ifndef ELEVATOR_H
#define  ELEVATOR_H

#include <QObject>

#include <iostream>
#include <vector>

#include "include/Utility.h"
#include "include/Person.h"
#include "include/ElevatorPanel.h"
#include "include/ElevatorState.h"

class Elevator : public QObject, public std::enable_shared_from_this<Elevator>
{
    Q_OBJECT

public:
    Elevator(const std::string label, DoorState doorState, const float height); 
    virtual ~Elevator();   

    int GetLevel() const {return currentLevel_;}
    
    float GetHeight() const {return height_;}
    void SetHeight(const float height){height_ = height;}
    void UpdateHeight(const float deltaH);

    std::string GetStateString() const;

    DoorState GetDoorState() const {return doorState_;}

    std::string GetLabel() const {return label_;}

    void SetNumberOfFloors(const int numFloors) {numberOfFloors_ = numFloors;}

    ServiceRequest GetCurrentlyServicing() const {return currentlyServicing_;}
    void SetCurrentlyServicing(const ServiceRequest& r) {currentlyServicing_ = r;}

    ElevatorPanelSharedPtr GetPanel() const {return panel_;}
    void SetPanel(ElevatorPanelSharedPtr panel){panel_ = panel;}

    std::string StateString() const;

    void OpenDoor();

    void CloseDoor();

    bool IsRouteEmpty() const {return route_.empty();}

    // Functions used to add routes to the elevator  
    void AddToRoute(const ServiceRequest& request);

    // Updates the state and position of the elevator
    void Update(const float timeStep);

    void UpdateCurrentLevel();

    ServiceRequest PopRoute();

    void ToConsole() const;

 private:

    // Private methods
    void handlePendingRequests();

    bool checkOverloaded();

    bool checkDoorObstructed();

    // Private members
    DoorState doorState_;
  
    float height_;

    bool doorObstructed_;

    std::string label_;

    ElevatorState* elevatorState_;

    PersonVector passengers_;

    // all these should be stored in the panel
    ElevatorPanelSharedPtr panel_;
    std::vector<ServiceRequest> route_;
    ServiceRequest currentlyServicing_;
    int currentLevel_;

    // this should be gotten rid off
    int numberOfFloors_;    
};



#endif