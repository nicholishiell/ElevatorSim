#ifndef PERSON_H
#define PERSON_H

#include <QObject>

#include <iostream>

class Person
{
    Q_OBJECT

public:
    Person( const std::string label, 
            const int currentLevel,
            const int desiredLevel); 
            
    virtual ~Person(); 

    bool IsGoingUp() const {return desiredLevel_ - currentLevel_ > 0;}

    int GetDesiredLevel() const {return desiredLevel_;}
    int GetCurrentLevel() const {return currentLevel_;}

    
private:

    std::string name_;
    int currentLevel_;
    int desiredLevel_;
};

#endif