#ifndef MAPLIBWIDGET_H
#define MAPLIBWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

#include <sysdef.h>

class maplibwidget : public QWidget
{
    Q_OBJECT
public:
    explicit maplibwidget(QWidget *parent = nullptr,QString path = "");
    explicit maplibwidget(QWidget *parent = nullptr,int mapnumber = -1,QString path = "");
    explicit maplibwidget(QWidget *parent = nullptr);

    ~maplibwidget();

    void setmapImageFilePath(QString);

    void setmapImageNumber(int i){ _map_number = i; }
    QImage chooseImage(){ return _ChooseImage; }
    QRect chooseRect(){ return _ChooseRect; }

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);

    void paintEvent(QPaintEvent *event);

private:
    int    _map_number =  -1;
    QImage _mapImage;
    QPoint _mousePoint;
    QPoint _mousePressPoint;
    bool   _pressFlag = false;
    bool   _ReleaseFlag = false;
    bool   _CHooseFlag = false;
    QRect  _ChooseRect;
    QImage _ChooseImage;

signals:
    void ChooseImage(QImage,QRect);
    void ChooseRect(int,QRect);

public slots:
};

#endif // MAPLIBWIDGET_H
