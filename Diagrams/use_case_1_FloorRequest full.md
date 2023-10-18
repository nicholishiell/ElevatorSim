``` mermaid
sequenceDiagram
    actor passenger
    
    passenger->>FloorPanel: Pressed UP button
    
    activate FloorPanel
    FloorPanel->>FloorPanel: SetUpLight(On)
    FloorPanel->>ECS: GenerateServiceRequest(level=3, UP)
    deactivate FloorPanel

    activate ECS
    ECS->>ElevatorA: OfferRequest(3,UP)
    ECS->>ElevatorB: OfferRequest(3,UP)
    ElevatorA-->>ECS: Response Decline
    ElevatorB-->>ECS: Response Accept
    ECS->>ElevatorB: AddToRoute(3,UP)
    deactivate ECS

    activate ElevatorB
    ElevatorB->>ElevatorB: CheckOverLoaded()
    ElevatorB->>ElevatorB: Move to Requested Floor
    ElevatorB->>ECS: UpdateState(Arrived)
    deactivate ElevatorB

    activate ECS
    ECS->>FloorPanel: SetUpLight(OFF)
    ECS->>FloorPanel: RingBell()
    ECS->>ElevatorB: SetDoor(OPEN)
    ElevatorB->>ElevatorB: OpenDoor()
    passenger->>ElevatorB: Enters Elevator    
    ElevatorB->>ElevatorB: Wait(10)

    ElevatorB->>ECS: UpdateState(Ready)
    ECS->>FloorPanel: RingBell()
    ECS->>ElevatorB: SetDoor(CLOSE)
    deactivate ECS

    activate ElevatorB
    ElevatorB->>ElevatorB: CheckObstruction() 
    ElevatorB->>ElevatorB: CheckOpenDoorButton()
    ElevatorB->>ElevatorB: CloseDoor()
    deactivate ElevatorB
```

Success Use Case 1: Passenger calls an elevator to their floor