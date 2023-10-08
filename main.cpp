#include <QApplication>

#include "include/GraphicsWindow.h"
#include "include/MyController.h"
#include "include/Utility.h"

int main(int argc, char *argv[]) 
{   
  QApplication app(argc, argv);  
  
  MyController aController;

  aController.AddFloor("F1");
  aController.AddFloor("F2");
  aController.AddFloor("F3");
  aController.AddFloor("F4");

  aController.AddElevator("EV1",0,DoorState::OPEN);

  aController.AddElevator("EV2",0,DoorState::CLOSED);

  GraphicsWindow aWindow;
  aWindow.AddController(&aController);
  aWindow.show();

  app.exec();

  return 0;
}
