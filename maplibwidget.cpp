#include "maplibwidget.h"

maplibwidget::maplibwidget(QWidget *parent) : QWidget(parent)
{
    this->setMouseTracking(true);
    //_ChooseImage = new QImage(_ChooseRect.width(),_ChooseRect.height(),_mapImage.format());
}

maplibwidget::maplibwidget(QWidget *parent,QString path) : QWidget(parent)
{
    this->setMouseTracking(true);
    _mapImage.load(path);
    this->resize(_mapImage.width(),_mapImage.height());
    this->setMinimumSize(_mapImage.width(),_mapImage.height());
    //_ChooseImage = new QImage(_ChooseRect.width(),_ChooseRect.height(),_mapImage.format());
}

maplibwidget::maplibwidget(QWidget *parent,int mapnumber,QString path) : QWidget(parent)
{
    this->setMouseTracking(true);
    _mapImage.load(path);
    this->resize(_mapImage.width(),_mapImage.height());
    this->setMinimumSize(_mapImage.width(),_mapImage.height());
    _map_number = mapnumber;
}

maplibwidget::~maplibwidget()
{

}

void maplibwidget::setmapImageFilePath(QString path)
{
    _mapImage.load(path);
    this->resize(_mapImage.width(),_mapImage.height());
    this->setMinimumSize(_mapImage.width(),_mapImage.height());
}

void maplibwidget::mousePressEvent(QMouseEvent* e)
{
    Q_UNUSED(e);
    if( e->button() == Qt::LeftButton )
    {
        qDebug()<<"Press";
        _pressFlag = true;
        _CHooseFlag = false;
        _mousePressPoint = e->pos();
    }
    if( e->button() == Qt::RightButton )
    {

    }
    this->update();
}

void maplibwidget::mouseMoveEvent(QMouseEvent* e)
{
    Q_UNUSED(e);
    _mousePoint = e->pos();
    this->update();
}

void maplibwidget::mouseReleaseEvent(QMouseEvent* e)
{
    Q_UNUSED(e);
    if( e->button() == Qt::LeftButton )
    {
        _pressFlag = false;
        _CHooseFlag = true;

        _ChooseImage = QImage(qAbs(_ChooseRect.width()),qAbs(_ChooseRect.height()),_mapImage.format());
        //_ChooseImage = _mapImage.copy(_ChooseRect);
        //emit ChooseImage(_ChooseImage,_ChooseRect);
        emit ChooseRect(_map_number,_ChooseRect);
        /*
        qDebug()<<__FILE__<<__LINE__;
        if( _ChooseImage != nullptr) delete _ChooseImage;
        qDebug()<<__FILE__<<__LINE__;
        _ChooseImage = new QImage(_ChooseRect.width(),_ChooseRect.height(),_mapImage.format());
        qDebug()<<__FILE__<<__LINE__;
        *_ChooseImage = _mapImage.copy(_ChooseRect);
        qDebug()<<__FILE__<<__LINE__;
        emit ChooseImage(*_ChooseImage,_ChooseRect);
        */
    }
    if( e->button() == Qt::RightButton )
    {

    }
}

void maplibwidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawImage(QPoint(0,0),_mapImage);
    QPen painterPen(QColor(0,0,128),2);
    painter.setPen(painterPen);
    if( _pressFlag )
    {
        painterPen.setColor(QColor(200,0,0));
        painter.setPen(painterPen);

        int rect_sx = 0,rect_sy = 0,rect_w = 0,rect_h = 0;
        if( _mousePoint.x() >= _mousePressPoint.x())
        {
            rect_sx = _mousePressPoint.x() - _mousePressPoint.x() % 24;
            rect_w = _mousePoint.x() + ( 24 - _mousePoint.x() % 24 ) - rect_sx;
        }
        else
        {
            rect_sx = _mousePressPoint.x() + ( 24 - _mousePressPoint.x() % 24 );
            rect_w = _mousePoint.x() - _mousePoint.x() % 24 - rect_sx;
        }

        if( _mousePoint.y() >= _mousePressPoint.y())
        {
            rect_sy = _mousePressPoint.y() - _mousePressPoint.y() % 24;
            rect_h = _mousePoint.y() + ( 24 - _mousePoint.y() % 24 ) - rect_sy;
        }
        else
        {
            rect_sy = _mousePressPoint.y() + ( 24 - _mousePressPoint.y() % 24 );
            rect_h = _mousePoint.y() - _mousePoint.y() % 24 - rect_sy;
        }

        rect_w = ( rect_w == 0 ) ? 1: rect_w;
        rect_h = ( rect_h == 0 ) ? 1: rect_h;

        //_ChooseRect.setRect(rect_sx,rect_sy,rect_w,rect_h);
        if( rect_w < 0 )
        {
            rect_sx = rect_sx + rect_w;
            rect_w = -rect_w;
        }
        if( rect_h < 0 )
        {
            rect_sy = rect_sy + rect_h;
            rect_h = -rect_h;
        }
        _ChooseRect.setRect(rect_sx,rect_sy,rect_w,rect_h);
        painter.drawRect(rect_sx, rect_sy, rect_w, rect_h);
    }
    else
    {
        painterPen.setColor(QColor(0,200,0));
        painter.setPen(painterPen);

        int rectx0 = _mousePoint.x()  - _mousePoint.x() % 24;
        int recty0 = _mousePoint.y()  - _mousePoint.y() % 24;
        painter.drawRect(rectx0, recty0, 24, 24);
    }

    if( _CHooseFlag )
    {
        painterPen.setColor(QColor(200,0,0));
        painter.setPen(painterPen);

        painter.drawRect(_ChooseRect);
    }
    painter.drawText(QRect(0,0,100,20),QString("%1,%2").arg(_mousePoint.x()).arg(_mousePoint.y()));
}
