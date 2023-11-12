#ifndef PERSON_H
#define PERSON_H

#include <QObject>

#include <iostream>

class Person
{

public:
    Person( const std::string name, 
            const int currentLevel,
            const int desiredLevel,
            const float weight); 
            
    virtual ~Person(); 

    int GetDestinationFloor() const {return destinationFloor_;}
    int GetCurrentFloor() const {return currentFloor_;}

    std::string GetName() const {return name_;}
    float GetWeight() const {return weight_;}

    std::string GetLabel() const;
    
    bool IsGoingUp() const {return currentFloor_ < destinationFloor_;}
    bool IsGoingDown() const {return currentFloor_ > destinationFloor_;}

    void Arrived() {currentFloor_ = destinationFloor_;}

private:

    std::string name_;
    int currentFloor_;
    int destinationFloor_;

    float weight_;
};

#endif