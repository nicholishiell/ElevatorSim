#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <string>
#include <memory>
#include <sstream>  
#include <chrono>

///////////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS
///////////////////////////////////////////////////////////////////////////////////////

class Elevator;
class ElevatorPanel;
class ElevatorState;
class ElevatorPositionSensor;

class Floor;
class FloorPanel;

class BuildingPanel;

class Controller;

class Person;

class GraphicsGenerator;

///////////////////////////////////////////////////////////////////////////////////////
// ENUMs
///////////////////////////////////////////////////////////////////////////////////////

enum DoorState{OPEN, CLOSED, OBSTRUCTED};

enum LightState{ON,OFF, UNKNOWN};

enum RequestDirection{REQ_UP, REQ_DOWN, REQ_IDLE};

enum EmergencyType{FIRE, POWER_OUTAGE, BLANK};

///////////////////////////////////////////////////////////////////////////////////////
// STRUCTs
///////////////////////////////////////////////////////////////////////////////////////

struct ServiceRequest 
{
    ServiceRequest()
    {
        level = -1;
        direction = RequestDirection::REQ_IDLE;
    }
     ServiceRequest(const int l, const RequestDirection d)
    {
        level = l;
        direction = d;
    }

    bool operator<(const ServiceRequest& a) const
    {
        return level < a.level;
    }

    int level;
    RequestDirection direction;
};

struct special_compare : public std::unary_function<ServiceRequest, bool>
{
  explicit special_compare(const ServiceRequest& baseline) : baseline(baseline) {}
  bool operator() (const ServiceRequest &arg)
  { return arg.level == baseline.level && arg.direction == baseline.direction; }
  ServiceRequest baseline;
};

struct EmergencyRequest 
{
    EmergencyRequest()
    {
        level = -1;
        type = EmergencyType::BLANK;
    }

    EmergencyRequest(const int l, const EmergencyType t)
    {
        level = l;
        type = t;
    }

    int level;
    EmergencyType type;
};

struct FloorPosition
{
    FloorPosition()
    {
        prevFloor = 0;
        nextFloor = 0;
    }

    FloorPosition(const int p, const int n)
    {
        prevFloor = p;
        nextFloor = n;
    }
    
    int prevFloor;
    int nextFloor;
};

///////////////////////////////////////////////////////////////////////////////////////
// ALIAS'
///////////////////////////////////////////////////////////////////////////////////////
using LightStateVector = std::vector<LightState>;
using ServiceRequestVector = std::vector<ServiceRequest>;

using ElevatorSharedPtr = std::shared_ptr<Elevator>;
using ElevatorSharedPtrVector = std::vector<ElevatorSharedPtr>;

using ElevatorPanelSharedPtr = std::shared_ptr<ElevatorPanel>;
using ElevatorPanelSharedPtrVector = std::vector<ElevatorPanelSharedPtr>;

using ElevatorStateUniquePtr = std::unique_ptr<ElevatorState>;

using ElevatorPositionSensorSharedPtr = std::shared_ptr<ElevatorPositionSensor>;

using FloorSharedPtr = std::shared_ptr<Floor>;
using FloorSharedPtrVector = std::vector<FloorSharedPtr>;

using FloorPanelSharedPtr = std::shared_ptr<FloorPanel>;
using FloorPanelSharedPtrVector = std::vector<FloorPanelSharedPtr>;

using BuildingPanelSharedPtr = std::shared_ptr<BuildingPanel>;

using ControllerSharedPtr = std::shared_ptr<Controller>;

using PersonSharedPtr = std::shared_ptr<Person>;
using PersonSharedPtrVector = std::vector<PersonSharedPtr>;

using GraphicsGeneratorSharedPtr = std::shared_ptr<GraphicsGenerator>;

using TimeStamp = std::chrono::time_point<std::chrono::system_clock>;

///////////////////////////////////////////////////////////////////////////////////////
// CONST
///////////////////////////////////////////////////////////////////////////////////////

const int FLOOR_WIDTH = 1654;
const int FLOOR_HEIGHT = 426;

const int SIGN_WIDTH = 221;
const int SIGN_HEIGHT = 82;

const int ELEVATOR_LIGHT_WIDTH = 81;
const int ELEVATOR_LIGHT_HEIGHT = 34;

const int ELEVATOR_WIDTH = 215;
const int ELEVATOR_HEIGHT = 265;

const int ELEVATOR_SPACING = 300;

const int PERSON_WIDTH = 200;
const int PERSON_HEIGHT = 300;

const std::string FLOOR_SIGN_FILE_NAMES[7] = {  "Floor_00.png",
                                                "Floor_01.png",
                                                "Floor_02.png",
                                                "Floor_03.png",
                                                "Floor_04.png",
                                                "Floor_05.png",
                                                "Floor_06.png"};

const float FLOOR_HEIGHT_METERS = 2.;
const float ELEVATOR_SPEED = 0.5;
const float ELEVATOR_WAIT_TIME = 3.;
const float ELEVATOR_MAX_WEIGHT = 200.;

const int MAX_NUMBER_OF_FLOORS = 6;
const int MAX_NUMBER_OF_ELEVATORS = 4;

const int64_t CALL_TIME_OUT  = 5;

#endif