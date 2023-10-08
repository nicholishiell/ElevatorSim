#ifndef MY_GRAPHICS_H
#define MY_GRAPHICS_H

#include <QMainWindow>
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QTimer>
#include <QObject>
#include <QMenuBar>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QHBoxLayout>

#include <iostream>

#include "include/Controller.h"
#include "include/GraphicalController.h"

class MyGraphicsWindow  : public QMainWindow
{

    Q_OBJECT

public:
    MyGraphicsWindow(QWidget *parent = nullptr);
    virtual ~MyGraphicsWindow(); 
    
    void AddController(Controller* controller);

public slots:

    void PlaySimulation();
    void UpdateSimulationOneStep();
    void StopSimulation();

private:

    void updateScene();
    
    QGraphicsScene * scene_;
    QGraphicsView * viewer_;
    QTimer* simTimer_;
    QTimer* aniTimer_;

    QPushButton* playButton_;
    QPushButton* stopButton_;
    QPushButton* stepButton_;

    GraphicalController* graphicalController_;
    Controller* controller_;

    int timeStepMilliseconds = 30;
    float timeStepSeconds = 0.03;
};

#endif