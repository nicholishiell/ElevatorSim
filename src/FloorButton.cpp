#include <iostream>
#include "include/FloorButton.h"

void
FloorButton::FloorButtonPressed()
{   
    emit(ServiceRequested(level_));
}
