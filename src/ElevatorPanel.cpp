#include "include/ElevatorPanel.h"
#include "include/FloorButton.h"

#include <cmath>
#include <limits>
#include <algorithm>

#include <QGridLayout>
#include <QPixmap>
#include <QLabel>
#include <QString>

ElevatorPanel::ElevatorPanel(   const std::string label, 
                                const int numFloors,
                                ElevatorPositionSensorSharedPtr floorSensor,
                                QWidget *parent) : QWidget(parent)
{
    // Create all the compulsory buttons
    auto gridLayout = new QGridLayout(this);
    auto helpButton = new QPushButton("HELP", this);
    auto openButton = new QPushButton("OP", this);
    auto closeButton = new QPushButton("CD", this);

    gridLayout->addWidget(helpButton,0,0,1,2);
    gridLayout->addWidget(openButton,1,0);
    gridLayout->addWidget(closeButton,1,1);

    QObject::connect(helpButton, &QPushButton::clicked, this, &ElevatorPanel::HelpButtonPresssed);
    
    QObject::connect(openButton, &QPushButton::pressed, this, &ElevatorPanel::OpenButtonInteraction);
    QObject::connect(openButton, &QPushButton::released, this, &ElevatorPanel::OpenButtonInteraction);
    
    QObject::connect(closeButton, &QPushButton::pressed, this, &ElevatorPanel::CloseButtonInteraction);
    QObject::connect(closeButton, &QPushButton::released, this, &ElevatorPanel::CloseButtonInteraction);

    auto line1 = new QFrame;
    line1->setFrameShape(QFrame::HLine);
    gridLayout->addWidget(line1,2,0,1,2);

    // Create labels for overloaded, obstructed, and display msg
    obstructedLabel_ = new QLabel("    OB    ");
    overloadLabel_ = new QLabel("    OL    ");

    gridLayout->addWidget(obstructedLabel_,3,0,Qt::AlignHCenter);
    gridLayout->addWidget(overloadLabel_,3,1,Qt::AlignHCenter);

    this->SetDoorObstructedState(false);
    this->SetOverloadState(false);

    auto line2 = new QFrame;
    line2->setFrameShape(QFrame::HLine);
    gridLayout->addWidget(line2,4,0,1,2);

    displayLabel_ = new QLabel(QString::fromStdString("Display Msg"));
    gridLayout->addWidget(displayLabel_,5,0,1,2,Qt::AlignHCenter);

    auto line3 = new QFrame;
    line3->setFrameShape(QFrame::HLine);
    gridLayout->addWidget(line3,6,0,1,2);

    bell_ = new AnimatedImage("./images/bell/bell_", 9, 0.1, this);
    gridLayout->addWidget(bell_,7,0,Qt::AlignHCenter);

    speaker_ = new AnimatedImage("./images/speaker/speaker_", 12, 0.15, this);
    gridLayout->addWidget(speaker_,7,1,Qt::AlignHCenter);

    auto line4 = new QFrame;
    line4->setFrameShape(QFrame::HLine);
    gridLayout->addWidget(line4,8,0,1,2);

    // Create the floor buttons
    std::vector<QPushButton*> floorButtons;
    auto row = 9;
    auto col = 0;
    for(int i = 0; i < numFloors; i++)
    {
        auto buttonLabel = std::to_string(i);
        auto floorButton = new FloorButton(i,QString::fromStdString(buttonLabel),this);
        
        if(i%2==0)
        {
            row++;
            col = 0;
        }
        else
            col = 1;
        gridLayout->addWidget(floorButton,row,col);

        QObject::connect(floorButton, &FloorButton::ServiceRequested, this, &ElevatorPanel::ServiceFloorButtonRequest);
    }
       
    this->setLayout(gridLayout);
    this->setWindowTitle(QString::fromStdString(label));

    this->show();

    openDoor_ = false;
    closeDoor_ = false;
 
    label_ = label;

    floorSensor_ = floorSensor;
}

ElevatorPanel::~ElevatorPanel()
{

} 

std::string 
ElevatorPanel::FloorPosString() const
{
    return std::string( std::to_string(GetPreviousFloor()) + ", " +
                        std::to_string(GetNextFloor()));
}

ServiceRequest 
ElevatorPanel::GetCurrentlyServicing() const 
{
    ServiceRequest currentlyServicing(this->GetPreviousFloor(), RequestDirection::REQ_IDLE);

    if(!route_.empty())
        currentlyServicing = route_[0];

    return currentlyServicing;
}
    
bool 
ElevatorPanel::IsGoingUp() const 
{
    auto currentlyServicing = this->GetCurrentlyServicing();

    return  currentlyServicing.direction == RequestDirection::REQ_UP || 
            currentlyServicing.direction == RequestDirection::REQ_IDLE;
}

