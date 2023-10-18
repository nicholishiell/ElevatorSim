``` mermaid
sequenceDiagram
    ECS->>ElevatorPanel:  SetDoor(CLOSED)
    ElevatorPanel->>DoorObstructionSensor: GetSensorState()
    alt sensorSttate
        DoorObstructionSensor-->>ElevatorPanel: Activate
        ElevatorPanel->>ElevatorPanel: PlayAudioMsg(Type::DoorObstructed) 
        ElevatorPanel->>ElevatorPanel: SetDisplayMsg("Door Obstructed")
    else
        DoorObstructionSensor-->>ElevatorPanel: Inactive
        ElevatorPanel->>ElevatorPanel:CloseDoor()
        ElevatorPanel-->>ECS: UpdateDoorState(CLOSED)
    end
```

Safety Use Case 4: Door Obstructed Sensor