#include "include/MyController.h"


MyController::MyController() : Controller()
{


}

MyController::~MyController()
{

} 

void 
MyController::HandleFireAlarm(  const int level,
                                ElevatorSharedPtrVector elevators,
                                FloorSharedPtrVector floors)
{
    std::cout << "handleFireAlarm" << std::endl;
}

void 
MyController::HandlePowerOutageAlarm(   ElevatorSharedPtrVector elevators,
                                        FloorSharedPtrVector floors)
{
    std::cout << "handlePowerOutageAlarm" << std::endl;
}

void 
MyController::HandleServiceRequest( const ServiceRequest request,
                                    ElevatorSharedPtrVector elevators,
                                    FloorSharedPtrVector floors)
{
    std::cout << "handleServiceRequest" << std::endl;
    pendingRequests_.emplace_back(request);
}

void 
MyController::Step( const float timeStep,
                    ElevatorSharedPtrVector elevators,
                    FloorSharedPtrVector floors)
{
    this->updateFloorPanels(elevators, floors);

    this->assignServiceRequests(elevators, floors);
}


void 
MyController::updateFloorPanels(ElevatorSharedPtrVector elevators,
                                FloorSharedPtrVector floors)
{
    for(auto floor : floors)
        floor->GetPanel()->LightsOut();

    for(int i = 0; i < elevators.size(); i++)
    {
        auto elevator = elevators[i];
        auto level = elevator->GetLevel();
        auto currentRequest = elevator->GetCurrentlyServicing();

        // Turn elevator light for the floor it is currently on
        floors[level]->GetPanel()->SetElevatorLight(i, LightState::ON);

        if(currentRequest.level == level)
        {            
            // turn off service call lights
            if(currentRequest.direction == RequestDirection::REQ_UP)
            {
                floors[currentRequest.level]->GetPanel()->UpRequestServiced();
            }
            else if(currentRequest.direction == RequestDirection::REQ_DOWN)
            {
                floors[currentRequest.level]->GetPanel()->DownRequestServiced();
            }

        }
    }
}

void 
MyController::assignServiceRequests(ElevatorSharedPtrVector elevators,
                                    FloorSharedPtrVector floors)
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
        for(auto elevator : elevators)
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