#include "include/BuildingPanel.h"

#include <QGridLayout>
#include <QPushButton>

BuildingPanel::BuildingPanel(QWidget *parent): QWidget(parent)
{
    auto gridLayout = new QGridLayout(this);
    auto fireButton = new QPushButton("FIRE", this);
    auto powerOutageButton = new QPushButton("PO", this);
    
    gridLayout->addWidget(fireButton,0,0);
    gridLayout->addWidget(powerOutageButton,0,1);

    QObject::connect(fireButton, &QPushButton::clicked, this, &BuildingPanel::SoundFireAlarm);
    QObject::connect(powerOutageButton, &QPushButton::clicked, this, &BuildingPanel::SoundPowerOutageAlarm);

    this->setLayout(gridLayout);
    this->setWindowTitle(QString::fromStdString("Building Panel"));

    this->show();
}

BuildingPanel::~BuildingPanel()
{

}

void 
BuildingPanel::SoundFireAlarm()
{
    if(isFireAlarmActive_)
    {
       isFireAlarmActive_ = false; 
    }
    else
    {
        emit (EmergencyRequested(EmergencyRequest(-1,EmergencyType::FIRE)));
        isFireAlarmActive_ = true;
    }
}

void BuildingPanel::SoundPowerOutageAlarm()
{
    if(isPowerOutageAlarmActive_)
    {
       isPowerOutageAlarmActive_ = false; 
    }
    else
    {
         emit (EmergencyRequested(EmergencyRequest(-1,EmergencyType::POWER_OUTAGE)));
        isPowerOutageAlarmActive_= true;
    }
}

