#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <string>
#include <memory>
#include <sstream>  

///////////////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS
///////////////////////////////////////////////////////////////////////////////////////

class Elevator;
class ElevatorPanel;
class ElevatorState;

class Floor;
class FloorPanel;

class BuildingPanel;

class Controller;

class Person;

///////////////////////////////////////////////////////////////////////////////////////
// ENUMs
///////////////////////////////////////////////////////////////////////////////////////

enum DoorState{OPEN, CLOSED};

enum LightState{ON,OFF, UNKNOWN};

enum RequestDirection{REQ_UP, REQ_DOWN, REQ_IDLE};

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

using FloorSharedPtr = std::shared_ptr<Floor>;
using FloorSharedPtrVector = std::vector<FloorSharedPtr>;

using FloorPanelSharedPtr = std::shared_ptr<FloorPanel>;
using FloorPanelSharedPtrVector = std::vector<FloorPanelSharedPtr>;

using BuildingPanelSharedPtr = std::shared_ptr<BuildingPanel>;

using ControllerSharedPtr = std::shared_ptr<Controller>;

using PersonVector = std::vector<Person>;

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

const std::string FLOOR_SIGN_FILE_NAMES[6] = {  "Floor_01.png",
                                                "Floor_02.png",
                                                "Floor_03.png",
                                                "Floor_04.png",
                                                "Floor_05.png",
                                                "Floor_06.png"};

const float FLOOR_HEIGHT_METERS = 2.;
const float ELEVATOR_SPEED = 0.5;
const float ELEVATOR_WAIT_TIME = 3.;
const float ELEVATOR_MAX_WEIGHT = 500.;

const int MAX_NUMBER_OF_FLOORS = 6;
const int MAX_NUMBER_OF_ELEVATORS = 4;

#endif