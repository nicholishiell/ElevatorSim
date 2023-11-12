#include "include/MyController.h"


MyController::MyController() : Controller()
{
    helpRequested_ = false;
}

MyController::~MyController()
{

} 

void 
MyController::HandleFireAlarm(  const int level,
                                ElevatorSharedPtrVector elevators,
                                FloorSharedPtrVector floors)
{
    for(auto floor : floors)
    {
        // TODO: Add a fire on the specified level
    }

    for(auto elevator : elevators)
    {
        auto panel = elevator->GetPanel();
        elevator->Disable();

        auto nearestLevel = panel->GetSensor()->GetNearestLevel();

        if(nearestLevel == level)
        {
            if(panel->GetSensor()->AtBottom())
            {
                panel->GoToFloor(level+1);
            }
            else
            {
                panel->GoToFloor(level-1);
            }
        }
        else
        {
            panel->GoToFloor(nearestLevel);
        }
    }
}

void 
MyController::HandlePowerOutageAlarm(   ElevatorSharedPtrVector elevators,
                                        FloorSharedPtrVector floors)
{
    for(auto floor : floors)
    {
        // TODO: Have all the lights become dimmed and red?
    }

    for(auto elevator : elevators)
    {
        auto panel = elevator->GetPanel();
        elevator->Disable();

        if(panel->GetSensor()->IsBetweenFloors())
        {
            auto targetFloor = std::min(panel->GetPreviousFloor(), panel->GetNextFloor());
            auto emergencyRequest = ServiceRequest(targetFloor, RequestDirection::REQ_DOWN);

            panel->SetCurrentlyServicing(emergencyRequest);
        }
        
    }
}

void 
MyController::HandleServiceRequest( const ServiceRequest request,
                                    ElevatorSharedPtrVector elevators,
                                    FloorSharedPtrVector floors)
{
    pendingRequests_.emplace_back(request);
}

void
MyController::HandleHelpRequest(ElevatorPanelSharedPtr elevatorPanel,
                                BuildingPanelSharedPtr buildingPanel)
{
    buildingPanel->HelpRequested(elevatorPanel);
    calledTimeStamp_ = std::chrono::system_clock::now();
    helpRequested_ = true;
}

void 
MyController::HandleAnswerHelp(ElevatorPanelSharedPtr elevatorPanel)
{
    elevatorPanel->AudioMessage();
    elevatorPanel->DisplayMessage("Building Safety Service");
    helpRequested_ = false;
}

void 
MyController::Step( const float timeStep,
                    ElevatorSharedPtrVector elevators,
                    BuildingPanelSharedPtr buildingPanel)
{
    this->assignServiceRequests(elevators);
    
    // Check if call timed out
    if(helpRequested_)
    {
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now()-calledTimeStamp_).count();

        if(elapsedTime > CALL_TIME_OUT)
        {
            auto elevatorPanel = buildingPanel->GetCallingPanel();
            elevatorPanel->AudioMessage();
            elevatorPanel->DisplayMessage("911 Operator");
            helpRequested_ = false;
            buildingPanel->EndCall();  
        }
    }
}

void 
MyController::assignServiceRequests(ElevatorSharedPtrVector elevators)
{   
    // std::cout << "PendingJobs: " << std::endl;
    // for(auto r : pendingRequests_)
    //     std::cout << r.level << " ";
    // std::cout << std::endl;
    
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
            auto levelDiff = request.level - elevator->GetPanel()->GetPreviousFloor();

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
            assignedElevator->GetPanel()->AddToRoute(request);
            
            // Remove it from the pending list
            pendingRequests_.erase(requestIter);
            requestIter--;
        }
    }

}