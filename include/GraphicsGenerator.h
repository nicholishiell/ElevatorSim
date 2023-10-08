#ifndef GRAPHICAL_CONTROLLER_H
#define GRAPHICAL_CONTROLLER_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QPixmap>

#include <iostream>
#include <string>

#include "include/BuildingSimulator.h"
#include "include/Utility.h"

class GraphicsGenerator : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    GraphicsGenerator(const BuildingSimulator* buildingSim); 
            
    virtual ~GraphicsGenerator();   

    QRectF boundingRect() const override;

    void paint( QPainter *painter, 
                const QStyleOptionGraphicsItem *option,
                QWidget *widget) override;

protected:
    void advance(int step) override;

private:

    const BuildingSimulator* buildingSim_;

    QPixmap drawFloor(const FloorSharedPtr floor);
    QPixmap drawElevatorOnFloor(const ElevatorSharedPtr elevator, 
                                QPixmap floorImage, 
                                const int floorIndex, 
                                const int elevatorIndex);
};

#endif