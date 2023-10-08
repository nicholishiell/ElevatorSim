#include "include/Person.h" 
 
 
Person::Person( const std::string label, 
                const int currentLevel,
                const int desiredLevel)
{
    name_ = label;
    currentLevel_ = currentLevel;
    desiredLevel_ = desiredLevel;
}
                
Person::~Person()
{

}