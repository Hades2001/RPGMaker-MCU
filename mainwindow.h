#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "elvesresources.h"
#include "mappainter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:

private:
    ElvesResources  *elvestool;
    MapPainter      *mappainter;
};

#endif // MAINWINDOW_H
