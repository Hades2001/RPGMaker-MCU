#ifndef MAPPAINTER_H
#define MAPPAINTER_H

#include <QWidget>
#include <QMouseEvent>
#include <QFileDialog>
#include <QScrollArea>
#include "mappainterwidget.h"
#include "maplibwidget.h"

#include <sysdef.h>

namespace Ui {
class MapPainter;
}

class MapPainter : public QWidget
{
    Q_OBJECT
public:
    explicit MapPainter(QWidget *parent = nullptr);
    ~MapPainter();
    mappack_t* creatMap(QString,int,int);
    int deletemap(mappack_t*);

private slots:
    void on_Bn_Generated_pressed();

private:
    maplibwidget *_maplib[128];
    QScrollArea *_maplab[128];
    QStringList mapimage = {
        "Dungeon_A1.png",//1
        "Dungeon_A2.png",//2
        "Dungeon_A4.png",//3
        "Dungeon_A5.png",//4
        "Dungeon_B.png",//5
        "Dungeon_C.png",//6
        "Inside_A1.png",//7
        "Inside_A2.png",//8
        "Inside_A4.png",//9
        "Inside_A5.png",//10
        "Inside_B.png",//11
        "Inside_C.png",//12
        "Outside_A1.png",//13
        "Outside_A2.png",//14
        "Outside_A3.png",//15
        "Outside_A4.png",//16
        "Outside_A5.png",//17
        "Outside_B.png",//18
        "Outside_C.png",//19
        "SF_Inside_A4.png",//20
        "SF_Inside_B.png",//21
        "SF_Inside_C.png",//22
        "SF_Outside_A3.png",//23
        "SF_Outside_A4.png",//24
        "SF_Outside_A5.png",//25
        "SF_Outside_B.png",//26
        "SF_Outside_C.png",//27
        "World_A1.png",//28
        "World_A2.png",//29
        "World_B.png",//30
        "World_C.png",//31
    };

private:
    Ui::MapPainter *ui;

private:
    mappack_t *_mapptr = nullptr;

};

#endif // MAPPAINTER_H
