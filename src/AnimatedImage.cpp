#include "include/AnimatedImage.h"

#include <QImage>

#include <iostream>

AnimatedImage::AnimatedImage(   const QString& image_base, 
                                const int imageCount, 
                                const float resize, 
                                QWidget *parent) : QLabel(parent)
{
    currentPixmap = 0;

    for (int i = 0; i < imageCount; i++)
    {
        // TODO: This is ugly code...clean it up
        auto filepath = image_base;
        filepath = filepath.append(QString::number(i));
        filepath = filepath.append(QString(".png"));
        QImage subImage(filepath);

        pixmaps.push_back(QPixmap::fromImage(subImage.scaled(resize*subImage.width(), resize*subImage.height())));
    }
 
    connect(&timer, SIGNAL(timeout()), SLOT(changeImage()));
    
    setPixmap(pixmaps.at(0));
}

void 
AnimatedImage::Animate()
{
    currentPixmap = 0;
    timer.start(100);
}

void 
AnimatedImage::changeImage()
{
    if (currentPixmap < pixmaps.length())
    {
        setPixmap(pixmaps.at(currentPixmap));
        currentPixmap++;
    }
    else
    {
        timer.stop();
        setPixmap(pixmaps.at(0));
    }
}