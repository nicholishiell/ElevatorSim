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
    std::cout << "handleFireAlarm on level "<< level << std::endl;

    for(auto floor : floors)
    {
    }

    for(auto elevator : elevators)
    {
    }
}

void 
MyController::HandlePowerOutageAlarm(   ElevatorSharedPtrVector elevators,
                                        FloorSharedPtrVector floors)
{
    std::cout << "handlePowerOutageAlarm" << std::endl;
        for(auto floor : floors)
    {
    }

    for(auto elevator : elevators)
    {
    }
}

void 
MyController::HandleServiceRequest( const ServiceRequest request,
                                    ElevatorSharedPtrVector elevators,
                                    FloorSharedPtrVector floors)
{
    std::cout << "handleServiceRequest" << std::endl;
    pendingRequests_.emplace_back(request);

    for(auto floor : floors)
    {
    }

    for(auto elevator : elevators)
    {
    }
}

void 
MyController::Step( const float timeStep,
                    ElevatorSharedPtrVector elevators,
                    FloorSharedPtrVector floors)
{
    this->assignServiceRequests(elevators);
    
    if(timeStep < 0)
        std::cout << "Invalid time step" << std::endl;

    for(auto floor : floors)
    {
    }
}

void 
MyController::assignServiceRequests(ElevatorSharedPtrVector elevators)
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
            if( elevator->GetStateString() != "UP" &&  
                elevator->GetStateString() != "DOWN" && 
                elevator->GetStateString() != "IDLE")
                continue;

            // Skip elevators that are going in the wrong direction
            if( (elevator->GetStateString() == "UP" && request.direction == RequestDirection::REQ_DOWN) ||
                (elevator->GetStateString() == "DOWN" && request.direction == RequestDirection::REQ_UP) )
                continue;

            // Skip elevators that are already passed the request level
            auto levelDiff = request.level - elevator->GetLevel();

            if( (elevator->GetStateString() == "UP" && levelDiff < 0) ||
                (elevator->GetStateString() == "DOWN" && levelDiff > 0) )
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