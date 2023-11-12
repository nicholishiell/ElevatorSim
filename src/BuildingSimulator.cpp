#include "include/BuildingSimulator.h"

#include "include/GraphicsGenerator.h"

BuildingSimulator::BuildingSimulator(ControllerSharedPtr controller)
{
    buildingPanel_ = nullptr;
    controller_ = controller;

    graphicsObserver_ = nullptr;

    initialized_ = false;
}

BuildingSimulator::~BuildingSimulator()
{

} 

// Add a floor to the control system
void 
BuildingSimulator::AddFloor(std::string label)
{
    auto numFloors = static_cast<int>(floors_.size());

    if(numFloors <= MAX_NUMBER_OF_FLOORS)
    {
        auto newFloor = std::make_shared<Floor>(label, numFloors);
        floors_.emplace_back(newFloor);
    }
    else
    {
        std::cout << "Maximum # of Floors reached" << std::endl;
    }
}

// Add an elevator to the control system
void 
BuildingSimulator::AddElevator( std::string label, 
                                DoorState state, 
                                const float height)
{
    auto numElevators = static_cast<int>(elevators_.size());

    if(numElevators <= MAX_NUMBER_OF_ELEVATORS)
    {
        auto numFloors = static_cast<int>(floors_.size());
        auto newElevator = std::make_shared<Elevator>(label, state, height);
        
        elevators_.emplace_back(newElevator);
    }
    else
        std::cout << "Maximum # of Elevators reached" << std::endl;
}

void 
BuildingSimulator::Initialize()
{               
    buildingPanel_ = std::make_shared<BuildingPanel>(this->GetNumberOfFloors()); 
    QObject::connect(buildingPanel_.get(),&BuildingPanel::EmergencyRequested,this,&BuildingSimulator::HandleEmergencyRequest);  
    QObject::connect(buildingPanel_.get(),&BuildingPanel::EnableElevators,this,&BuildingSimulator::HandleEnableElevators);
    QObject::connect(buildingPanel_.get(),&BuildingPanel::AnswerHelpRequest,this,&BuildingSimulator::HandleAnswerHelp);

    personGenerator_ = new PersonGenerator(floors_);

    for(auto& floor : floors_)
    {
        auto panel = std::make_shared<FloorPanel>(  floor->GetLabel(), 
                                                    this->GetNumberOfElevators(), 
                                                    floor->GetLevel());
        QObject::connect(panel.get(), &FloorPanel::ServiceRequested, this, &BuildingSimulator::HandleServiceRequest);

        floor->SetPanel(panel);
    }

    for(auto& elevator : elevators_)
    {
        auto sensor = std::make_shared<ElevatorPositionSensor>(this->GetNumberOfFloors(),
                                                               elevator);

        auto panel = std::make_shared<ElevatorPanel>(   elevator->GetLabel(),
                                                        this->GetNumberOfFloors(),
                                                        sensor);
                                                        
        QObject::connect(panel.get(), &ElevatorPanel::HelpRequested, this, &BuildingSimulator::HandleHelpRequest);      
        QObject::connect(buildingPanel_.get(),&BuildingPanel::ObstructDoor, elevator.get(),&Elevator::HandleDoorObstructed);

        elevator->SetPanel(panel);
        elevator->AddObserver(panel.get());
        elevator->AddObserver(sensor.get());
    }

    initialized_ = true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Public SLOTS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void 
BuildingSimulator::HandleServiceRequest(const ServiceRequest request)
{
    if(!initialized_) Initialize();

    controller_->HandleServiceRequest(request, elevators_, floors_);
}

void
BuildingSimulator::HandleEnableElevators()
{
    for(auto elevator : elevators_)
        elevator->Enable();
}

void 
BuildingSimulator::HandleHelpRequest(ElevatorPanelSharedPtr panel)
{
    controller_->HandleHelpRequest(panel, buildingPanel_);
}

void 
BuildingSimulator::HandleAnswerHelp(ElevatorPanelSharedPtr panel)
{
    controller_->HandleAnswerHelp(panel);
}

void
BuildingSimulator::HandleEmergencyRequest(const EmergencyRequest request)
{
    if(request.type == EmergencyType::FIRE)
    {
        controller_->HandleFireAlarm(request.level, elevators_, floors_);   
    }
    else if(request.type == EmergencyType::POWER_OUTAGE)
    {
        controller_->HandlePowerOutageAlarm(elevators_, floors_);
    }
    else
    {
        std::cout << "Unknown Emergency Request." << std::endl;
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Updating the simulations one time step
void
BuildingSimulator::Update(const float timeStep)
{
    if(!initialized_) Initialize();

    this->updateElevators(timeStep);

    this->updateFloors(timeStep);

    // Let the user defined controller do any periodic work it needs to do
    controller_->Step(timeStep, elevators_, buildingPanel_);

    // Notify the graphics generator that things have changed.
    if(graphicsObserver_ != nullptr)
        graphicsObserver_->update();
}

void 
BuildingSimulator::updateElevators(const float timeStep)
{
    // Move all the elevators
    for(auto elevator : elevators_)
    {
        elevator->Update(timeStep, floors_);
    }
}

void 
BuildingSimulator::updateFloors(const float timeStep)
{
    for(auto floor : floors_)
    {
        floor->Update(elevators_);
    }
}
