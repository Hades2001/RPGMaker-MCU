#include "mappainterwidget.h"

mapPainterWidget::mapPainterWidget(QWidget *parent,mappack_t* map) : QWidget(parent)
{
    this->setMouseTracking(true);
    if( map == nullptr ) return;
    _map = map;

    BkPix = QPixmap(map->size_w * 24,map->size_h * 24);
    BkPix.fill(Qt::white);
    QPainter painter(&BkPix);
    for( int posy = 0; posy < map->size_h * 2 ; posy++ )
    {
        for( int posx = 0; posx < map->size_w * 2 ; posx++ )
        {
            painter.setPen(QColor(Qt::transparent));
            painter.setBrush(QColor(200,200,200));
            if(( posx + ( posy % 2 )) % 2 == 0 )
            {
                painter.drawRect(posx*12,posy*12,12,12);
            }
        }
    }
    painter.end();

    LayerIcon.load(":/icon/img/layers_0000.png");
    LayerIcon = LayerIcon.scaled(64,64);

    pixmap[0] = QPixmap(map->size_w * 24,map->size_h * 24);
    pixmap[0].fill(Qt::transparent);
    pixmap[1] = QPixmap(map->size_w * 24,map->size_h * 24);
    pixmap[1].fill(Qt::transparent);
    pixmap[2] = QPixmap(map->size_w * 24,map->size_h * 24);
    pixmap[2].fill(Qt::transparent);
    this->resize(map->size_w * 24,map->size_h * 24);
    this->setMinimumSize(map->size_w * 24,map->size_h * 24);
    _Rect = QRect(0,0,24,24);
    _DrawRect = QRect(0,0,24,24);
}

mapPainterWidget::~mapPainterWidget()
{

}
void mapPainterWidget::setPainterImage(QImage image,QRect r)
{
    _Image = QImage(r.width(),r.height(),image.format());
    _Rect = QRect(r.x(),r.y(),qAbs(r.width()),qAbs(r.height()));
    _Image = image;
}

void mapPainterWidget::setPainterImage(int number,QRect r)
{
    _mapImage.load(_mapList.at(number));
    _mapnumber = number;
    _Rect = QRect(r.x(),r.y(),qAbs(r.width()),qAbs(r.height()));
    qDebug()<<_Rect;
    _Image = _mapImage.copy(_Rect);
}

void mapPainterWidget::setmapLayer(int layer)
{
    _mapLayer = layer;

    switch (_mapLayer) {
        case 0 : LayerIcon.load(":/icon/img/layers_0002.png");break;
        case 1 : LayerIcon.load(":/icon/img/layers_0001.png");break;
        case 2 : LayerIcon.load(":/icon/img/layers_0000.png");break;
        default: LayerIcon.load(":/icon/img/layers_0000.png");break;
    }
    LayerIcon = LayerIcon.scaled(64,64);
    this->update();
}

QImage mapPainterWidget::reSetImageOffset(int offsetX,int offsetY)
{

    QImage image(_Rect.width(),_Rect.height(),_Image.format());
    offsetX = ( offsetX < 0 ) ? ( _Rect.width() - qAbs(offsetX) % _Rect.width()) : offsetX % _Rect.width();
    offsetY = ( offsetY < 0 ) ? ( _Rect.height() - qAbs(offsetY) % _Rect.height()) : offsetY % _Rect.height();

    for( int y = 0; y < _Rect.height(); y++ )
    {
        for(int x = 0; x < _Rect.width(); x++ )
        {
            image.setPixelColor(x,y,_Image.pixelColor(( offsetX + x ) % _Rect.width(),
                                                 ( offsetY + y ) % _Rect.height()));
        }
    }
    return image;
}

