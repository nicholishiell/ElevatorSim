#include <QGridLayout>
#include <QPixmap>
#include <QLabel>
#include <QString>

#include "include/FloorPanel.h"

FloorPanel::FloorPanel( const std::string label,
                        const int numElevators, 
                        const int level,
                        QWidget *parent) : QWidget(parent)
{
    level_ = level;

    for(int i = 0; i < numElevators; i++)
    {
        elevatorLights_.emplace_back(LightState::OFF);
    }

    // Setup GUI
    auto gridLayout = new QGridLayout(this);
    auto upButton = new QPushButton("UP", this);
    auto downButton = new QPushButton("DW", this);
    
    gridLayout->addWidget(upButton,0,0);
    gridLayout->addWidget(downButton,0,1);

    QObject::connect(upButton, &QPushButton::clicked, this, &FloorPanel::UpButtonPresssed);
    QObject::connect(downButton, &QPushButton::clicked, this, &FloorPanel::DownButtonPresssed);

    this->setLayout(gridLayout);
    this->setWindowTitle(QString::fromStdString(label));
  
    this->show();
}

FloorPanel::~FloorPanel()
{

}

LightState 
FloorPanel::GetElevatorLightState(const int index)
{
    auto retVal = LightState::UNKNOWN;

    if(index < static_cast<int>(elevatorLights_.size()))
        retVal = elevatorLights_[index];

    return retVal;    
}

void 
FloorPanel::SetElevatorLight(const int elevatorIndex, LightState newState)
{
    if(elevatorIndex < static_cast<int>(elevatorLights_.size()))
        elevatorLights_[elevatorIndex] = newState;
    else
        std::cout << "FloorPanel::ChangeElevatorLight::ERROR: index out of bounds" << std::endl;
}

void 
FloorPanel::LightsOut()
{
   std::fill(elevatorLights_.begin(), elevatorLights_.end(), LightState::OFF);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Slots  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void 
FloorPanel::DownRequestServiced()
{
    downRequested_ = false;
}

void 
FloorPanel::UpRequestServiced()
{
    upRequested_ = false;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Slots  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void 
FloorPanel::UpButtonPresssed()
{
    emit ServiceRequested(ServiceRequest(level_, RequestDirection::REQ_UP));
    upRequested_ = true;
}

void 
FloorPanel::DownButtonPresssed()
{
    emit ServiceRequested(ServiceRequest(level_, RequestDirection::REQ_DOWN));
    downRequested_ = true;
}