#include "include/Person.h" 
 
 
Person::Person( const std::string name, 
                const int currentLevel,
                const int desiredLevel,
                const float weight)
{
    name_ = name;
    currentFloor_ = currentLevel;
    destinationFloor_ = desiredLevel;
    weight_ = weight;
}
                
Person::~Person()
{

}

std::string 
Person::GetLabel() const
{
    return name_ + "\t" + std::to_string(destinationFloor_);
}