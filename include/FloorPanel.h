#ifndef FLOOR_PANEL_H
#define FLOOR_PANEL_H

#include <QObject>
#include <QWidget>
#include <QPushButton>

#include <iostream>

#include "include/Utility.h"

class FloorPanel : public QWidget
{
    Q_OBJECT

public:
    FloorPanel( const std::string label,
                const int numElevators, 
                const int level,
                QWidget *parent = nullptr); 

    virtual ~FloorPanel();   

    bool GetDownRequested() const {return downRequested_;}
    

    bool GetUpRequested() const {return upRequested_;}

    LightState GetElevatorLightState(const int index);   
    void SetElevatorLight(const int elevatorIndex, LightState newState);
    void LightsOut();

    bool IsUpRequested() const {return upRequested_;}
    bool IsDownRequested() const {return downRequested_;}

public slots:

    void UpButtonPresssed();
    void DownButtonPresssed();
 
    void DownRequestServiced(); 
    void UpRequestServiced();

signals:

    void ServiceRequested(const ServiceRequest& request);

private:

    int level_;

    LightStateVector elevatorLights_;

    bool upRequested_ = false;
    bool downRequested_ = false;
};

#endif