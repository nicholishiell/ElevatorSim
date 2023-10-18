``` mermaid
sequenceDiagram
    actor User
    User->>FireAlarmButton: Presses Button
    activate FireAlarmButton
    FireAlarmButton->>BuildingPanel:UpdateState(ButtonState::Pressed, level = 2)
    deactivate FireAlarmButton
    activate BuildingPanel
    BuildingPanel->>ECS: RaiseFireAlarm(2)
    deactivate BuildingPanel
    activate ECS
    ECS->>ElevatorPanelA: EmergencyOverride(EmergType::Fire,2)
    ECS->>ElevatorPanelB: EmergencyOverride(EmergType::Fire,2)
    deactivate ECS
    activate ElevatorPanelA
    activate ElevatorPanelB
    ElevatorPanelA->>ElevatorPanelA: ClearRoute()
    ElevatorPanelB->>ElevatorPanelB: ClearRoute()
    
    ElevatorPanelA->>ElevatorPanelA: PlayAudioMsg(Type::Fire) 
    ElevatorPanelB->>ElevatorPanelB: PlayAudioMsg(Type::Fire) 

    ElevatorPanelA->>ElevatorPanelA: SetDisplayMsg("Fire")
    ElevatorPanelB->>ElevatorPanelB: SetDisplayMsg("Fire")
    ElevatorPanelA->>ElevatorPanelA: GetCloseLevel()
    ElevatorPanelA->>ElevatorPanelA: CheckAgainstFireLevel(2)
    ElevatorPanelA->>ElevatorPanelA: AddToRoute(1,"DOWN")
    ElevatorPanelA->>ECS: Arrived()
    ElevatorPanelA->>ElevatorPanelA: GetCloseLevel()
    ElevatorPanelA->>ElevatorPanelA: CheckAgainstFireLevel(2)
    ElevatorPanelB->>ElevatorPanelB: AddToRoute(3,"DOWN")
    ElevatorPanelB->>ECS: Arrived()
    deactivate ElevatorPanelA
    deactivate ElevatorPanelB
    activate ECS
    ECS->>ElevatorPanelA: SetState(ElevatorState::Disabled)
    ECS->>ElevatorPanelB: SetState(ElevatorState::Disabled)
    deactivate ECS
```

Safety Use Case 5: Fire Alarm