#ifndef BUILDING_PANEL_H
#define BUILDING_PANEL_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>

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

    ElevatorPanelSharedPtr GetCallingPanel() const {return callingPanel_;}
    void EndCall();
   
private slots:
    
    void SoundFireAlarm(); 
    void SoundPowerOutageAlarm();
    void EnabledPressed();
    void HandleAnswerPressed();
    void HandleObstructPressed();

public slots:

    void HelpRequested(ElevatorPanelSharedPtr panel);

signals:

    void EmergencyRequested(const EmergencyRequest request);

    void EnableElevators();

    void AnswerHelpRequest(ElevatorPanelSharedPtr panel);

    void ObstructDoor(const int floorIndex);

private:

    QComboBox* floorSelector_;
    
    QLabel* callLabel_;
    
    bool isFireAlarmActive_ = false;
    bool isPowerOutageAlarmActive_ = false;

    ElevatorPanelSharedPtr callingPanel_;
};

#endif