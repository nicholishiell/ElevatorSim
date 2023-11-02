#ifndef BUILDING_PANEL_H
#define BUILDING_PANEL_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QComboBox>

#include <iostream>

#include "include/Utility.h"

class BuildingPanel : public QWidget
{
    Q_OBJECT

public:
    BuildingPanel(const int numFloors, QWidget *parent = nullptr); 

    virtual ~BuildingPanel();   

    bool IsFireAlarmActive() const {return isFireAlarmActive_;}
    
    bool IsPowerOutageAlarmActive() const {return isPowerOutageAlarmActive_;}
   
private slots:
    
    void SoundFireAlarm(); 
    void SoundPowerOutageAlarm();

signals:

    void EmergencyRequested(const EmergencyRequest request);

private:

    QComboBox* floorSelector;

    bool isFireAlarmActive_ = false;
    bool isPowerOutageAlarmActive_ = false;
};

#endif