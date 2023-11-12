#include <cmath>

#include "include/ElevatorPositionSensor.h"

ElevatorPositionSensor::ElevatorPositionSensor()
{
    numberOfFloors_ = 0;
    floorPos_ = FloorPosition(0,0);
}

ElevatorPositionSensor::ElevatorPositionSensor( const int numOfFloors, 
                                                const ElevatorSharedPtr elevator) : elevator_(elevator)
{
    numberOfFloors_ = numOfFloors;
    floorPos_ = FloorPosition(0,0);
}

void 
ElevatorPositionSensor::Notify()
{
    calculateCurrentLevel();    
}

FloorPosition 
ElevatorPositionSensor::GetFloorPosition() const 
{
    return floorPos_;
}

bool 
ElevatorPositionSensor::HasArrivedAtFloor(const int floor) const
{
    return floorPos_.prevFloor == floor && floorPos_.nextFloor == floor;
}

bool 
ElevatorPositionSensor::IsBetweenFloors() const 
{
    return floorPos_.nextFloor != floorPos_.prevFloor;
}

bool 
ElevatorPositionSensor::AtTop() const 
{
    return HasArrivedAtFloor(numberOfFloors_);
}

bool 
ElevatorPositionSensor::AtBottom() const 
{
    return HasArrivedAtFloor(0);
}

void 
ElevatorPositionSensor::calculateCurrentLevel()
{
    auto height = elevator_->GetHeight();

    floorPos_.nextFloor = std::ceil(height/float(FLOOR_HEIGHT_METERS));
    
    if( std::fabs(height - floorPos_.nextFloor*FLOOR_HEIGHT_METERS) < 0.05)
    {
        floorPos_.prevFloor = floorPos_.nextFloor;
    }
}

int 
ElevatorPositionSensor::GetNumberOfFloors() const 
{
    return numberOfFloors_;
}

int 
ElevatorPositionSensor::GetNearestLevel()
{
    auto height = elevator_->GetHeight();

    auto nearestLevel = std::round(height / FLOOR_HEIGHT_METERS);
    
    return int(nearestLevel);
}