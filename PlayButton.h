#include <QApplication>
#include <QWidget>
#include <QPushButton>

class PlayButton : public QWidget
{
    
 public:
     PlayButton(QWidget *parent = 0);
};

PlayButton::PlayButton(QWidget *parent)
    : QWidget(parent) 
{
           
}