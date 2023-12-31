#include "include/Floor.h"
#include "include/ElevatorPanel.h"

Floor::Floor(   const std::string label, 
                const int level)
{
    label_ = label;
    level_ = level;

    panel_ = nullptr;
}

Floor::~Floor()
{

}  

void 
Floor::UpRequestServiced()
{
    this->GetPanel()->UpRequestServiced();
}

void 
Floor::DownRequestServiced()
{
    this->GetPanel()->DownRequestServiced();
}

void
Floor::Update(ElevatorSharedPtrVector elevators)
{
    panel_->LightsOut();
    for(unsigned int iElevator = 0; iElevator < elevators.size(); iElevator++)
    {
        auto elevator = elevators[iElevator];
        auto elevatorPanel = elevator->GetPanel(); 
        
        // Update the lights  
        updateLights(elevatorPanel, iElevator);

        // Update the people
        for(uint iPerson = 0; iPerson < GetNumberOfPeople(); iPerson++)
        {
            auto person = peopleOnFloor_[iPerson];
           
            if(elevator->IsAtFloor(level_))
            {
                std::cout << person->GetName() <<" is getting on the elevator" << std::endl;
                if( (elevatorPanel->IsGoingUp() && person->IsGoingUp()))
                {
                    elevator->AddPerson(person);
                    peopleOnFloor_.erase(peopleOnFloor_.begin()+iPerson);
                    iPerson--;
                }
                else if(elevatorPanel->IsGoingDown() && person->IsGoingDown())
                {
                    elevator->AddPerson(person);
                    peopleOnFloor_.erase(peopleOnFloor_.begin()+iPerson);
                    iPerson--;
                }               
            }

            if(person->IsGoingUp() && !panel_->IsUpRequested())
            {
                std::cout << person->GetName() <<" is requesting an UP elevator" << std::endl;
                panel_->UpButtonPresssed();
            }
            else if(person->IsGoingDown() && !panel_->IsDownRequested())
            {
                std::cout << person->GetName() <<" is requesting a DOWN elevator" << std::endl;
                panel_->DownButtonPresssed();
            }
        }
    }  
}

void 
Floor::updateLights(ElevatorPanelSharedPtr ep, const int i)
{
    // Turn on indicator light if elevator was at floor.
    if(ep->GetSensor()->GetNearestLevel() == level_)
        panel_->SetElevatorLight(i, LightState::ON);

    // Turn off the service light if applicable
    if(ep->IsAtFloor(level_))
    {            
        auto currentRequest = ep->GetCurrentlyServicing();
        // turn off service call lights
        if(currentRequest.direction == RequestDirection::REQ_UP)
        {
            panel_->UpRequestServiced();
        }
        else if(currentRequest.direction == RequestDirection::REQ_DOWN)
        {
            panel_->DownRequestServiced();
        }
    }
}