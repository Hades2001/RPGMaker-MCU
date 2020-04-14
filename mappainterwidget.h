#ifndef MAPPAINTERWIDGET_H
#define MAPPAINTERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QtMath>
#include <QDebug>
#include <sysdef.h>

class mapPainterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit mapPainterWidget(QWidget *parent = nullptr,mappack_t* map = nullptr);
    ~mapPainterWidget();
    void setMapImageList(QStringList list){_mapList = list;}
    void setPainterImage(QImage,QRect);
    void setPainterImage(int,QRect);
    QImage reSetImageOffset(int offsetX,int offsetY);

    void setmapLayer(int);
    void writemapData(int posx, int posy, int offsetx, int offsety);

private:
    mappack_t *_map;
    QPixmap pixmap[3];
    QPoint mousePos;
    QRect _Rect,_DrawRect;
    QImage _Image;
    QImage _mapImage;
    bool _isPress = false;
    QPoint _drawPoint = QPoint(0,0);
    QPoint _pressPoint = QPoint(0,0);
    QStringList _mapList;
    int _mapLayer = 0;

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);

    void paintEvent(QPaintEvent *event);

signals:
    void MouseMove(QPoint);

public slots:
};

#endif // MAPPAINTERWIDGET_H
