#ifndef ELEVATOR_PANEL_H
#define  ELEVATOR_PANEL_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QFrame>

#include <iostream>
#include <string>

#include "include/Utility.h"

class FloorPressButton
{

};


class ElevatorPanel : public QWidget
{
    Q_OBJECT

public:
    ElevatorPanel(  const std::string label, 
                    const int numFloors,
                    QWidget *parent = nullptr); 
    virtual ~ElevatorPanel();   

    std::string GetDisplayMsg() {return displayMsg_;}
    void SetDisplayMsg(std::string msg) {displayMsg_ = msg;}

    ServiceRequestVector PopRequests();

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
    
    std::string displayMsg_ = "All Go";
    bool isOverloaded_ = false;
    bool isObstructed_ = false;
};

#endif