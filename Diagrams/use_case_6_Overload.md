``` mermaid
sequenceDiagram
    activate Elevator
    Elevator->>ElevatorPanel: popNextRequest()
    ElevatorPanel->>ElevatorPanel: requests_.pop()
    ElevatorPanel-->>Elevator:  serviceRequest(1, 'DOWN')
    Elevator->>ElevatorPanel: CheckOverloadSensor()
    deactivate Elevator
    activate ElevatorPanel
    ElevatorPanel->>OverloadSensor: GetState()
    alt sensorState
        OverloadSensor-->>ElevatorPanel: SensorState::Active
        ElevatorPanel->>ElevatorPanel: SetOverLoadIndicatorLight(ON)
        ElevatorPanel->>ElevatorPanel: SetDisplayMsg("Elevator Overloaded")
        deactivate ElevatorPanel
    else
        activate ElevatorPanel
        OverloadSensor-->>ElevatorPanel: SensorState::Inactive
        ElevatorPanel-->>Elevator: SensorState::Inactive
        deactivate ElevatorPanel
        activate Elevator
        Elevator->>Elevator: SetState(ElevatorState::Moving)
    end
    deactivate Elevator
```

Safety Use Case 6: Overload Sensor