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
    ECS->>ECS: GetOptimalElevator()
    ECS->>ElevatorB: AddToRoute(3,UP)
    deactivate ECS
```

Success Use Case 1: Passenger calls an elevator to their floor