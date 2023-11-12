#ifndef FLOOR_H
#define FLOOR_H

#include <QObject>
#include <QWidget>

#include <iostream>

#include "include/FloorPanel.h"
#include "include/Person.h"

class Floor : public QWidget
{
    Q_OBJECT

public:
    Floor(  const std::string label, 
            const int level); 
            
    virtual ~Floor(); 

    void SetPanel(FloorPanelSharedPtr panel){panel_ = panel;}

    int GetLevel() const {return level_;}
    std::string GetLabel() const {return label_;}

    void Update(ElevatorSharedPtrVector elevators);

    void UpRequestServiced();
    void DownRequestServiced();
    
    FloorPanelSharedPtr GetPanel() const {return panel_;}
    
    void AddPerson(PersonSharedPtr p){peopleOnFloor_.emplace_back(p);}
    void RemovePerson(PersonSharedPtr p);
    PersonSharedPtrVector GetPeople() const {return peopleOnFloor_;}
    int GetNumberOfPeople() {return peopleOnFloor_.size();}
    PersonSharedPtr GetPerson(const int i) const {return peopleOnFloor_[i];}

private:

    void updateLights(ElevatorPanelSharedPtr ep, const int i);

    int level_;
    std::string label_;

    FloorPanelSharedPtr panel_;

    PersonSharedPtrVector peopleOnFloor_;
};

#endif