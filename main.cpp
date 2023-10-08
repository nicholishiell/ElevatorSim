#include <QApplication>

#include "include/MyGraphicsWindow.h"
#include "include/Controller.h"
#include "include/Utility.h"

int main(int argc, char *argv[]) 
{   
  QApplication app(argc, argv);  
  
  Controller aController;

  aController.AddFloor("F1");
  aController.AddFloor("F2");
  aController.AddFloor("F3");
  aController.AddFloor("F4");

  aController.AddElevator("EV1",0,DoorState::OPEN);

  aController.AddElevator("EV2",0,DoorState::CLOSED);

  MyGraphicsWindow aWindow;
  aWindow.AddController(&aController);
  aWindow.show();

  app.exec();

  return 0;
}
