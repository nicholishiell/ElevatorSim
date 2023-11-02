#include "include/ElevatorPanel.h"
#include "include/FloorButton.h"

#include <cmath>
#include <limits>

#include <QGridLayout>
#include <QPixmap>
#include <QLabel>
#include <QString>

std::string
int_to_string(const int i)
{
    std::stringstream ss;
    std::string s;
    ss << i;
    ss >> s;
    return s;
}

ElevatorPanel::ElevatorPanel(   const std::string label, 
                                const int numFloors,
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

        QObject::connect(floorButton, &QPushButton::clicked, this, &ElevatorPanel::FloorButtonPresssed);
    }
       
    this->setLayout(gridLayout);
    this->setWindowTitle(QString::fromStdString(label));

    this->show();

    numberOfFloors_ = numFloors;
    openDoor_ = false;
    closeDoor_ = false;
}

ElevatorPanel::~ElevatorPanel()
{

} 

void 
ElevatorPanel::SetLevel(const int level) 
{
    currentLevel_ = level;
    this->DisplayMessage(int_to_string(currentLevel_));
}

void 
ElevatorPanel::CalculateCurrentLevel(const float height)
{
    for(int i = 0; i < numberOfFloors_; i++)
    {
        if( std::fabs(height - i*FLOOR_HEIGHT_METERS) < 0.01 )
        {
            SetLevel(i);
        }
    }
}
int 
ElevatorPanel::GetNearestLevel(const float height)
{
    auto nearestLevel = 0;
    float currentMin = std::numeric_limits<float>::max();

    for(int i = 0; i < numberOfFloors_; i++)
    {
        auto diff = height - i*FLOOR_HEIGHT_METERS;
        if(height - i*FLOOR_HEIGHT_METERS < currentMin)
        {
            nearestLevel = i;
            currentMin = diff;
        }
    } 

    return nearestLevel;
}

void 
ElevatorPanel::AddToRoute(const ServiceRequest& request)
{
    // Only add a level once to the route
    if(std::find_if(route_.begin(), route_.end(), special_compare(request)) == route_.end())  
        route_.emplace_back(request);
}

void 
ElevatorPanel::DisplayRoute() const
{
    for(auto r : route_)
        std::cout << "(" << r.level << ", " << r.direction << ") ";
    std::cout << std::endl;
}
void
ElevatorPanel::PopRoute()
{      
    ServiceRequest nextRequest(currentLevel_, REQ_IDLE);

    if(route_.size() > 0)
    {
        nextRequest = route_[0];
        route_.front() = std::move(route_.back());
        route_.pop_back();
    }

    currentlyServicing_ = nextRequest;
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

}

void 
ElevatorPanel::FloorButtonPresssed()
{
    FloorButton* buttonSender = qobject_cast<FloorButton*>(sender()); // retrieve the button you have clicked
   
    auto requestedLevel = buttonSender->GetLevel();

    auto currentDirection =  currentlyServicing_.direction;

    if( requestedLevel > currentLevel_ && 
        (currentDirection == RequestDirection::REQ_UP || currentDirection == RequestDirection::REQ_IDLE))
        route_.emplace_back(requestedLevel, RequestDirection::REQ_UP);
    if( requestedLevel < currentLevel_ && 
        (currentDirection == RequestDirection::REQ_UP || currentDirection == RequestDirection::REQ_IDLE))
        route_.emplace_back(requestedLevel, RequestDirection::REQ_DOWN);   
}