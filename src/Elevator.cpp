#include "include/Elevator.h"

#include <cmath>

std::string
int_to_string(const int i)
{
    std::stringstream ss;
    std::string s;
    ss << i;
    ss >> s;
    return s;
}


Elevator::Elevator( const std::string label, 
                    DoorState doorState, 
                    const float height)
{
    doorState_ = doorState;
    label_ = label;

    elevatorState_ =  new ElevatorStateIdle();
    
    height_ = height;
    
    panel_ = nullptr;
    //panel_ = std::make_shared<ElevatorPanel>(label,numFloors);
}

Elevator::~Elevator()
{

} 

void 
Elevator::AddToRoute(const ServiceRequest& request)
{
    // Only add a level once to the route
    if(std::find_if(route_.begin(), route_.end(), special_compare(request)) != route_.end())
        return;

    if(elevatorState_->GetStateString() == "UP")
    {
        // TODO: This and the duplicate code below should be put into a seperate common function    
        auto rIter = route_.begin();
        for(; rIter != route_.end(); rIter++)
        {
            if(rIter->level < rIter->level)
                continue;
            break;
        }

        route_.insert(rIter, request);
    }
    else if(elevatorState_->GetStateString() == "DOWN")
    {
        // TODO: This and the duplicate code below should be put into a seperate common function
        auto rIter = route_.begin();
        for(; rIter != route_.end(); rIter++)
        {
            if(rIter->level > rIter->level)
                continue;
            break;
        }

        route_.insert(rIter, request);
    }
    else
    {
        route_.clear();
        route_.emplace_back(request);
    }

}

void 
Elevator::Update(const float timeStep)
{
    // Display data to console
    ToConsole();

    // Handle any of the jobs that were entered between update cycles    
    handlePendingRequests();

    // Update the state
    ElevatorState* nextState = elevatorState_->Update(shared_from_this(), timeStep); 

    if(nextState != nullptr)
    {
        delete(elevatorState_);
        elevatorState_ = nextState;
    }
    
    // finally update the currently level of the elevator
    this->UpdateCurrentLevel();
}

void 
Elevator::handlePendingRequests()
{
    auto requests = panel_->PopRequests();
    for(auto r : requests)
    {
        // TODO: Come back and get rid of this duplicate code
        auto levelDiff = r.level - currentLevel_;
       
        if( (currentlyServicing_.direction == RequestDirection::REQ_UP  || 
            currentlyServicing_.direction == RequestDirection::REQ_IDLE)
            && levelDiff > 0 )
        {
            r.direction = RequestDirection::REQ_UP;
            AddToRoute(r);
        }
        if((currentlyServicing_.direction == RequestDirection::REQ_DOWN || 
            currentlyServicing_.direction == RequestDirection::REQ_IDLE)
            && levelDiff < 0)
        {
            r.direction = RequestDirection::REQ_DOWN;
            AddToRoute(r);
        }
    }
}

void 
Elevator::ToConsole() const
{
    // Display info to the console
    std::cout << label_ << "\t" << currentLevel_ << "\t" << height_ << "\t" << elevatorState_->GetStateString() << std::endl;
    std::cout << "CUR REQ: " << GetCurrentlyServicing().level << "\t" << GetCurrentlyServicing().direction << std::endl;
    std::cout << "Route: ";
    for(auto r : route_)
        std::cout << "(" << r.level << ", " << r.direction << ") ";
    std::cout << std::endl;
}

void 
Elevator::UpdateCurrentLevel()
{
    for(int i = 0; i < numberOfFloors_; i++)
    {
        if( std::fabs(height_ - i*FLOOR_HEIGHT_METERS) < 0.01 )
            currentLevel_ = i; 
    }
    
    panel_->DisplayMessage(int_to_string(currentLevel_));
}

void 
Elevator::UpdateHeight(const float deltaH)
{
    height_ += deltaH;
}

std::string 
Elevator::GetStateString() const 
{
    return elevatorState_->GetStateString();
}

ServiceRequest 
Elevator::PopRoute()
{      
    ServiceRequest nextRequest(currentLevel_, REQ_IDLE);

    if(route_.size() > 0)
    {
        nextRequest = route_[0];
        route_.front() = std::move(route_.back());
        route_.pop_back();
    }

    return nextRequest;
}

bool 
Elevator::checkOverloaded()
{
    float currentWeight = 0.;
    for(auto person : passengers_)
        currentWeight = currentWeight + person.GetWeight();

    auto overloaded = currentWeight > ELEVATOR_MAX_WEIGHT;

    if(overloaded)
        panel_->SetOverloadState(overloaded);

    return overloaded;
}

bool 
Elevator::checkDoorObstructed()
{
    panel_->SetOverloadState(doorObstructed_);
    return doorObstructed_;
}

void 
Elevator::OpenDoor()
{
    doorState_ = DoorState::OPEN;
}

void 
Elevator::CloseDoor()
{
    // TODO: There should be a check to see if the door can close (obstruction or open door button)
    doorState_ = DoorState::CLOSED;
}