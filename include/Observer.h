#ifndef OBSERVER_H
#define OBSERVER_H

class Observer
{
public:
    Observer(){};

    virtual void Notify() = 0;
};

#endif