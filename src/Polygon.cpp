#include "include/Polygon.h"

Polygon::Polygon(const float x, const float y, const float h, const float w)
{
    x_= x;
    y_= y;
    h_= h;
    w_= w;
}

QRectF Polygon::boundingRect()  const 
{
    return QRectF(x_,y_,h_,w_);
}

void 
Polygon::advance(int step)
{
    if (!step)
        return;  

    colourValue_ += 16;

    if(colourValue_ > 255)
        colourValue_ = 0;

    update();
}

void 
Polygon::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(QColor(colourValue_,colourValue_,colourValue_));
    painter->drawRect(x_,y_,h_,w_);
}

void 
Polygon::Up()
{
    setPos(mapToParent(0,-5));
}

void 
Polygon::Down()
{
    setPos(mapToParent(0,5));
}

void 
Polygon::Left()
{
    setPos(mapToParent(-5,0));
}

void 
Polygon::Right()
{
    setPos(mapToParent(5,0));
}

