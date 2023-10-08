#include "include/MyController.h"


MyController::MyController() : Controller()
{


}

MyController::~MyController()
{

} 

void 
MyController::HandleFireAlarm(const int level)
{
    std::cout << "handleFireAlarm" << std::endl;
}

void 
MyController::HandlePowerOutageAlarm()
{
    std::cout << "handlePowerOutageAlarm" << std::endl;
}

void 
MyController::Step(const float timeStep)
{
    this->updateFloorPanels();

    this->assignServiceRequests();
}


void 
MyController::updateFloorPanels()
{
    for(auto floor : floors_)
        floor->GetPanel()->LightsOut();

    for(int i = 0; i < elevators_.size(); i++)
    {
        auto elevator = elevators_[i];
        auto level = elevator->GetLevel();
        auto currentRequest = elevator->GetCurrentlyServicing();

        // Turn elevator light for the floor it is currently on
        floors_[level]->GetPanel()->SetElevatorLight(i, LightState::ON);

        if(currentRequest.level == level)
        {            
            // turn off service call lights
            if(currentRequest.direction == RequestDirection::REQ_UP)
            {
                floors_[currentRequest.level]->GetPanel()->UpRequestServiced();
            }
            else if(currentRequest.direction == RequestDirection::REQ_DOWN)
            {
                floors_[currentRequest.level]->GetPanel()->DownRequestServiced();
            }

        }
    }
}

void 
MyController::assignServiceRequests()
{   
    std::cout << "PendingJobs: " << std::endl;
    for(auto r : pendingRequests_)
        std::cout << r.level << " ";
    std::cout << std::endl;
    
    // Loop over all the pending requests
    auto requestIter = pendingRequests_.begin();
    for(; requestIter != pendingRequests_.end(); requestIter++)
    {
        auto request = *requestIter;

        auto minLevelDiff = 999;
        ElevatorSharedPtr assignedElevator = nullptr;
        
        // Loop over all the elevators to see which ones can service the request
        for(auto elevator : elevators_)
        {
            // Skip elevators that aren't in states UP/DOWN/IDLE
            if( elevator->GetState() != ElevatorState::UP &&  
                elevator->GetState() != ElevatorState::DOWN && 
                elevator->GetState() != ElevatorState::IDLE)
                continue;

            // Skip elevators that are going in the wrong direction
            if( (elevator->GetState() == ElevatorState::UP && request.direction == RequestDirection::REQ_DOWN) ||
                (elevator->GetState() == ElevatorState::DOWN && request.direction == RequestDirection::REQ_UP) )
                continue;

            // Skip elevators that are already passed the request level
            auto levelDiff = request.level - elevator->GetLevel();

            if( (elevator->GetState() == ElevatorState::UP && levelDiff < 0) ||
                (elevator->GetState() == ElevatorState::DOWN && levelDiff > 0) )
                continue;

            if(std::abs(levelDiff) < minLevelDiff)
            {
                assignedElevator = elevator;
                minLevelDiff = std::abs(levelDiff);
            }
        }

        // If there is an elevator that is able to handle the request than assign it
        if(assignedElevator != nullptr)
        {
            // Assign it to the elevator
            assignedElevator->AddToRoute(request);
            
            // Remove it from the pending list
            pendingRequests_.erase(requestIter);
            requestIter--;
        }
    }

}