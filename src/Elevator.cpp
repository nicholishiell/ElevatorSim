#include "include/Elevator.h"

#include <cmath>

Elevator::Elevator( const std::string label, 
                    DoorState doorState, 
                    const float height)
{
    doorState_ = doorState;
    label_ = label;

    elevatorState_ =  new ElevatorStateIdle();
    
    height_ = height;
    
    panel_ = nullptr;
}

Elevator::~Elevator()
{

} 

void 
Elevator::Disable()
{
    elevatorState_ = new ElevatorStateDisabled();
}

void 
Elevator::Enable()
{
     elevatorState_ =  new ElevatorStateIdle();
}

void 
Elevator::Update(const float timeStep)
{
    // Display data to console
    ToConsole();

    // Update the state
    ElevatorState* nextState = elevatorState_->Update(shared_from_this(), timeStep); 

    if(nextState != nullptr)
    {
        delete(elevatorState_);
        elevatorState_ = nextState;
    }
    
}

void 
Elevator::ToConsole() const
{
    // Display info to the console
    std::cout << label_ << "\t" << panel_->GetLevel() << "\t" << height_ << "\t" << elevatorState_->GetStateString() << std::endl;
    std::cout << "CUR REQ: " <<  panel_->GetCurrentlyServicing().level << "\t" <<  panel_->GetCurrentlyServicing().direction << std::endl;
    std::cout << "Route: " << std::endl;
    panel_->DisplayRoute();
}


void 
Elevator::UpdateHeight(const float deltaH)
{
    auto updatedHeight = height_ + deltaH;
    auto maxHeight = float(panel_->GetNumberOfFloors())*FLOOR_HEIGHT_METERS;

    if(updatedHeight > 0. && updatedHeight <= maxHeight)
    {
        SetHeight(updatedHeight);
    }
    else if(updatedHeight > maxHeight)
    {
        SetHeight(maxHeight);
    }
    else
    {
        SetHeight(0.);
    }
}

void 
Elevator::SetHeight(const float height)
{
    height_ = height;
    panel_->CalculateCurrentLevel(height_);
}

std::string 
Elevator::GetStateString() const 
{
    return elevatorState_->GetStateString();
}

void 
Elevator::SetPanel(ElevatorPanelSharedPtr panel)
{
    panel_ = panel;
    panel_->CalculateCurrentLevel(height_);
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