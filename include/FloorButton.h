#ifndef FLOOR_BUTTON_H
#define  FLOOR_BUTTON_H

#include <QPushButton>

#include "include/Utility.h"

class FloorButton : public QPushButton
{
    Q_OBJECT

public:

    FloorButton(const int level, 
                const QString & text, 
                QWidget * parent = 0) : QPushButton(text, parent)
    {
        level_ = level;

        QObject::connect(this, &QPushButton::pressed, this, &FloorButton::FloorButtonPressed);
    }

private slots:

    void FloorButtonPressed();

signals:

    void ServiceRequested(const int destinationFloor);

private:

    int level_ = -1;

};

#endif