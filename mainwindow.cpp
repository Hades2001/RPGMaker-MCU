#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    elvestool = new ElvesResources;
    mappainter = new MapPainter;
    ui->sw_tools->addWidget(elvestool);
    ui->sw_tools->addWidget(mappainter);
    ui->sw_tools->setCurrentIndex(1);

    connect( elvestool, &ElvesResources::widgetResize,[=](int w,int h){
        ui->sw_tools->resize(w,h);
        w = ( w <= -1 ) ? this->width() : w + 200;
        h = ( h <= -1 ) ? this->height() : h + 20;
        this->resize(w,h);
        //ui->sw_tools->move(200,10);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
