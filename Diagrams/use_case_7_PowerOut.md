``` mermaid
sequenceDiagram
    activate BuildingPanel
    BuildingPanel->>ECS: RaisePowerOutageAlarm()
    deactivate BuildingPanel
    activate ECS
    ECS->>ElevatorA: EmergencyOverride(EmergType::PowerOutage)
    ECS->>ElevatorB: EmergencyOverride(EmergType::PowerOutage)
    deactivate ECS
    activate ElevatorA
    activate ElevatorB
    ElevatorA->>ElevatorA: ClearRoute()
    ElevatorB->>ElevatorB: ClearRoute()
    
    ElevatorA->>ElevatorA: PlayAudioMsg(Type::PowerOutMsg) 
    ElevatorB->>ElevatorB: PlayAudioMsg(Type::PowerOutMsg) 

    ElevatorA->>ElevatorA: SetDisplayMsg("Power Outage")
    ElevatorB->>ElevatorB: SetDisplayMsg("Power Outage")
    ElevatorA->>ElevatorA: AddToRoute(1,"DOWN")
    ElevatorA->>ECS: Arrived()
    ElevatorB->>ElevatorB: AddToRoute(3,"DOWN")
    ElevatorB->>ECS: Arrived()
    deactivate ElevatorA
    deactivate ElevatorB
    activate ECS
    ECS->>ElevatorA: SetState(ElevatorState::Disabled)
    ECS->>ElevatorB: SetState(ElevatorState::Disabled)
    deactivate ECS
```

Safety Use Case 7: Power Outage Alarm