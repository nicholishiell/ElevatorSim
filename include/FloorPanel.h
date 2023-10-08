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
                const int level,
                QWidget *parent = nullptr); 

    virtual ~FloorPanel();   

    bool GetDownRequested() const {return downRequested_;}
    

    bool GetUpRequested() const {return upRequested_;}
   
    int GetNumberOfElevators() const {return static_cast<int>(elevatorLights_.size());}
    void SetNumberOfElevators(const int numElevators);

    LightState GetElevatorLightState(const int index);   
    void SetElevatorLight(const int elevatorIndex, LightState newState);
    void LightsOut();

private slots:

    void UpButtonPresssed();
    void DownButtonPresssed();

public slots:
    
    void DownRequestServiced(); 
    void UpRequestServiced();

signals:
    void UpRequested(const int level);
    void DownRequested(const int level);

private:

    int level_;
    std::string label_;

    LightStateVector elevatorLights_;

    bool upRequested_ = false;
    bool downRequested_ = false;
};

#endif