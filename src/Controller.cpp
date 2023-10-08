#include "include/Controller.h"


Controller::Controller()
{

}

Controller::~Controller()
{

} 

void 
Controller::AddServiceRequest(const ServiceRequest request)
{
    pendingRequests_.emplace_back(request);
}
