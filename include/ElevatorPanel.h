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

    ServiceRequestVector PopRequests();

    void RingBell();

    void DisplayMessage(const std::string& msg);

    void AudioMessage();

    void SetOverloadState(const bool s);
    void SetDoorObstructedState(const bool s);

private slots:

    void FireButtonPresssed();
    void HelpButtonPresssed();

    void OpenButtonPresssed();
    void CloseButtonPresssed();

    void FloorButtonPresssed();

private:

    ServiceRequestVector requests_;

    std::string label_;
    int numFloors_;
    
    QLabel* displayLabel_;

    QLabel* obstructedLabel_; 
    QLabel* overloadLabel_;

    AnimatedImage* bell_;
    AnimatedImage* speaker_;
};

#endif