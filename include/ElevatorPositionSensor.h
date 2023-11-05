#ifndef ELEVATOR_POS_SENSOR_H
#define ELEVATOR_POS_SENSOR_H

#include <cmath>

#include "include/Utility.h"
#include "include/Elevator.h"
#include "include/Observer.h"

class ElevatorPositionSensor : public Observer
{
public:
    ElevatorPositionSensor();

    ElevatorPositionSensor( const int numOfFloors, 
                            const ElevatorSharedPtr elevator);

    FloorPosition GetFloorPosition() const;

    bool HasArrivedAtFloor(const int floor) const;
    bool ArrivedAtTargetFloor() const;
    bool IsBetweenFloors() const;
    bool AtTop() const;
    bool AtBottom() const;

    int GetNumberOfFloors() const;
    
    int GetNearestLevel();

    void Notify();

private:

    void calculateCurrentLevel();
    
    FloorPosition floorPos_;
    int numberOfFloors_;

    const ElevatorSharedPtr elevator_;
};

#endif