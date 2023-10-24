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

        floors_.emplace_back(newFloor);

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
        
        elevators_.emplace_back(newElevator);

        for(auto floor : floors_)
            floor->SetNumElevators(static_cast<int>(elevators_.size()));
    }
    else
        std::cout << "Maximum # of Elevators reached" << std::endl;
}

// Public SLOTS
void 
BuildingSimulator::HandleServiceRequest(const ServiceRequest request)
{
    controller_->HandleServiceRequest(request, elevators_, floors_);
}

void 
BuildingSimulator::HandleFireAlarm(const int level)
{
    controller_->HandleFireAlarm(level, elevators_, floors_);
}

void 
BuildingSimulator::HandlePowerOutageAlarm()
{
    controller_->HandlePowerOutageAlarm(elevators_, floors_);
}

//
void
BuildingSimulator::Step(const float timeStep)
{
    this->mandatoryStep(timeStep);

    // Let the user defined controller do any periodic work it needs to do
    controller_->Step(timeStep, elevators_, floors_);
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

    // Update the floors
    this->updateFloors();

    // TODO: Next update the people in the building
    // this->updatePeople()
}

void 
BuildingSimulator::updateFloors()
{
    for(auto floor : floors_)
        floor->GetPanel()->LightsOut();

    for(unsigned int i = 0; i < elevators_.size(); i++)
    {
        auto elevator = elevators_[i];
        auto level = elevator->GetLevel();
        auto currentRequest = elevator->GetCurrentlyServicing();

        // Turn elevator light for the floor it is currently on
        floors_[level]->GetPanel()->SetElevatorLight(i, LightState::ON);

        if(currentRequest.level == level)
        {            
            // turn off service call lights
            if(currentRequest.direction == RequestDirection::REQ_UP)
            {
                floors_[currentRequest.level]->GetPanel()->UpRequestServiced();
            }
            else if(currentRequest.direction == RequestDirection::REQ_DOWN)
            {
                floors_[currentRequest.level]->GetPanel()->DownRequestServiced();
            }

        }
    }
}