``` mermaid
sequenceDiagram
    actor Building Safety Service
    actor 911_Operator
    actor passenger
    activate passenger
    passenger->>ElevatorPanel: Press HELP button
    deactivate passenger
    activate ElevatorPanel
    ElevatorPanel->>ECS: HelpButton(Pressed)
    deactivate ElevatorPanel
    activate ECS
    ECS->>Building Safety Service: Call(BSS)
    ECS->>ECS: Wait 5 Seconds
    alt BSS Picks Up
        Building Safety Service-->>ECS: Response
        ECS->>Building Safety Service: ConnectToPanel()
        Building Safety Service-->>ElevatorPanel: Connected
    else
        Building Safety Service-->>ECS: No Response
        ECS->>911_Operator: Call(911)
        911_Operator-->>ECS: Response
        ECS->>911_Operator: ConnectToPanel()
        911_Operator-->>ElevatorPanel: Connected
    end

    deactivate ECS
```

Safety Use Case 3: Pressing the Help button