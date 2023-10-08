#include "include/GraphicalController.h"

GraphicalController::GraphicalController(const Controller * aController)
{
    aController_ = aController;
}

GraphicalController::~GraphicalController()
{

} 

void 
GraphicalController::advance(int step)
{
    if (!step)
        return; 

    update();
}

QRectF 
GraphicalController::boundingRect() const
{
    auto nFloors = aController_->GetNumberOfFloors();
    
    return QRectF(0,0,FLOOR_WIDTH, nFloors*FLOOR_HEIGHT);
}

void 
GraphicalController::paint( QPainter *painter, 
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{ 
    // Draw the cartoon representation of the floors
    float y = 0;
   
    for(int iFloor = aController_->GetNumberOfFloors()-1; iFloor >= 0; iFloor--)
    {             
        auto floorImage = drawFloor(aController_->GetFloor(iFloor));

        for(u_int iElevator = 0; iElevator  < aController_->GetNumberOfElevators(); iElevator++)
        {
            floorImage = drawElevatorOnFloor(   aController_->GetElevator(iElevator),
                                                floorImage,
                                                iFloor,
                                                iElevator);
        }

        painter->drawPixmap(0,y,0.35*FLOOR_WIDTH,0.35*FLOOR_HEIGHT,floorImage);

        y = y + 0.35*FLOOR_HEIGHT+ 10;
    }
   
    // Draw the basic drawing of the elevators locations
    painter->setPen(QColor(0,0,255));

    auto totalHeightPixels = (0.35*FLOOR_HEIGHT+ 10) * aController_->GetNumberOfFloors();
    auto totalHeightMeters = FLOOR_HEIGHT_METERS * aController_->GetNumberOfFloors();

    // TODO: this should be a const in utility
    auto elevator_height = 100;

    for(u_int iElevator = 0; iElevator  < aController_->GetNumberOfElevators(); iElevator++)
    {
        auto elevator = aController_->GetElevator(iElevator);
        auto elevatorHeightMeters = elevator->GetHeight();

        auto elevatorX = (0.35*FLOOR_WIDTH + 100 +iElevator*125);
        auto elevatorY = totalHeightPixels * (1. - elevatorHeightMeters/totalHeightMeters) - elevator_height - 25;

        painter->drawRect(elevatorX, elevatorY, 60, 100);
    }
}

QPixmap 
GraphicalController::drawFloor(const FloorSharedPtr floor)
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
    for(int i = 0; i < aController_->GetNumberOfElevators(); i++)
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
GraphicalController::drawElevatorOnFloor(   const ElevatorSharedPtr elevator, 
                                            QPixmap floorImage, 
                                            const int floorIndex, 
                                            const int elevatorIndex)
{

    QPixmap elevatorImage;
    
    if(elevator->GetLevel() == floorIndex && elevator->GetDoorState() == DoorState::OPEN)
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
