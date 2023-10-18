``` mermaid
sequenceDiagram
    actor passenger
    
    activate passenger
    passenger->>FloorButton: Pressed level 3 button
    deactivate passenger
    activate FloorButton
    FloorButton->>ElevatorPanel: Update(level=3)
    deactivate FloorButton
    activate ElevatorPanel
    ElevatorPanel->>ElevatorPanel: GenerateServiceRequest(3, "DOWN")
    ElevatorPanel->>ElevatorPanel: CheckServiceRequest(3, "DOWN")
    ElevatorPanel->>ElevatorPanel: AddToRoute(3, "DOWN")
    ElevatorPanel->>FloorButton: SetLight(ON)
    deactivate ElevatorPanel 
```
Success Use Case 2: Passenger selects a destination floor from the elevator panel.