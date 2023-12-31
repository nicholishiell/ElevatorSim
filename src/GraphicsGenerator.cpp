#include "include/GraphicsGenerator.h"
#include "include/BuildingSimulator.h"

GraphicsGenerator::GraphicsGenerator(BuildingSimulator * buildingSim)
{
    buildingSim_ = buildingSim;
    buildingSim_->SetGraphicsGenerator(this);
}

GraphicsGenerator::~GraphicsGenerator()
{

} 

void 
GraphicsGenerator::advance(int step)
{
    if (!step)
        return; 

    update();
}

QRectF 
GraphicsGenerator::boundingRect() const
{
    auto nFloors = buildingSim_->GetNumberOfFloors();
    
    return QRectF(0,0,FLOOR_WIDTH, nFloors*FLOOR_HEIGHT);
}

void 
GraphicsGenerator::paint(   QPainter *painter, 
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{ 
    if(buildingSim_->IsInitialized())
    {
        // Draw the cartoon representation of the floors
        float y = 0;
    
        for(int iFloor = buildingSim_->GetNumberOfFloors()-1; iFloor >= 0; iFloor--)
        {    
            auto floor = buildingSim_->GetFloor(iFloor);        
            auto floorImage = drawFloor(floor);

            for(u_int iElevator = 0; iElevator  < buildingSim_->GetNumberOfElevators(); iElevator++)
            {
                floorImage = drawElevatorOnFloor(   buildingSim_->GetElevator(iElevator),
                                                    floorImage,
                                                    iFloor,
                                                    iElevator);
            }


            for(u_int iPerson = 0; iPerson < floor->GetNumberOfPeople(); iPerson++)
            {
                floorImage = drawPerson(floor->GetPerson(iPerson),
                                        floorImage,
                                        iFloor,
                                        iPerson);
            }
               
            painter->drawPixmap(0,y,0.35*FLOOR_WIDTH,0.35*FLOOR_HEIGHT,floorImage);

            y = y + 0.35*FLOOR_HEIGHT+ 10;
        }
    
        // Draw the basic drawing of the elevators locations
        painter->setPen(QColor(0,0,255));

        auto totalHeightPixels = (0.35*FLOOR_HEIGHT+ 10) * buildingSim_->GetNumberOfFloors();
        auto totalHeightMeters = FLOOR_HEIGHT_METERS * buildingSim_->GetNumberOfFloors();

        // TODO: this should be a const in utility
        auto elevator_height = 100;

        for(u_int iElevator = 0; iElevator  < buildingSim_->GetNumberOfElevators(); iElevator++)
        {
            auto elevator = buildingSim_->GetElevator(iElevator);
            auto elevatorHeightMeters = elevator->GetHeight();

            auto elevatorX = (0.35*FLOOR_WIDTH + 100 +iElevator*125);
            auto elevatorY = totalHeightPixels * (1. - elevatorHeightMeters/totalHeightMeters) - elevator_height - 25;

            painter->drawRect(elevatorX, elevatorY, 60, 100);
            
            for(u_int iPerson = 0; iPerson < elevator->GetNumberOfPeople(); iPerson++)
            {
                auto personImage = QPixmap(QString::fromStdString("./images/Person/Person.png"));
                painter->drawPixmap(elevatorX+iPerson*15, elevatorY+27, 40, 80, personImage);
            }
        }
    }
}

QPixmap 
GraphicsGenerator::drawFloor(const FloorSharedPtr floor)
{
    // Get the base floor image
    QPixmap floorImage;

    if(floor->GetPanel()->GetUpRequested() && floor->GetPanel()->GetDownRequested())
        floorImage  = QPixmap("./images/floor/FloorBaseBothOn.png"); 
    else if(floor->GetPanel()->GetUpRequested())
        floorImage = QPixmap("./images/floor/FloorBaseUpOn.png");
    else if(floor->GetPanel()->GetDownRequested())
        floorImage = QPixmap("./images/floor/FloorBaseDownOn.png");
    else
        floorImage = QPixmap("./images/floor/FloorBaseBothOff.png");

    // Get the correct floor sign
    auto floorSignFilename = FLOOR_SIGN_FILE_NAMES[floor->GetLevel()];
    QPixmap floorSignImage(QString::fromStdString("./images/signs/"+floorSignFilename));

    QPainter painter(&floorImage);
    painter.drawPixmap(150,40,SIGN_WIDTH,SIGN_HEIGHT,floorSignImage);
    
    QPixmap elevatorLightImage;
    // Get the elevator lights with correct state (on/off)
    for(int i = 0; i < buildingSim_->GetNumberOfElevators(); i++)
    {
        if(floor->GetPanel()->GetElevatorLightState(i) == LightState::ON)        
        {    
            elevatorLightImage = QPixmap(QString::fromStdString("./images/floor/ElevatorLightOn.png"));
        }
        else if(floor->GetPanel()->GetElevatorLightState(i) == LightState::OFF)        
        {
            elevatorLightImage = QPixmap(QString::fromStdString("./images/floor/ElevatorLightOff.png"));
        }
        else
        {             
            elevatorLightImage = QPixmap(ELEVATOR_LIGHT_WIDTH, ELEVATOR_LIGHT_HEIGHT);
            elevatorLightImage.fill(QColor(0,0,0));
        }

        painter.drawPixmap( 687+i*ELEVATOR_SPACING,
                            51,
                            ELEVATOR_LIGHT_WIDTH,
                            ELEVATOR_LIGHT_HEIGHT, 
                            elevatorLightImage);
    }

    return floorImage;
}

QPixmap 
GraphicsGenerator::drawPerson(  PersonSharedPtr person,
                                QPixmap floorImage, 
                                const int floorIndex, 
                                const int personIndex)
{
    auto personImage = QPixmap(QString::fromStdString("./images/Person/Person.png"));
    
    QPainter painter(&floorImage);        
    painter.drawPixmap( 400+200*personIndex,
                        100,
                        PERSON_WIDTH,
                        PERSON_HEIGHT, 
                        personImage);
    QFont font=painter.font() ;
    font.setPointSize(18);
    painter.setFont(font);

    painter.drawText(   400 + 200*personIndex,
                        400,
                        QString::fromStdString(person->GetLabel()));

    return floorImage;
}

QPixmap 
GraphicsGenerator::drawElevatorOnFloor( const ElevatorSharedPtr elevator, 
                                        QPixmap floorImage, 
                                        const int floorIndex, 
                                        const int elevatorIndex)
{   
    QPixmap elevatorImage;

    if(elevator->IsAtFloor(floorIndex) && elevator->GetDoorState() != DoorState::CLOSED)
    {
        elevatorImage = QPixmap("./images/elevator/ElevatorOpen.png");
    }
    else
    {
        elevatorImage = QPixmap("./images/elevator/ElevatorClosed.png");
    }

    auto x = 612+elevatorIndex*ELEVATOR_SPACING;
    auto y = 87;
    QPainter painter(&floorImage);
    painter.drawPixmap( x,
                        y,
                        ELEVATOR_WIDTH,
                        ELEVATOR_HEIGHT,
                        elevatorImage);

    return floorImage;
}
