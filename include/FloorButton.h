#ifndef FLOOR_BUTTON_H
#define  FLOOR_BUTTON_H

#include <QPushButton>

class FloorButton : public QPushButton
{
    Q_OBJECT

public:

    FloorButton(const int level, 
                const QString & text, 
                QWidget * parent = 0) : QPushButton(text, parent)
    {
        level_ = level;
    }

    int GetLevel() const {return level_;}

private:

    int level_ = -1;

};

#endif