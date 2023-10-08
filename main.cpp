#include <QApplication>

#include "include/GraphicsWindow.h"
#include "include/BuildingSimulator.h"
#include "include/MyController.h"
#include "include/Utility.h"

int main(int argc, char *argv[]) 
{   
  QApplication app(argc, argv);  

  auto buildingSim = new BuildingSimulator(std::make_shared<MyController>());

  buildingSim->AddFloor("F1");
  buildingSim->AddFloor("F2");
  buildingSim->AddFloor("F3");
  buildingSim->AddFloor("F4");

  buildingSim->AddElevator("EV1",0,DoorState::OPEN);
  buildingSim->AddElevator("EV2",0,DoorState::CLOSED);

  GraphicsWindow aWindow(buildingSim);

  aWindow.show();

  app.exec();

  return 0;
}
