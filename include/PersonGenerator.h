#ifndef PERSON_GEN_h
#define PERSON_GEN_h

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QSlider>
#include <QLineEdit>

#include <iostream>
#include <string>

#include "include/Utility.h"
#include "include/Floor.h"

class PersonGenerator : public QWidget
{
    Q_OBJECT

public:
    PersonGenerator(FloorSharedPtrVector floors,
                    QWidget *parent = nullptr); 

    virtual ~PersonGenerator();   

private slots:

    void generatePerson();

private:

    QComboBox* initialFloorSelector_;
    QComboBox* destinationFloorSelector_;
    QSlider* weightSelector_;
    QLineEdit* nameField_;

    FloorSharedPtrVector floors_;
};

#endif