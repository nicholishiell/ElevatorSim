#include "include/ElevatorPanel.h"
#include "include/FloorButton.h"

#include <QGridLayout>
#include <QPixmap>
#include <QLabel>
#include <QString>

ElevatorPanel::ElevatorPanel(   const std::string label, 
                                const int numFloors,
                                QWidget *parent) : QWidget(parent)
{
    // Create all the compulsory buttons
    label_ = label;
    numFloors_ = numFloors;

    auto gridLayout = new QGridLayout(this);
    auto fireButton = new QPushButton("FIRE", this);
    auto helpButton = new QPushButton("HELP", this);
    auto openButton = new QPushButton("OP", this);
    auto closeButton = new QPushButton("CD", this);
    
    gridLayout->addWidget(fireButton,0,0);
    gridLayout->addWidget(helpButton,0,1);
    gridLayout->addWidget(openButton,1,0);
    gridLayout->addWidget(closeButton,1,1);

    QObject::connect(fireButton, &QPushButton::clicked, this, &ElevatorPanel::FireButtonPresssed);
    QObject::connect(helpButton, &QPushButton::clicked, this, &ElevatorPanel::HelpButtonPresssed);
    QObject::connect(openButton, &QPushButton::clicked, this, &ElevatorPanel::OpenButtonPresssed); // TODO: This should change to pressed not clicked
    QObject::connect(closeButton, &QPushButton::clicked, this, &ElevatorPanel::CloseButtonPresssed);

    auto line1 = new QFrame;
    line1->setFrameShape(QFrame::HLine);

    gridLayout->addWidget(line1,2,0,1,2);

    // Create labels for overloaded, obstructed, and display msg
    auto obLabel_ = new QLabel("OB");
    auto olLabel_ = new QLabel("OL");

    gridLayout->addWidget(obLabel_,3,0,Qt::AlignHCenter);
    gridLayout->addWidget(olLabel_,3,1,Qt::AlignHCenter);

    auto displayLabel_ = new QLabel(QString::fromStdString(displayMsg_));
    gridLayout->addWidget(displayLabel_,4,0,1,2,Qt::AlignHCenter);

    auto line2 = new QFrame;
    line2->setFrameShape(QFrame::HLine);

    gridLayout->addWidget(line2,5,0,1,2);

    // Create the floor buttons
    std::vector<QPushButton*> floorButtons;
    auto row = 6;
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
    this->setWindowTitle(QString::fromStdString(label_));

    this->show();
}

ElevatorPanel::~ElevatorPanel()
{

} 

ServiceRequestVector 
ElevatorPanel::PopRequests()
{
    auto temp = requests_;

    requests_.erase(requests_.begin(),requests_.end());

    return temp;

}


void 
ElevatorPanel::OpenButtonPresssed()
{

}

void 
ElevatorPanel::CloseButtonPresssed()
{

}


void 
ElevatorPanel::FireButtonPresssed()
{

}

void 
ElevatorPanel::HelpButtonPresssed()
{

}

void 
ElevatorPanel::FloorButtonPresssed()
{
    FloorButton* buttonSender = qobject_cast<FloorButton*>(sender()); // retrieve the button you have clicked
    QString buttonText = buttonSender->text(); // retrive the text from the button clicked

    requests_.emplace_back( buttonSender->GetLevel(), 
                            RequestDirection::REQ_IDLE);
}