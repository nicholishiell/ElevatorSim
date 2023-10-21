#include "include/Person.h" 
 
 
Person::Person( const std::string label, 
                const int currentLevel,
                const int desiredLevel,
                const float weight)
{
    name_ = label;
    currentLevel_ = currentLevel;
    desiredLevel_ = desiredLevel;
    weight_ = weight;
}
                
Person::~Person()
{

}