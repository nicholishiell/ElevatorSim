#include "include/ElevatorState.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ElevatorState*
ElevatorStateIdle::Update(  ElevatorSharedPtr elevator, 
                            const float timeStep)
{
    elevator->CloseDoor();

    if(!elevator->IsRouteEmpty())
    {
        return new ElevatorStateLeaving();
    }
    else
    {
        elevator->SetCurrentlyServicing(ServiceRequest(elevator->GetLevel(), RequestDirection::REQ_IDLE));
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

    // // Get the next route off the route vector
    elevator->SetCurrentlyServicing(elevator->PopRoute());

    // Determine which direction to head
    if(elevator->GetCurrentlyServicing().level > elevator->GetLevel())
        return  new ElevatorStateUp();
    else if(elevator->GetCurrentlyServicing().level < elevator->GetLevel())
        return new ElevatorStateDown();
    else
        return new ElevatorStateIdle();

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

    if(elevator->GetLevel() == elevator->GetCurrentlyServicing().level)
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
    
    if(elevator->GetHeight() + delta > 0.)
    {
        elevator->UpdateHeight(delta);
    }
    else
    {
        elevator->SetHeight(0.);
    }

    if(elevator->GetLevel() == elevator->GetCurrentlyServicing().level)
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
    timeSpentWaiting_ = timeSpentWaiting_ + timeStep;
    
    if(timeSpentWaiting_ > ELEVATOR_WAIT_TIME)
        return  new ElevatorStateIdle();
    else
        return nullptr;  
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ElevatorState*
ElevatorStateDisabled::Update(  ElevatorSharedPtr elevator, 
                                const float timeStep)
{
    return nullptr;
}