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

#include "include/BuildingSimulator.h"
#include "include/GraphicsGenerator.h"

class GraphicsWindow  : public QMainWindow
{

    Q_OBJECT

public:
    GraphicsWindow(BuildingSimulator* buildingSim);
    virtual ~GraphicsWindow(); 
    
public slots:

    void PlaySimulation();
    void UpdateSimulationOneStep();
    void StopSimulation();
    void AddFloorPressed();
    void AddElevatorPressed();
    void InitializedPressed();

private:

    void updateScene();
    
    QGraphicsScene * scene_;
    QGraphicsView * viewer_;
    QTimer* simTimer_;
    QTimer* aniTimer_;

    QPushButton* playButton_;
    QPushButton* stopButton_;
    QPushButton* stepButton_;

    QPushButton* addFloorButton_;
    QPushButton* addElevatorButton_;
    QPushButton* initializeButton_;

    QPushButton* exitButton_;

    GraphicsGenerator* graphicsGenerator_;
    BuildingSimulator* buildingSimulator_;

    int timeStepMilliseconds = 30;
    float timeStepSeconds = 0.03;
};

#endif