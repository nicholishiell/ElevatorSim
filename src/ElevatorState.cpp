#include "include/ElevatorState.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ElevatorStateUniquePtr
ElevatorStateIdle::Update(  ElevatorSharedPtr elevator, 
                            const float timeStep)
{
    if(!elevator->IsRouteEmpty())
    {
        return std::make_unique<ElevatorStateLeaving>();
    }
    else
    {
        elevator->UpdateCurrentLevel();
        //elevator->AddToRoute(ServiceRequest(elevator->GetLevel(), RequestDirection::REQ_IDLE));
        return std::make_unique<ElevatorStateIdle>();
    }

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ElevatorStateUniquePtr
ElevatorStateLeaving::Update(   ElevatorSharedPtr elevator, 
                                const float timeStep)
{
    elevator->GetPanel()->RingBell();

    // TODO: to make sure the door is closed and we are not overloaded, or there is a fire
    elevator->CloseDoor();

    // Get the next route off the route vector
    elevator->SetCurrentlyServicing(elevator->PopRoute());

    // Determine which direction to head
    if(elevator->GetCurrentlyServicing().level > elevator->GetLevel())
        return std::make_unique<ElevatorStateUp>();
    else if(elevator->GetCurrentlyServicing().level < elevator->GetLevel())
        return std::make_unique<ElevatorStateDown>();
    else
        return std::make_unique<ElevatorStateIdle>();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ElevatorStateUniquePtr
ElevatorStateUp::Update(ElevatorSharedPtr elevator, 
                        const float timeStep)
{
    // TODO: there needs to be a check here to make sure the elevator is not at the top of the building
    auto delta = timeStep*ELEVATOR_SPEED;
    elevator->UpdateHeight(delta);

    if(elevator->GetLevel() == elevator->GetCurrentlyServicing().level)
    {
        return std::make_unique<ElevatorStateArrived>();
    }
    else
    {
        return std::make_unique<ElevatorStateUp>(this);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ElevatorStateUniquePtr
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
        return std::make_unique<ElevatorStateArrived>();
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ElevatorStateUniquePtr
ElevatorStateArrived::Update(   ElevatorSharedPtr elevator, 
                                const float timeStep)
{
    elevator->GetPanel()->RingBell();
    
    // Open the door
    elevator->OpenDoor();
    
    // Change state to waiting
    return std::make_unique<ElevatorStateWaiting>();
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ElevatorStateUniquePtr
ElevatorStateWaiting::Update(   ElevatorSharedPtr elevator, 
                                const float timeStep)
{
    timeSpentWaiting_ = timeSpentWaiting_ + timeStep;
    
    if(timeSpentWaiting_ > ELEVATOR_WAIT_TIME)
        return std::make_unique<ElevatorStateIdle>();
    

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ElevatorStateUniquePtr
ElevatorStateDisabled::Update(  ElevatorSharedPtr elevator, 
                                const float timeStep)
{
    return std::make_unique<ElevatorStateDisabled>();
}