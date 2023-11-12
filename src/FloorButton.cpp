#include <iostream>
#include "include/FloorButton.h"

void
FloorButton::FloorButtonPressed()
{   
    std::cout << "FloorButton::FloorButtonPressed" << std::endl;
    emit(ServiceRequested(level_));
}
