#include "simpleexample.h"

bool SimpleExample::init(DIEPLUGIN *pDiePlugin)
{
    this->pDiePlugin=pDiePlugin;


    // Create GUI
    button=new QPushButton;
    button->setText("Test");

    connect(button, SIGNAL(clicked()),this, SLOT(process()));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button);
    pDiePlugin->pWidget->setLayout(layout);


    return true;
}

QString SimpleExample::getName()
{
    // return name of the plugin for ListBox
    return QString("Simple plugin");
}

void SimpleExample::process()
{
    QMessageBox::information(pDiePlugin->pWidget,"Hello","I'm a simple plugin");
}

Q_EXPORT_PLUGIN2(simpleexample, SimpleExample)
