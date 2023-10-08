#include "include/BuildingSimulator.h"


BuildingSimulator::BuildingSimulator(ControllerSharedPtr controller)
{
    buildingPanel_ = std::make_shared<BuildingPanel>();
    
    QObject::connect(buildingPanel_.get(),&BuildingPanel::FireAlarm,this,&BuildingSimulator::HandleFireAlarm);
    QObject::connect(buildingPanel_.get(),&BuildingPanel::PowerOutageAlarm,this,&BuildingSimulator::HandlePowerOutageAlarm);

    controller_ = controller;
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
        newFloor->SetNumElevators(static_cast<int>(elevators_.size()));
        
        auto floorPanel = newFloor->GetPanel();
        QObject::connect(floorPanel.get(), &FloorPanel::ServiceRequested, this, &BuildingSimulator::HandleServiceRequest);

        addFloor(newFloor);

        for(auto elevator : elevators_)        
            elevator->SetNumberOfFloors(static_cast<int>(floors_.size()));
    }
    else
    {
        std::cout << "Maximum # of Floors reached" << std::endl;
    }
}

// Add an elevator to the control system
void 
BuildingSimulator::AddElevator(std::string label, int level, DoorState state)
{
    auto numElevators = static_cast<int>(elevators_.size());

    if(numElevators <= MAX_NUMBER_OF_ELEVATORS)
    {
        auto numFloors = static_cast<int>(floors_.size());
        auto newElevator = std::make_shared<Elevator>(label, state, level, numFloors);
        
        addElevator(newElevator);

        for(auto floor : floors_)
            floor->SetNumElevators(static_cast<int>(elevators_.size()));
    }
    else
        std::cout << "Maximum # of Elevators reached" << std::endl;
}


void 
BuildingSimulator::addElevator(ElevatorSharedPtr elevator)
{
    elevators_.emplace_back(elevator);
    controller_->AddElevator(elevator);
}

void 
BuildingSimulator::addFloor(FloorSharedPtr floor)
{
    floors_.emplace_back(floor);
    controller_->AddFloor(floor);
}

// Public SLOTS
void 
BuildingSimulator::HandleServiceRequest(const ServiceRequest request)
{
    controller_->AddServiceRequest(request);
}

void 
BuildingSimulator::HandleFireAlarm(const int level)
{
    controller_->HandleFireAlarm(level);
}

void 
BuildingSimulator::HandlePowerOutageAlarm()
{
    controller_->HandlePowerOutageAlarm();
}

//
void
BuildingSimulator::Step(const float timeStep)
{
    this->mandatoryStep(timeStep);

    controller_->Step(timeStep);
}

void 
BuildingSimulator::mandatoryStep(const float timeStep)
{
    // Move the elevators
    for(auto elevator : elevators_)
    {
        std::cout << "updating: " << elevator->GetLabel() << std::endl;
        elevator->Update(timeStep);
    }

    // TODO: Next update the people in the building
}
