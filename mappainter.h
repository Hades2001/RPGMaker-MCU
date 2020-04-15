#ifndef MAPPAINTER_H
#define MAPPAINTER_H

#include <QWidget>
#include <QMouseEvent>
#include <QFileDialog>
#include <QScrollArea>
#include <QButtonGroup>

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
        "Dungeon_A1.png",//0----group 0-0
        "Dungeon_A2.png",//1----group 0-1
        "Dungeon_A4.png",//2----group 0-2
        "Dungeon_A5.png",//3----group 0-3
        "Dungeon_B.png",//4 ----group 0-4
        "Dungeon_C.png",//5 ----group 0-5

        "Inside_A1.png",//6 ----group 1-0
        "Inside_A2.png",//7 ----group 1-1
        "Inside_A4.png",//8 ----group 1-2
        "Inside_A5.png",//9 ----group 1-3
        "Inside_B.png",//10 ----group 1-4
        "Inside_C.png",//11 ----group 1-5

        "Outside_A1.png",//12---group 2-0
        "Outside_A2.png",//13---group 2-1
        "Outside_A3.png",//14---group 2-2
        "Outside_A4.png",//15---group 2-3
        "Outside_A5.png",//16---group 2-4
        "Outside_B.png",//17 ---group 2-5
        "Outside_C.png",//18 ---group 2-6

        "SF_Inside_A4.png",//19 ---group 3-0
        "SF_Inside_B.png",//20  ---group 3-1
        "SF_Inside_C.png",//21  ---group 3-2

        "SF_Outside_A3.png",//22---group 4-0
        "SF_Outside_A4.png",//23---group 4-1
        "SF_Outside_A5.png",//24---group 4-2
        "SF_Outside_B.png",//25 ---group 4-3
        "SF_Outside_C.png",//26 ---group 4-4

        "World_A1.png",//27     ---group 5-0
        "World_A2.png",//28     ---group 5-1
        "World_B.png",//29      ---group 5-2
        "World_C.png",//30      ---group 5-3
    };

    int mapgroupbuff[6][8]={
        { 0 , 1 , 2 , 3 , 4 , 5 , -1, -1 },
        { 6 , 7 , 8 , 9 , 10, 11, -1, -1 },
        { 12, 13, 14, 15, 16, 17, 18, -1 },
        { 19, 20, 21, -1, -1, -1, -1, -1 },
        { 22, 23, 24, 25, 26, -1, -1, -1 },
        { 27, 28, 29, 30, -1, -1, -1, -1 },
    };

private:
    Ui::MapPainter *ui;

private:
    mappack_t *_mapptr = nullptr;

};

#endif // MAPPAINTER_H
