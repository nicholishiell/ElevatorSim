#include "include/BuildingPanel.h"

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>

BuildingPanel::BuildingPanel(   const int numFloors,
                                QWidget *parent): QWidget(parent)
{
    auto gridLayout = new QGridLayout(this);
    auto fireButton = new QPushButton("FIRE", this);
    auto powerOutageButton = new QPushButton("PW Out", this);
    auto enableButton = new QPushButton("ENABLE", this);
    auto obstructorButton = new QPushButton("OBST FLR", this);
    auto answerButton = new QPushButton("ANSWER", this);
    callLabel_ = new QLabel("CALL INCOMING",this);
    floorSelector_ = new QComboBox();

    gridLayout->addWidget(fireButton,0,0);
    gridLayout->addWidget(answerButton,0,1);
    gridLayout->addWidget(powerOutageButton,1,0);
    gridLayout->addWidget(obstructorButton,1,1);
    gridLayout->addWidget(enableButton,2,0,1,2);
    gridLayout->addWidget(floorSelector_,3,0,1,2);
    gridLayout->addWidget(callLabel_,4,0,1,2,Qt::AlignHCenter);

    QObject::connect(fireButton, &QPushButton::clicked, this, &BuildingPanel::SoundFireAlarm);
    QObject::connect(powerOutageButton, &QPushButton::clicked, this, &BuildingPanel::SoundPowerOutageAlarm);
    QObject::connect(enableButton, &QPushButton::clicked, this, &BuildingPanel::EnabledPressed);
    QObject::connect(answerButton, &QPushButton::clicked, this, &BuildingPanel::HandleAnswerPressed);
    QObject::connect(obstructorButton, &QPushButton::clicked, this, &BuildingPanel::HandleObstructPressed);

    for(int i = 0; i < numFloors; i++)
        floorSelector_->addItem(QString::number(i));

    this->setLayout(gridLayout);
    this->setWindowTitle(QString::fromStdString("Building Panel"));

    this->show();

    callingPanel_ = nullptr;
}

BuildingPanel::~BuildingPanel()
{

}

void 
BuildingPanel::HandleObstructPressed()
{
    emit (ObstructDoor(floorSelector_->currentIndex()));
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
        emit (EmergencyRequested(EmergencyRequest(floorSelector_->currentIndex(),EmergencyType::FIRE)));
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

void
BuildingPanel::EnabledPressed()
{
    emit(EnableElevators());
}

void 
BuildingPanel::HelpRequested(ElevatorPanelSharedPtr panel)
{
    callingPanel_ = panel;
    callLabel_->setStyleSheet("QLabel { background-color : red; color : black; }");
          
    callLabel_->repaint();
}

void 
BuildingPanel::HandleAnswerPressed()
{
    if(callingPanel_ != nullptr)
    {
        emit(AnswerHelpRequest(callingPanel_));

        EndCall();
    }
}

void
BuildingPanel::EndCall()
{
    callLabel_->setStyleSheet("QLabel { background-color : lightGray; color : black; }");
    callLabel_->repaint();  

    callingPanel_ = nullptr;
}