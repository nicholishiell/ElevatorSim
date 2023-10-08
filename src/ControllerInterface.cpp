#include "include/ControllerInterface.h"


ControllerInterface::ControllerInterface()
{
    buildingPanel_ = std::make_shared<BuildingPanel>();
    
    QObject::connect(buildingPanel_.get(),&BuildingPanel::FireAlarm,this,&ControllerInterface::HandleFireAlarm);
    QObject::connect(buildingPanel_.get(),&BuildingPanel::PowerOutageAlarm,this,&ControllerInterface::HandlePowerOutageAlarm);
}

ControllerInterface::~ControllerInterface()
{

} 

// Add a floor to the control system
void 
ControllerInterface::AddFloor(std::string label)
{
    auto numFloors = static_cast<int>(floors_.size());

    if(numFloors <= MAX_NUMBER_OF_FLOORS)
    {
        auto newFloor = std::make_shared<Floor>(label, numFloors);
        newFloor->SetNumElevators(static_cast<int>(elevators_.size()));
        
        auto floorPanel = newFloor->GetPanel();
        QObject::connect(floorPanel.get(), &FloorPanel::ServiceRequested, this, &ControllerInterface::HandleServiceRequest);

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
ControllerInterface::AddElevator(std::string label, int level, DoorState state)
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
ControllerInterface::HandleServiceRequest(const ServiceRequest request)
{
    std::cout << "HandleServiceRequest" << std::endl;
    pendingRequests_.emplace_back(request);
}

void 
ControllerInterface::HandleFireAlarm(const int level)
{
    handleFireAlarm(level);
}

void 
ControllerInterface::HandlePowerOutageAlarm()
{
    handlePowerOutageAlarm();
}

//
void
ControllerInterface::Step(const float timeStep)
{
    this->mandatoryStep(timeStep);

    this->userStep(timeStep);
}

void 
ControllerInterface::mandatoryStep(const float timeStep)
{
    // Move the elevators
    for(auto elevator : elevators_)
    {
        std::cout << "updating: " << elevator->GetLabel() << std::endl;
        elevator->Update(timeStep);
    }
}
