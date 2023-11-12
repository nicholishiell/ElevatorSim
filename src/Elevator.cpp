#include "include/Elevator.h"
#include "include/Floor.h"

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
    panel_->SetEmergency(false);
}

void 
Elevator::Update(   const float timeStep, 
                    FloorSharedPtrVector floors)
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

    for(uint iPerson = 0; iPerson < this->GetNumberOfPeople(); iPerson++)
    {
        auto person = peopleOnboard_[iPerson];

        if(this->IsAtFloor(person->GetDestinationFloor()))
        {
            person->Arrived();
            floors[person->GetDestinationFloor()]->AddPerson(person);
            peopleOnboard_.erase(peopleOnboard_.begin()+iPerson);
            iPerson--;
        }
        else
        {
            this->panel_->ServiceFloorButtonRequest(person->GetDestinationFloor());
        }
    }
}

bool 
Elevator::IsAtFloor(const int floorIndex) const
{
    return panel_->IsAtFloor(floorIndex);
}

void 
Elevator::notifyObservers()
{
    for(auto observer : observers_)
        observer->Notify();
}

void 
Elevator::ToConsole() const
{
    // Display info to the console
    std::cout   << label_ <<"\t"  
                << panel_->FloorPosString() << "\t"
                << height_ << "\t" << elevatorState_->GetStateString() << std::endl;
    std::cout   << "CUR REQ: " 
                << "lvl: " << panel_->GetCurrentlyServicing().level << "\t" 
                << "dir: " << panel_->GetCurrentlyServicing().direction << std::endl;
    std::cout << "Route: " << std::endl;
    panel_->DisplayRoute();
}

void 
Elevator::UpdateHeight(const float deltaH)
{
    auto updatedHeight = height_ + deltaH;
    
    // TODO: Figure out how to calculate this better
    auto maxHeight = 4.*FLOOR_HEIGHT_METERS;

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

    notifyObservers();
}

void 
Elevator::SetHeight(const float height)
{
    height_ = height;
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
}

bool 
Elevator::CheckOverloaded()
{
    float currentWeight = 0.;
    for(auto person : peopleOnboard_)
    {
        currentWeight = currentWeight + person->GetWeight();
    }

    auto overloaded = currentWeight > ELEVATOR_MAX_WEIGHT;

    if(overloaded)
    {
        panel_->SetOverloadState(overloaded);
        panel_->DisplayMessage("!!! OVERLOADED !!!");
        panel_->AudioMessage();
    }

    return overloaded;
}

void 
Elevator::HandleDoorObstructed(const int floorIndex)
{
    if(panel_->IsAtFloor(floorIndex))
    {
        if(doorState_ == DoorState::OBSTRUCTED)
        {   
            doorState_ = DoorState::OPEN;
            panel_->SetDoorObstructedState(false);
        }
        else if(doorState_ == DoorState::OPEN)
        {
            doorState_ = DoorState::OBSTRUCTED;
            panel_->SetDoorObstructedState(true);
        }
    }
}

void 
Elevator::OpenDoor()
{
    doorState_ = DoorState::OPEN;
}

void 
Elevator::CloseDoor()
{
    if(doorState_ != DoorState::OBSTRUCTED)
        doorState_ = DoorState::CLOSED;
}

void 
Elevator::AddPerson(PersonSharedPtr p)
{
    peopleOnboard_.emplace_back(p);
}

void 
Elevator::RemovePerson(PersonSharedPtr p)
{
    auto b = peopleOnboard_.begin();
    auto e = peopleOnboard_.end();

    peopleOnboard_.erase(std::remove(b,e,p),e);
}