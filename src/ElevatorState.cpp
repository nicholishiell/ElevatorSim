#include "include/ElevatorState.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ElevatorState*
ElevatorStateIdle::Update(  ElevatorSharedPtr elevator, 
                            const float timeStep)
{
    auto panel = elevator->GetPanel();

    if(!panel->IsRouteEmpty())
    {
        return new ElevatorStateLeaving();
    }
    else
    {
        return nullptr;
    }

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ElevatorState*
ElevatorStateLeaving::Update(   ElevatorSharedPtr elevator, 
                                const float timeStep)
{
    auto panel = elevator->GetPanel();

    elevator->GetPanel()->RingBell();
    
    elevator->CloseDoor();

    if(elevator->GetDoorState() == DoorState::CLOSED)
    {
        if(!elevator->CheckOverloaded())
        {
            // Determine which direction to head
            if(panel->GetCurrentlyServicing().level > panel->GetPreviousFloor())
                return  new ElevatorStateUp();
            else if(panel->GetCurrentlyServicing().level < panel->GetPreviousFloor())
                return new ElevatorStateDown();
            else
                return new ElevatorStateArrived();
        }
    }
    else
    {
        timeSpentWaiting_ += timeStep;
        if(timeSpentWaiting_ > 5. && elevator->GetDoorState() == DoorState::OPEN)
        {
            panel->AudioMessage();
            panel->DisplayMessage("!!! DOOR OBSTRUCTED !!!");
        }
    }

    return nullptr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ElevatorState*
ElevatorStateUp::Update(ElevatorSharedPtr elevator, 
                        const float timeStep)
{
    // TODO: there needs to be a check here to make sure the elevator is not at the top of the building
    auto delta = timeStep*ELEVATOR_SPEED;
    elevator->UpdateHeight(delta);

    auto panel = elevator->GetPanel();

    if(panel->IsAtTargetFloor())
    {
        return new ElevatorStateArrived();
    }
    else
    {
        return nullptr;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ElevatorState*
ElevatorStateDown::Update(  ElevatorSharedPtr elevator, 
                            const float timeStep)
{
    auto delta = -1.*timeStep*ELEVATOR_SPEED;
    elevator->UpdateHeight(delta);

    auto panel = elevator->GetPanel();  
    if(panel->IsAtTargetFloor())
    {
        return new ElevatorStateArrived();
    }
    else
    {
        return nullptr;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ElevatorState*
ElevatorStateArrived::Update(   ElevatorSharedPtr elevator, 
                                const float timeStep)
{
    elevator->GetPanel()->RingBell();
    
    elevator->GetPanel()->Arrived();

    // Open the door
    elevator->OpenDoor();
    
    // Change state to waiting
    return  new ElevatorStateWaiting();
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ElevatorState*
ElevatorStateWaiting::Update(   ElevatorSharedPtr elevator, 
                                const float timeStep)
{
    auto panel = elevator->GetPanel();

    timeSpentWaiting_ = timeSpentWaiting_ + timeStep;
    
    if( (timeSpentWaiting_ > ELEVATOR_WAIT_TIME || panel->IsCloseDoorButtionActive()) && !panel->IsOpenDoorButtionActive())
    {
        elevator->CloseDoor();
        if(elevator->GetDoorState() == DoorState::CLOSED)
        {
            return  new ElevatorStateIdle();
        }
    }
    else
    {
        return nullptr;  
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ElevatorState*
ElevatorStateDisabled::Update(  ElevatorSharedPtr elevator, 
                                const float timeStep)
{
    auto panel = elevator->GetPanel();

    panel->AudioMessage();
    panel->RingBell();
    panel->DisplayMessage("!!!EMERGENCY!!!");

    if(!panel->IsAtTargetFloor())
    {
        auto delta = 0.f;
        if(panel->IsGoingDown())
        {
            delta = -1.*timeStep*ELEVATOR_SPEED;
        }
        else
        {
            delta = timeStep*ELEVATOR_SPEED;
        }
        
        elevator->UpdateHeight(delta);
    }
    else
    {
        elevator->OpenDoor();
        panel->SetCurrentlyServicing(ServiceRequest(panel->GetPreviousFloor(), RequestDirection::REQ_IDLE));
    } 

    return nullptr;
}