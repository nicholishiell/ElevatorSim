#ifndef ELEVATOR_H
#define  ELEVATOR_H

#include <QObject>

#include <iostream>
#include <vector>
#include <list>

#include "include/Utility.h"
#include "include/Person.h"
#include "include/ElevatorPanel.h"
#include "include/ElevatorState.h"
#include "include/Observer.h"

class Elevator : public QObject, public std::enable_shared_from_this<Elevator>
{
    Q_OBJECT

public:
    Elevator(const std::string label, DoorState doorState, const float height); 
    virtual ~Elevator();   
    
    std::string GetLabel() const {return label_;}

    float GetHeight() const {return height_;}
    void SetHeight(const float height);
    void UpdateHeight(const float deltaH);

    std::string GetStateString() const;
   
    ElevatorPanelSharedPtr GetPanel() const {return panel_;}
    void SetPanel(ElevatorPanelSharedPtr panel);

    DoorState GetDoorState() const {return doorState_;}
    void OpenDoor();
    void CloseDoor();

    // Updates the state and position of the elevator
    void Update(const float timeStep);

    void ToConsole() const;

    // Used in case of emergencies.
    void Disable();
    void Enable();

    void AddObserver(Observer * obs){observers_.push_back(obs);}
    void RemoveObserver(Observer * obs){observers_.remove(obs);}

    bool IsAtFloor(const int floorIndex) const;

 private:

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Private methods

    bool checkOverloaded();

    bool checkDoorObstructed();

    void notifyObservers();

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // People currently on the elvators
    PersonVector passengers_;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Panel that controls the elevator
    ElevatorPanelSharedPtr panel_;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Everything related to the state
    ElevatorState* elevatorState_;

    bool doorObstructed_;
    DoorState doorState_;
  
    float height_;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // other private members
    std::string label_;

    std::list<Observer*> observers_;
};



#endif