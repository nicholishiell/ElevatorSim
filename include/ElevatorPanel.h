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

class ElevatorPanel : public QWidget
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
   
    int GetLevel() const {return currentLevel_;}
    void SetLevel(const int level);
    void CalculateCurrentLevel(const float height);
    int GetNumberOfFloors() const {return numberOfFloors_;}
    int GetNearestLevel(const float height);

    ServiceRequest GetCurrentlyServicing() const {return currentlyServicing_;}
    void SetCurrentlyServicing(const ServiceRequest& r) {currentlyServicing_ = r;}
    bool IsGoingDown() const {return currentlyServicing_.direction == RequestDirection::REQ_DOWN;}


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

    void EmergencyRequested(const EmergencyRequest& request);
    
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
    int currentLevel_;
    int numberOfFloors_;
};

#endif