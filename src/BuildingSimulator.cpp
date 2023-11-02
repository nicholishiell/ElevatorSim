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
        auto panel = std::make_shared<ElevatorPanel>(   elevator->GetLabel(),
                                                        this->GetNumberOfFloors());
                                                        
        QObject::connect(panel.get(), &ElevatorPanel::ServiceRequested, this, &BuildingSimulator::HandleServiceRequest);
        QObject::connect(panel.get(), &ElevatorPanel::EmergencyRequested, this, &BuildingSimulator::HandleEmergencyRequest);

        elevator->SetPanel(panel);
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
    else if(request.type == EmergencyType::HELP)
    {
        controller_->HandleHelpRequest(request.level);
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

    this->updatePeople(timeStep);

    // Let the user defined controller do any periodic work it needs to do
    controller_->Step(timeStep, elevators_, floors_);

    // Notify the graphics generator that things have changed.
    if(graphicsObserver_ != nullptr)
        graphicsObserver_->update();
}

void 
BuildingSimulator::updateElevators(const float timeStep)
{
    // Move the elevators
    for(auto elevator : elevators_)
    {
        std::cout << "updating: " << elevator->GetLabel() << std::endl;
        elevator->Update(timeStep);
    }
}

void 
BuildingSimulator::updateFloors(const float timeStep)
{
    for(auto floor : floors_)
        floor->GetPanel()->LightsOut();

    for(unsigned int i = 0; i < elevators_.size(); i++)
    {
        auto panel = elevators_[i]->GetPanel();
        auto level = panel->GetLevel();
        auto currentRequest = panel->GetCurrentlyServicing();

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

void 
BuildingSimulator::updatePeople(const float timeStep)
{

}
