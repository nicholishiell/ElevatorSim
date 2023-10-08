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

    int GetLevel() const {return level_;}
    void SetNumElevators(const int numElevators);

    void UpRequestServiced();
    void DownRequestServiced();
    
    FloorPanelSharedPtr GetPanel() const {return panel_;}
    
private:

    int level_;
    std::string label_;

    FloorPanelSharedPtr panel_;
};

#endif