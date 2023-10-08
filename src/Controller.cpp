#include "include/Controller.h"


Controller::Controller()
{


}

Controller::~Controller()
{

} 

// Add a floor to the control system
void 
Controller::AddFloor(std::string label)
{
    auto numFloors = static_cast<int>(floors_.size());

    if(numFloors <= MAX_NUMBER_OF_FLOORS)
    {
        auto newFloor = std::make_shared<Floor>(label, numFloors);
        newFloor->SetNumElevators(static_cast<int>(elevators_.size()));
        
        auto floorPanel = newFloor->GetPanel();
        QObject::connect(floorPanel.get(), &FloorPanel::UpRequested, this, &Controller::HandleUpRequest);
        QObject::connect(floorPanel.get(), &FloorPanel::DownRequested, this, &Controller::HandleDownRequest);

        floors_.emplace_back(newFloor);

        for(auto elevator : elevators_)        
            elevator->SetNumberOfFloors(static_cast<int>(floors_.size()));
    }
    else
    {
        std::cout << "Maximum # of Floors reached" << std::endl;
    }
}

// Add an elevator to the control system
void 
Controller::AddElevator(std::string label, int level, DoorState state)
{
    auto numElevators = static_cast<int>(elevators_.size());

    if(numElevators <= MAX_NUMBER_OF_ELEVATORS)
    {
        auto numFloors = static_cast<int>(floors_.size());
        auto newElevator = std::make_shared<Elevator>(label, state, level, numFloors);
        
        elevators_.emplace_back(newElevator);

        for(auto floor : floors_)
            floor->SetNumElevators(static_cast<int>(elevators_.size()));
    }
    else
        std::cout << "Maximum # of Elevators reached" << std::endl;
}

void 
Controller::HandleUpRequest(const int level)
{
    pendingRequests_.emplace_back(level, RequestDirection::REQ_UP);
}

void 
Controller::HandleDownRequest(const int level)
{
    pendingRequests_.emplace_back(level, RequestDirection::REQ_DOWN);
}

//
void
Controller::Update(const float timeStep)
{
    // Move the elevators
    for(auto elevator : elevators_)
    {
        std::cout << "updating: " << elevator->GetLabel() << std::endl;
        elevator->Update(timeStep);
    }
    // Based on the states and positions of the elevators update the floor panels
    updateFloorPanels();

    // Try to assign any pending requests
    assignServiceRequests();

    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
}

void 
Controller::updateFloorPanels()
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
Controller::assignServiceRequests()
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