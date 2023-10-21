#include "include/Elevator.h"

#include <cmath>
#include <sstream>  

Elevator::Elevator( const std::string label, DoorState doorState, int level, int numFloors)
{
    doorState_ = doorState;
    currentLevel_ = level;
    label_ = label;

    elevatorState_ = ElevatorState::IDLE;
    
    height_ = level*FLOOR_HEIGHT_METERS;
    speed_ = 0.5;

    numberOfFloors_ = numFloors;
    panel_ = std::make_shared<ElevatorPanel>(label,numFloors);

    maxWeight_ = 500.;
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

    if(elevatorState_ == ElevatorState::UP)
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
    else if(elevatorState_ == ElevatorState::DOWN)
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

std::string 
Elevator::StateString() const
{
    std::string stateString = "";

    if(elevatorState_ == ElevatorState::IDLE)
        stateString = "IDLE";
    else if(elevatorState_ == ElevatorState::ARRIVED)
        stateString = "ARRIVED";
    else if(elevatorState_ == ElevatorState::UP)
        stateString = "UP";
    else if(elevatorState_ == ElevatorState::DOWN)
        stateString = "DOWN";
    else if(elevatorState_ == ElevatorState::WAITING)
        stateString = "WAITING";
    else if(elevatorState_ == ElevatorState::LEAVING)
        stateString = "LEAVING";
    else
        stateString = "STOP";
    
    return stateString;
}

void 
Elevator::Update(const float timeStep)
{
    std::cout << label_ << "\t" << currentLevel_ << "\t" << height_ << "\t" << elevatorState_ << "\t" << StateString() << std::endl;
    std::cout << "CUR REQ: " << GetCurrentlyServicing().level << "\t" << GetCurrentlyServicing().direction << std::endl;
    std::cout << "cur lvl: " << currentLevel_ << std::endl;
    std::cout << "Route: ";
    for(auto r : route_)
        std::cout << "(" << r.level << ", " << r.direction << ") ";
    std::cout << std::endl;
    
    // TODO: Move request handling to it's open function
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

    // TODO: Move this another function
    if(elevatorState_ == ElevatorState::IDLE)
        idle(timeStep);
    else if(elevatorState_ == ElevatorState::UP)
        up(timeStep);
    else if(elevatorState_ == ElevatorState::DOWN)
        down(timeStep);
    else if(elevatorState_ == ElevatorState::ARRIVED)
        arrived(timeStep);
    else if(elevatorState_ == ElevatorState::WAITING)
        waiting(timeStep);
    else if(elevatorState_ == ElevatorState::STOPPED)
        stopped(timeStep);
    else if(elevatorState_ == ElevatorState::LEAVING)
        leaving(timeStep);
    else    
        std::cout << "Elevator::Update: Unknown state!" << std::endl;    
}

std::string
int_to_string(const int i)
{
    std::stringstream ss;
    std::string s;
    ss << i;
    ss >> s;
    return s;
}

void 
Elevator::updateCurrentLevel()
{
    for(int i = 0; i < numberOfFloors_; i++)
    {
        if( std::fabs(height_ - i*FLOOR_HEIGHT_METERS) < 0.01 )
            currentLevel_ = i; 
    }

    panel_->DisplayMessage(int_to_string(currentLevel_));
}

ServiceRequest 
Elevator::popRoute()
{      
    auto nextRequest = route_[0];
    route_.front() = std::move(route_.back());
    route_.pop_back();

    return nextRequest;
}

bool 
Elevator::checkOverloaded()
{
    float currentWeight = 0.;
    for(auto person : passengers_)
        currentWeight = currentWeight + person.GetWeight();

    auto overloaded = currentWeight > maxWeight_;

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


void Elevator::idle(const float timeStep)
{
    // TODO: There should be a check to see if the door can close (obstruction or open door button)
    doorState_ = DoorState::CLOSED;

    if(!route_.empty())
    {
        elevatorState_ = ElevatorState::LEAVING;
    }
    else
    {
        updateCurrentLevel();
        currentlyServicing_ = ServiceRequest(currentLevel_,RequestDirection::REQ_IDLE);
        elevatorState_ = ElevatorState::IDLE;
    }
}

void 
Elevator::leaving(const float timeStep)
{
    panel_->RingBell();

    // TODO: to make sure the door is closed and we are not overloaded, or there is a fire
    
    // Get the next route off the route vector
    currentlyServicing_ = popRoute();

    // Determine which direction to head
    // TODO: maybe there should only be a MOVING state
    if(currentlyServicing_.level > currentLevel_)
        elevatorState_ = ElevatorState::UP;
    else if(currentlyServicing_.level < currentLevel_)
        elevatorState_ = ElevatorState::DOWN;
    else
        elevatorState_ = ElevatorState::IDLE;
}


void Elevator::up(const float timeStep)
{    
    // TODO: there needs to be a check here to make sure the elevator is not at the top of the building
    auto delta = timeStep*speed_;
    height_ += delta;

    updateCurrentLevel();

    if(currentLevel_ == currentlyServicing_.level)
    {
        elevatorState_ = ElevatorState::ARRIVED;
    }

}

void Elevator::down(const float timeStep)
{
    auto delta = -1.*timeStep*speed_;
    
    if(height_ + delta > 0.)
    {
        height_ += delta;

    }
    else
    {
        height_ = 0.;
    }

    updateCurrentLevel();

    if(currentLevel_ == currentlyServicing_.level)
    {
        elevatorState_ = ElevatorState::ARRIVED;
    }
}
void Elevator::arrived(const float timeStep)
{
    panel_->RingBell();

    // Restart the wait timer
    timeSpentWaiting_ = 0.;

    // Open the door
    doorState_ = DoorState::OPEN;
    
    // Change state to waiting
    elevatorState_ = ElevatorState::WAITING;
}

void Elevator::waiting(const float timeStep)
{
    timeSpentWaiting_ = timeSpentWaiting_ + timeStep;
    if(timeSpentWaiting_ > ELEVATOR_WAIT_TIME)
        elevatorState_ = ElevatorState::IDLE;
}

void Elevator::stopped(const float timeStep)
{

}