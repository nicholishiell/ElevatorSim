#include "include/GraphicsWindow.h"

GraphicsWindow::GraphicsWindow(BuildingSimulator* buildingSim)  : QMainWindow(nullptr)
{
    buildingSimulator_ = buildingSim;
    graphicsGenerator_ = new GraphicsGenerator(buildingSimulator_);

    scene_ = new QGraphicsScene();
    scene_->setSceneRect(0, 0, 800, 600);
    scene_->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene_->addItem(graphicsGenerator_);

    viewer_ = new QGraphicsView(scene_);    
    viewer_->setCacheMode(QGraphicsView::CacheBackground);
    viewer_->setDragMode(QGraphicsView::ScrollHandDrag);
    viewer_->setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Elevator Simulator"));
    viewer_->setRenderHint(QPainter::Antialiasing); 
    viewer_->setScene(scene_); 
      
    playButton_ = new QPushButton("Play", this);
    playButton_->setGeometry(0, 0, 80, 30);
    playButton_->setEnabled(false);

    stopButton_ = new QPushButton("Stop", this);
    stopButton_->setGeometry(80, 0, 80, 30);
    stopButton_->setEnabled(false);

    stepButton_ = new QPushButton("Step", this);
    stepButton_->setGeometry(160, 0, 80, 30);
    stepButton_->setEnabled(false);

    addFloorButton_= new QPushButton("Add Floor (0)", this);
    addElevatorButton_= new QPushButton("Add Elevator (0)", this);
    initializeButton_= new QPushButton("Initialize", this);

    exitButton_ = new QPushButton("Exit", this);

    auto graphicFrame = new QFrame;   
    graphicFrame->setLayout(new QVBoxLayout);
    graphicFrame->layout()->addWidget(viewer_);
    graphicFrame->layout()->addWidget(playButton_);
    graphicFrame->layout()->addWidget(stopButton_);
    graphicFrame->layout()->addWidget(stepButton_);
    graphicFrame->layout()->addWidget(addFloorButton_);
    graphicFrame->layout()->addWidget(addElevatorButton_);
    graphicFrame->layout()->addWidget(initializeButton_);
    graphicFrame->layout()->addWidget(exitButton_);

    this->setCentralWidget(graphicFrame);

    QObject::connect(playButton_, &QPushButton::clicked, this, &GraphicsWindow::PlaySimulation);
    QObject::connect(stopButton_, &QPushButton::clicked, this, &GraphicsWindow::StopSimulation);
    QObject::connect(stepButton_, &QPushButton::clicked, this, &GraphicsWindow::UpdateSimulationOneStep);
   
    QObject::connect(addFloorButton_, &QPushButton::clicked, this, &GraphicsWindow::AddFloorPressed);
    QObject::connect(addElevatorButton_, &QPushButton::clicked, this, &GraphicsWindow::AddElevatorPressed);
    QObject::connect(initializeButton_, &QPushButton::clicked, this, &GraphicsWindow::InitializedPressed);

    QObject::connect(exitButton_, &QPushButton::clicked, this, &GraphicsWindow::close);

    aniTimer_ = new QTimer();   
    simTimer_ = new QTimer();   
    aniTimer_->start(timeStepMilliseconds);
    QObject::connect(simTimer_, &QTimer::timeout, this, &GraphicsWindow::UpdateSimulationOneStep);   
    QObject::connect(aniTimer_, &QTimer::timeout, scene_, &QGraphicsScene::advance);
}


GraphicsWindow::~GraphicsWindow()
{

}

void 
GraphicsWindow::UpdateSimulationOneStep()
{
    buildingSimulator_->Update(timeStepSeconds);
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

void 
GraphicsWindow::AddFloorPressed()
{
    auto num = buildingSimulator_->GetNumberOfFloors();
    std::string label = "F" + std::to_string(num);
    buildingSimulator_->AddFloor(label);

    
    addFloorButton_->setText(QString::fromStdString("Add Floor (" + std::to_string(num+1) + ")"));
}

void 
GraphicsWindow::AddElevatorPressed()
{
    auto num = buildingSimulator_->GetNumberOfElevators();
    std::string label = "EV" + std::to_string(num);
    buildingSimulator_->AddElevator(label,DoorState::CLOSED, 0.);

    addElevatorButton_->setText(QString::fromStdString("Add Elevator (" + std::to_string(num+1) + ")"));
}

void 
GraphicsWindow::InitializedPressed()
{
    addFloorButton_->setEnabled(false);
    addElevatorButton_->setEnabled(false);
    initializeButton_->setEnabled(false);

    playButton_->setEnabled(true);
    stopButton_->setEnabled(true);
    stepButton_->setEnabled(true);

    buildingSimulator_->Initialize();
}