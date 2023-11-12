#include <QApplication>

#include "include/GraphicsWindow.h"
#include "include/BuildingSimulator.h"
#include "include/MyController.h"
#include "include/Utility.h"

int main(int argc, char *argv[]) 
{   
  QApplication app(argc, argv);  

  auto buildingSim = new BuildingSimulator(std::make_shared<MyController>());

  GraphicsWindow aWindow(buildingSim);

  aWindow.show();

  app.exec();

  return 0;
}