void mapPainterWidget::writemapData(int posx, int posy, int offsetx, int offsety)
{
    uint16_t **layer = nullptr;

    if( _mapLayer == -1 ) return;

    switch (_mapLayer) {
        case 0: layer = _map->maplayer0;break;
        case 1: layer = _map->maplayer1;break;
        case 2: layer = _map->maplayer2;break;
        default:layer = _map->maplayer0;break;
    }

    int image_w = _mapImage.width() / 24;
    posx = posx / 24;
    posy = posy / 24;

    if(( _Rect.width() == 24 )&&( _Rect.height() == 24 ))
    {
        layer[posx][posy] = uint16_t(image_w * (_Rect.y() / 24) + (_Rect.x() / 24));
        return;
    }
    uint16_t **mapdata = nullptr;

    int pack_w = _Rect.width() / 24;
    int pack_h = _Rect.height() / 24;

    mapdata = new uint16_t*[pack_h];

    for (int y = 0; y < pack_h; y++ ) {
        mapdata[y] = new uint16_t[pack_w];

        for (int x = 0; x < pack_w; x++) {
            mapdata[y][x] = uint16_t(image_w * ( y + (_Rect.y() / 24)) + ( x + (_Rect.x() / 24)));
        }
    }

    offsetx = ( offsetx < 0 ) ? ( _Rect.width() - qAbs(offsetx) % _Rect.width()) / 24 : offsetx % _Rect.width() / 24;
    offsety = ( offsety < 0 ) ? ( _Rect.height() - qAbs(offsety) % _Rect.height()) / 24 : offsety % _Rect.height() / 24;

    for (int y = 0; y < pack_h; y++ ) {
        for (int x = 0; x < pack_w; x++) {
            if(( posy + y < 0 )||( posx + x < 0 )||( posx + x >= _map->size_w)||( posy + y >= _map->size_h)) continue;
            layer[posy + y][posx + x] = mapdata[( y + offsety ) % pack_h][( x + offsetx ) % pack_w];
            layer[posy + y][posx + x] |= 0x8000;
            layer[posy + y][posx + x] |= ( _mapnumber & 0x7 ) << 12;
        }
    }
    for (int y = 0; y < pack_h; y++ ) {
        delete [] mapdata[y];
    }

    delete [] mapdata;
}
void mapPainterWidget::mousePressEvent(QMouseEvent* e)
{
    Q_UNUSED(e);
    mousePos = e->pos();
    if( _mapLayer == -1 ) return;

    if( e->button() == Qt::LeftButton )
    {
        QPainter painter(&pixmap[_mapLayer]);
        int rectx0 = mousePos.x()  - mousePos.x() % 24;
        int recty0 = mousePos.y()  - mousePos.y() % 24;
        painter.drawImage(rectx0,recty0,_Image);
        painter.end();

        writemapData(rectx0,
                     recty0,
                     0,
                     0);

        _pressPoint = QPoint(rectx0,recty0);

        _drawPoint = mousePos;
        _isPress = true;
        this->update();
    }
    if( e->button() == Qt::RightButton )
    {
        pixmap[_mapLayer].fill(Qt::transparent);
        this->update();
    }
}

void mapPainterWidget::mouseMoveEvent(QMouseEvent* e)
{
    Q_UNUSED(e);
    mousePos = e->pos();
    if( _mapLayer == -1 ) return;

    if( _isPress )
    {
        if(( qAbs( mousePos.x() - _drawPoint.x()) >= 24 )||\
           ( qAbs( mousePos.y() - _drawPoint.y()) >= 24 ))
        {
            //qDebug()<<"Layer:"<<_mapLayer;
            QPainter painter(&pixmap[_mapLayer]);
            int rectx0 = mousePos.x()  - mousePos.x() % 24;
            int recty0 = mousePos.y()  - mousePos.y() % 24;



            painter.drawImage(rectx0,
                              recty0,
                              reSetImageOffset(rectx0 - _pressPoint.x(),
                                               recty0 - _pressPoint.y()));


            //qDebug()<<__FILE__<<__LINE__;
            writemapData(rectx0,
                         recty0,
                         rectx0 - _pressPoint.x(),
                         recty0 - _pressPoint.y());
            //qDebug()<<__FILE__<<__LINE__;

            _drawPoint = mousePos;
            //qDebug()<<__FILE__<<__LINE__;

            this->update();
        }
    }
    else
    {
        //mousePos.setX(( mousePos.x() < 0 ) ? 0 : mousePos.x());
        //mousePos.setY(( mousePos.y() < 0 ) ? 0 : mousePos.y());
        emit MouseMove(e->pos());
        this->update();
    }

}
void mapPainterWidget::mouseReleaseEvent(QMouseEvent* e)
{
    Q_UNUSED(e);
    if( _mapLayer == -1 ) return;

    if( e->button() == Qt::LeftButton )
    {
        _isPress = false;
    }
    if( e->button() == Qt::RightButton )
    {
        pixmap[_mapLayer].fill(Qt::transparent);
    }
}
void mapPainterWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    painter.drawPixmap(0,0,BkPix);

    for( int i = 0; i < 3; i++ )
    {
        if(( _mapLayer != i )&&( _mapLayer != -1 )) painter.setOpacity(0.5);
        painter.drawPixmap(0,0,pixmap[i]);
        painter.setOpacity(1);
    }

    if( _mapLayer != -1 )
    {
        QPen painterPen(QColor(0,0,128),2);
        painter.setPen(painterPen);

        int x0 = (( mousePos.x() - 5 ) < 0 ) ? 0 :  mousePos.x() - 5;
        int y0 = (( mousePos.y() - 5 ) < 0 ) ? 0 :  mousePos.y() - 5;
        int x1 = (( mousePos.x() + 5 ) > this->width()) ? this->width() :  mousePos.x() + 5;
        int y1 = (( mousePos.y() + 5 ) > this->height()) ? this->height() :  mousePos.y() + 5;
        painter.drawLine(x0,mousePos.y(),x1,mousePos.y());
        painter.drawLine(mousePos.x(),y0,mousePos.x(),y1);

        int rectx0 = mousePos.x()  - mousePos.x() % 24;
        int recty0 = mousePos.y()  - mousePos.y() % 24;
        painter.drawRect(rectx0, recty0, _Rect.width(), _Rect.height());
        _DrawRect = QRect(rectx0,recty0,_Rect.width(),_Rect.height());

        painter.setOpacity(0.5);
        painter.drawPixmap(20,20,LayerIcon);
        painter.setOpacity(1);
    }
}
