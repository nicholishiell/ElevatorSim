#include "include/GraphicsWindow.h"

GraphicsWindow::GraphicsWindow(QWidget *parent)  : QMainWindow(parent)
{
    this->resize(800,600);

    auto graphcFrame = new QFrame;
    graphcFrame->setLayout(new QVBoxLayout);
    this->setCentralWidget(graphcFrame);

    scene_ = new QGraphicsScene();
    scene_->setSceneRect(0, 0, 800, 600);
    scene_->setItemIndexMethod(QGraphicsScene::NoIndex);

    viewer_ = new QGraphicsView(scene_);    
    viewer_->setCacheMode(QGraphicsView::CacheBackground);
    viewer_->setDragMode(QGraphicsView::ScrollHandDrag);
    viewer_->setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Elevator Simulator"));
    viewer_->setRenderHint(QPainter::Antialiasing);
    
    viewer_->setScene(scene_); 
    graphcFrame->layout()->addWidget(viewer_);
   
    playButton_ = new QPushButton("Play", this);
    playButton_->setGeometry(0, 0, 80, 30);
    
    stopButton_ = new QPushButton("Stop", this);
    stopButton_->setGeometry(80, 0, 80, 30);
    
    stepButton_ = new QPushButton("Step", this);
    stepButton_->setGeometry(160, 0, 80, 30);

    graphcFrame->layout()->addWidget(playButton_);
    graphcFrame->layout()->addWidget(stopButton_);
    graphcFrame->layout()->addWidget(stepButton_);

    aniTimer_ = new QTimer();   
    simTimer_ = new QTimer();   
   
    QObject::connect(playButton_, &QPushButton::clicked, this, &GraphicsWindow::PlaySimulation);
    QObject::connect(stopButton_, &QPushButton::clicked, this, &GraphicsWindow::StopSimulation);
    QObject::connect(stepButton_, &QPushButton::clicked, this, &GraphicsWindow::UpdateSimulationOneStep);
    QObject::connect(simTimer_, &QTimer::timeout, this, &GraphicsWindow::UpdateSimulationOneStep);
    
    aniTimer_->start(timeStepMilliseconds);
    QObject::connect(aniTimer_, &QTimer::timeout, scene_, &QGraphicsScene::advance);
}

void 
GraphicsWindow::AddController(ControllerInterface* controller)
{
    graphicalController_ = new GraphicalController(controller);
    scene_->addItem(graphicalController_);

    controller_ = controller;
}

GraphicsWindow::~GraphicsWindow()
{

}

void 
GraphicsWindow::UpdateSimulationOneStep()
{
    controller_->Step(timeStepSeconds);
    graphicalController_->update();
}


void 
GraphicsWindow::PlaySimulation()
{
    simTimer_->start(timeStepMilliseconds);
}

void 
GraphicsWindow::StopSimulation()
{
    simTimer_->stop();
}