bool 
ElevatorPanel::IsGoingDown() const 
{
    auto currentlyServicing = this->GetCurrentlyServicing();

    return  currentlyServicing.direction == RequestDirection::REQ_DOWN || 
            currentlyServicing.direction == RequestDirection::REQ_IDLE;
}


void 
ElevatorPanel::SetCurrentlyServicing(const ServiceRequest& r) 
{
    route_.insert(route_.begin(), r);
}

bool 
ElevatorPanel::IsAtTargetFloor() const
{
    return floorSensor_->HasArrivedAtFloor(this->GetCurrentlyServicing().level);
}

void 
ElevatorPanel::GoToFloor(const int i)
{
    if(floorSensor_->HasArrivedAtFloor(i))
    {
        this->SetCurrentlyServicing(ServiceRequest(i, RequestDirection::REQ_IDLE));
    }
    else if(i > GetPreviousFloor())
    {
        this->SetCurrentlyServicing(ServiceRequest(i, RequestDirection::REQ_UP));
    }
    else
    {
        this->SetCurrentlyServicing(ServiceRequest(i, RequestDirection::REQ_DOWN));
    }

}

void 
ElevatorPanel::DisplayRoute() const
{
    for(auto r : route_)
        std::cout << "(" << r.level << ", " << r.direction << ") ";
    std::cout << std::endl;
}

void 
ElevatorPanel::RingBell()
{
    bell_->Animate();
}


void 
ElevatorPanel::DisplayMessage(const std::string& msg)
{
    displayLabel_->setText(QString::fromStdString(msg));
    displayLabel_->repaint();
}

void 
ElevatorPanel::AudioMessage()
{
    speaker_->Animate();
}

void 
ElevatorPanel::SetOverloadState(const bool s)
{
    if(s)
        overloadLabel_->setStyleSheet("QLabel { background-color : red; color : black; }");
    else
        overloadLabel_->setStyleSheet("QLabel { background-color : green; color : black; }");
    
    overloadLabel_->repaint();
}

void 
ElevatorPanel::SetDoorObstructedState(const bool s)
{
    if(s)
        obstructedLabel_->setStyleSheet("QLabel { background-color : red; color : black; }");
    else
        obstructedLabel_->setStyleSheet("QLabel { background-color : green; color : black; }");
    
    obstructedLabel_->repaint();
}

void 
ElevatorPanel::OpenButtonInteraction()
{
    if(openDoor_)
    {
        openDoor_ = false;
    }
    else
    {
        openDoor_ = true;
    }
}

void 
ElevatorPanel::CloseButtonInteraction()
{
    if(closeDoor_)
    {
        closeDoor_ = false;
    }
    else
    {
        closeDoor_ = true;
    }
}

void 
ElevatorPanel::HelpButtonPresssed()
{
    emit(HelpRequested(shared_from_this()));
}

int
ElevatorPanel::GetPreviousFloor() const
{
    return floorSensor_->GetFloorPosition().prevFloor;
}

int 
ElevatorPanel::GetNextFloor() const
{
    return floorSensor_->GetFloorPosition().nextFloor;
}

void 
ElevatorPanel::ServiceFloorButtonRequest(const int destinationFloor)
{
    ServiceRequest request(destinationFloor, RequestDirection::REQ_IDLE);
    
    if(destinationFloor > GetPreviousFloor())
        request.direction = RequestDirection::REQ_UP;
    else
        request.direction = RequestDirection::REQ_DOWN;
    
    this->AddToRoute(request);
}


void 
ElevatorPanel::AddToRoute(const ServiceRequest& request)
{
    // Only add a level once to the route
    if(std::find_if(route_.begin(), route_.end(), special_compare(request)) == route_.end())  
    {
        auto currentlyServicing = this->GetCurrentlyServicing();

        if(request.direction == currentlyServicing.direction || 
            currentlyServicing.direction == RequestDirection::REQ_IDLE)
        {
            this->addRequest(request);
        }
    }
}

void 
ElevatorPanel::addRequest(const ServiceRequest& request)
{
    auto isGoingUp = this->IsGoingUp();

    route_.emplace_back(request);
    std::sort(route_.begin(), route_.end());

    if(!isGoingUp)
        std::reverse(route_.begin(),route_.end()); 
}

void 
ElevatorPanel::Arrived()
{
    if(!this->IsRouteEmpty())
    {
        route_.erase(route_.begin());
    }  
}

void 
ElevatorPanel::Notify()
{
    this->DisplayMessage(std::to_string(GetPreviousFloor()));
}

bool
ElevatorPanel::IsAtFloor(const int floorIndex) const
{
    return floorSensor_->HasArrivedAtFloor(floorIndex);
}