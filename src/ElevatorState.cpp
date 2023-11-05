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
        panel->SetCurrentlyServicing(ServiceRequest(panel->GetPreviousFloor(), RequestDirection::REQ_IDLE));
        return nullptr;
    }

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ElevatorState*
ElevatorStateLeaving::Update(   ElevatorSharedPtr elevator, 
                                const float timeStep)
{
    elevator->GetPanel()->RingBell();

    // TODO: to make sure the door is closed and we are not overloaded, or there is a fire
    elevator->CloseDoor();

    auto panel = elevator->GetPanel();

    // Get the next route off the route vector
    panel->PopRoute();

    // Determine which direction to head
    if(panel->GetCurrentlyServicing().level > panel->GetPreviousFloor())
        return  new ElevatorStateUp();
    else if(panel->GetCurrentlyServicing().level < panel->GetPreviousFloor())
        return new ElevatorStateDown();
    else
        return new ElevatorStateArrived();

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
    
    auto panel = elevator->GetPanel();

    if(elevator->GetHeight() + delta > 0.)
    {
        elevator->UpdateHeight(delta);
    }
    else
    {
        elevator->SetHeight(0.);
    }

    if(panel->IsAtTargetFloor())
    {
        return new ElevatorStateArrived();
    }

    return nullptr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ElevatorState*
ElevatorStateArrived::Update(   ElevatorSharedPtr elevator, 
                                const float timeStep)
{
    elevator->GetPanel()->RingBell();
    
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
        return  new ElevatorStateIdle();
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