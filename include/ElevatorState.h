#ifndef ELEVATOR_STATE_H
#define ELEVATOR_STATE_H

#include <string>

#include "include/Utility.h"
#include "include/Elevator.h"

class ElevatorState 
{

public:
    ElevatorState(const std::string& s) 
    {
        stateString_ = s;
    };

    std::string GetStateString() const {return stateString_;}

    virtual ElevatorStateUniquePtr Update(  ElevatorSharedPtr elevator, 
                                            const float timeStep) = 0;
protected:

    std::string stateString_;

};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class ElevatorStateUp: public ElevatorState
{
public:
    ElevatorStateUp() : ElevatorState("UP"){}
    
    ElevatorStateUniquePtr Update(   ElevatorSharedPtr elevator, 
                                    const float timeStep);

private:

};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class ElevatorStateDown: public ElevatorState
{
public:
    ElevatorStateDown() : ElevatorState("DOWN"){}

    ElevatorStateUniquePtr Update(  ElevatorSharedPtr elevator, 
                                    const float timeStep);

private:

};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class ElevatorStateArrived: public ElevatorState
{
public:
    ElevatorStateArrived() : ElevatorState("ARRIVED"){}

    ElevatorStateUniquePtr Update(  ElevatorSharedPtr elevator, 
                                    const float timeStep);
    
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class ElevatorStateWaiting: public ElevatorState
{

public:

    ElevatorStateWaiting() : ElevatorState("WAITING"){}
    
    ElevatorStateUniquePtr Update(  ElevatorSharedPtr elevator, 
                                    const float timeStep);

private:

    float timeSpentWaiting_ = 0.;

};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class ElevatorStateIdle: public ElevatorState
{
public:
    ElevatorStateIdle() : ElevatorState("IDLE"){}
    
    ElevatorStateUniquePtr Update(  ElevatorSharedPtr elevator, 
                                    const float timeStep);
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class ElevatorStateLeaving: public ElevatorState
{
public:
    ElevatorStateLeaving() : ElevatorState("LEAVING"){}
    
    ElevatorStateUniquePtr Update(  ElevatorSharedPtr elevator, 
                                    const float timeStep);
    
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class ElevatorStateDisabled: public ElevatorState
{
public:
    ElevatorStateDisabled() : ElevatorState("DISABLED"){}
    
    ElevatorStateUniquePtr Update(  ElevatorSharedPtr elevator, 
                                    const float timeStep); 
};

#endif
