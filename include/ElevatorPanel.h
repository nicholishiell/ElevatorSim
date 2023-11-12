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
#include "include/ElevatorPositionSensor.h"
#include "include/Observer.h"

class ElevatorPanel : public QWidget, public std::enable_shared_from_this<ElevatorPanel>, public Observer
{
    Q_OBJECT

public:
    ElevatorPanel(  const std::string label, 
                    const int numFloors,
                    ElevatorPositionSensorSharedPtr floorSensor,
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
   
    bool IsAtTargetFloor() const;
    int GetPreviousFloor() const;
    int GetNextFloor() const;

    ServiceRequest GetCurrentlyServicing() const {return currentlyServicing_;}
    
    bool IsGoingUp() const {return currentlyServicing_.direction == RequestDirection::REQ_UP || 
                                    currentlyServicing_.direction == RequestDirection::REQ_IDLE;}

    bool IsGoingDown() const {return currentlyServicing_.direction == RequestDirection::REQ_DOWN || 
                                    currentlyServicing_.direction == RequestDirection::REQ_IDLE;}


    void SetCurrentlyServicing(const ServiceRequest& r) {currentlyServicing_ = r;}
    void GoToFloor(const int i);

    bool IsAtFloor(const int floorIndex) const;

    bool IsOpenDoorButtionActive() const {return openDoor_;}
    bool IsCloseDoorButtionActive() const {return closeDoor_;}

    std::string FloorPosString() const;

    // Functions used to add routes to the elevator  
    void AddToRoute(const ServiceRequest& request);
    bool IsRouteEmpty() const {return route_.empty();}
    void DisplayRoute() const;
    void PopRoute();

    ElevatorPositionSensorSharedPtr GetSensor() const {return floorSensor_;}

    void Notify();

public slots:

    void HelpButtonPresssed();

    void OpenButtonInteraction();
    void CloseButtonInteraction();

    void ServiceFloorButtonRequest(const int destinationFloor);

signals:

    void HelpRequested(ElevatorPanelSharedPtr panel);
    
    // void ServiceRequested(const ServiceRequest& request);

private:
    
    void addRequest(const ServiceRequest& request);

    bool openDoor_;
    bool closeDoor_;
    
    QLabel* displayLabel_;

    QLabel* obstructedLabel_; 
    QLabel* overloadLabel_;

    AnimatedImage* bell_;
    AnimatedImage* speaker_;

    ServiceRequestVector route_;
    ServiceRequest currentlyServicing_;
    
    std::string label_;

    ElevatorPositionSensorSharedPtr floorSensor_;
};

#endif