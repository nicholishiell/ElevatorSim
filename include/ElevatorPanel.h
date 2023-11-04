#ifndef ELEVATOR_PANEL_H
#define  ELEVATOR_PANEL_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QFrame>

#include <iostream>
#include <string>

#include "include/Utility.h"
#include "include/AnimatedImage.h"

class ElevatorPanel : public QWidget, public std::enable_shared_from_this<ElevatorPanel>
{
    Q_OBJECT

public:
    ElevatorPanel(  const std::string label, 
                    const int numFloors,
                    QWidget *parent = nullptr); 
                    
    virtual ~ElevatorPanel();   

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // All these functions relate to the GUI
    void RingBell();
    void DisplayMessage(const std::string& msg);

    void AudioMessage();

    void SetOverloadState(const bool s);
    void SetDoorObstructedState(const bool s);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // All these functions relate to operationing the elevator
    ServiceRequestVector PopRequests();
   
    int GetPreviousFloor() const {return floorPos_.prevFloor;}
    int GetNextFloor() const {return floorPos_.nextFloor;}

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // This functionality could all be moved into a "FloorSensor" object
    bool HasArrivedAtFloor(const int floor) const;
    bool ArrivedAtTargetFloor() const;
    bool IsBetweenFloors() const {return floorPos_.nextFloor != floorPos_.prevFloor;}
    bool AtTop() const {return HasArrivedAtFloor(numberOfFloors_);}
    bool AtBottom() const {return HasArrivedAtFloor(0);}
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    void CalculateCurrentLevel(const float height);
    
    int GetNumberOfFloors() const {return numberOfFloors_;}
    
    int GetNearestLevel(const float height);

    ServiceRequest GetCurrentlyServicing() const {return currentlyServicing_;}
    bool IsGoingDown() const {return currentlyServicing_.direction == RequestDirection::REQ_DOWN;}

    void SetCurrentlyServicing(const ServiceRequest& r) {currentlyServicing_ = r;}
    void GoToFloor(const int i);
        
    bool IsOpenDoorButtionActive() const {return openDoor_;}
    bool IsCloseDoorButtionActive() const {return closeDoor_;}

    // Functions used to add routes to the elevator  
    void AddToRoute(const ServiceRequest& request);
    bool IsRouteEmpty() const {return route_.empty();}
    void DisplayRoute() const;
    void PopRoute();

private slots:

    void HelpButtonPresssed();

    void OpenButtonInteraction();
    void CloseButtonInteraction();

    void FloorButtonPresssed();

signals:

    void HelpRequested(ElevatorPanelSharedPtr panel);
    
    void ServiceRequested(const ServiceRequest& request);

private:
    
    bool openDoor_;
    bool closeDoor_;
    
    QLabel* displayLabel_;

    QLabel* obstructedLabel_; 
    QLabel* overloadLabel_;

    AnimatedImage* bell_;
    AnimatedImage* speaker_;

    ServiceRequestVector route_;
    ServiceRequest currentlyServicing_;
    FloorPosition floorPos_;
    int numberOfFloors_;

    std::string label_;
};

#endif