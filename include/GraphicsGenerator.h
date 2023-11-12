#ifndef GRAPHICAL_CONTROLLER_H
#define GRAPHICAL_CONTROLLER_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QPixmap>

#include <iostream>
#include <string>

#include "include/Utility.h"

class BuildingSimulator;

class GraphicsGenerator : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    GraphicsGenerator(BuildingSimulator* buildingSim); 
            
    virtual ~GraphicsGenerator();   

    QRectF boundingRect() const override;

    void paint( QPainter *painter, 
                const QStyleOptionGraphicsItem *option,
                QWidget *widget) override;

protected:

    void advance(int step) override;

private:

    BuildingSimulator* buildingSim_;

    QPixmap drawFloor(const FloorSharedPtr floor);
    QPixmap drawElevatorOnFloor(const ElevatorSharedPtr elevator, 
                                QPixmap floorImage, 
                                const int floorIndex, 
                                const int elevatorIndex);

    QPixmap drawPerson( PersonSharedPtr person,
                        QPixmap floorImage, 
                        const int floorIndex, 
                        const int personIndex);
};

#endif