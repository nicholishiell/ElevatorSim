#include <QLabel>
#include <QGridLayout>

#include "include/PersonGenerator.h"
#include "include/Person.h"

PersonGenerator::PersonGenerator(   FloorSharedPtrVector floors,
                                    QWidget *parent) :QWidget(parent)
{
    // Create all the compulsory buttons
    auto gridLayout = new QGridLayout(this);
    auto generateButton = new QPushButton("Generate", this);
    auto nameLabel = new QLabel("Name");
    auto initialFloorLabel = new QLabel("Initial Floor");
    auto destinationFloorLabel = new QLabel("Destination Floor");
    auto weightLabel = new QLabel("Weight");
    nameField_ = new QLineEdit(this);
    initialFloorSelector_ = new QComboBox(this);
    destinationFloorSelector_ = new QComboBox(this);
    weightSelector_ = new QSlider(this);

    gridLayout->addWidget(nameLabel,0,0,1,2);
    gridLayout->addWidget(nameField_,0,2);

    gridLayout->addWidget(initialFloorLabel,1,0,1,2);
    gridLayout->addWidget(initialFloorSelector_,1,2);
    
    gridLayout->addWidget(destinationFloorLabel,2,0,1,2);
    gridLayout->addWidget(destinationFloorSelector_,2,2);
    
    gridLayout->addWidget(weightLabel,3,0,1,2);
    gridLayout->addWidget(weightSelector_,3,2);

    gridLayout->addWidget(generateButton,4,0);

    for(int i = 0; i < floors.size(); i++)
    {
        initialFloorSelector_->addItem(QString::number(i));
        destinationFloorSelector_->addItem(QString::number(i));
    }
    
    weightSelector_->setOrientation(Qt::Orientation::Horizontal);
    weightSelector_->setMinimum(35.);
    weightSelector_->setMaximum(150.);
    weightSelector_->setSliderPosition(75.);

    QObject::connect(generateButton, &QPushButton::clicked, this, &PersonGenerator::generatePerson);
    
    this->setWindowTitle(QString::fromStdString("Person Generator"));

    this->show();

    floors_ = floors;
}

PersonGenerator::~PersonGenerator()
{

}


void 
PersonGenerator::generatePerson()
{
    auto p =  std::make_shared<Person>( nameField_->text().toStdString(),
                                        initialFloorSelector_->currentIndex(),
                                        destinationFloorSelector_->currentIndex(),
                                        weightSelector_->value());
    

    floors_[initialFloorSelector_->currentIndex()]->AddPerson(p);
}

