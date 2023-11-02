#ifndef ELEVATOR_POS_SENSOR_H
#define  ELEVATOR_POS_SENSOR_H

#include <cmath>

#include "include/Utility.h"
#include "include/Elevator.h"

class ElevatorPositionSensor
{
public:
    ElevatorPositionSensor(const int numOfFloors){};

    

private:

};

#endif

// void 
// Elevator::UpdateCurrentLevel()
// {
//     for(int i = 0; i < numberOfFloors_; i++)
//     {
//         if( std::fabs(height_ - i*FLOOR_HEIGHT_METERS) < 0.01 )
//             currentLevel_ = i; 
//     }
    
//     panel_->DisplayMessage(int_to_string(currentLevel_));
// }