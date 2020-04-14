#include "mappainter.h"
#include "ui_mappainter.h"

MapPainter::MapPainter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapPainter)
{
    ui->setupUi(this);

    _mapptr = creatMap("map",30,30);
    mapPainterWidget *lab = new mapPainterWidget(this,_mapptr);
    ui->sA_mapPainter->setWidget(lab);

    connect(ui->Bn_Layer1,&QPushButton::pressed,[=]{lab->setmapLayer(1);});
    connect(ui->Bn_Layer2,&QPushButton::pressed,[=]{lab->setmapLayer(2);});
    connect(ui->Bn_Layer3,&QPushButton::pressed,[=]{lab->setmapLayer(3);});

    connect(lab,&mapPainterWidget::MouseMove,[=](QPoint p){
        Q_UNUSED(p);
        //ui->lineEdit->setText(QString("%1,%2").arg(p.x()).arg(p.y()));
    });

    ui->tabWidget->clear();
    QStringList mapfilepath;
    for( int i = 0; i < mapimage.size(); i++ )
    {
        QString apppath = QApplication::applicationDirPath();
        apppath.append("/img/tilesets/" + mapimage.at(i));
        qDebug()<<apppath;
        mapfilepath.append(apppath);
        _maplib[i] = new maplibwidget(this,i,apppath);
        _maplab[i] = new QScrollArea;
        _maplab[i]->setWidget(_maplib[i]);
        ui->tabWidget->addTab(_maplab[i],QString("%1").arg(i));

        connect( _maplib[i],&maplibwidget::ChooseRect,[=](int n,QRect r){
            lab->setPainterImage(n,r);
        });
    }

    lab->setMapImageList(mapfilepath);
}

MapPainter::~MapPainter()
{
    delete ui;
}

mappack_t* MapPainter::creatMap(QString name,int w,int h)
{
    mappack_t *map = new mappack_t;
    map->size_w = w;
    map->size_h = h;
    map->name = name;

    map->maplayer0 = new uint16_t*[map->size_h];
    map->maplayer1 = new uint16_t*[map->size_h];
    map->maplayer2 = new uint16_t*[map->size_h];

    for(int y = 0; y < map->size_h; y++ )
    {
        map->maplayer0[y] = new uint16_t[map->size_w];
        map->maplayer1[y] = new uint16_t[map->size_w];
        map->maplayer2[y] = new uint16_t[map->size_w];

        memset(map->maplayer0[y],0,sizeof(uint16_t)*size_t(map->size_w));
        memset(map->maplayer1[y],0,sizeof(uint16_t)*size_t(map->size_w));
        memset(map->maplayer2[y],0,sizeof(uint16_t)*size_t(map->size_w));
    }
    return map;
}

int MapPainter::deletemap(mappack_t* map)
{
    for(int y = 0; y < map->size_h; y++ )
    {
        delete [] map->maplayer0[y];
        delete [] map->maplayer1[y];
        delete [] map->maplayer2[y];
    }
    delete [] map->maplayer0;
    delete [] map->maplayer1;
    delete [] map->maplayer2;

    delete map;
    return 0;
}

void MapPainter::on_Bn_Generated_pressed()
{
    QString path = QFileDialog::getExistingDirectory(this,
                                                    "Generate",
                                                    "./");
    if(path.isEmpty()) return;


    QStringList imageList = QFileDialog::getOpenFileNames(this,
                                                          "Open",
                                                          "./",
                                                          "image(*.png)");
    /*
    if( _mapptr == nullptr ) return;
    QString filestr;

    //filestr.append("uint16_t **mapptr[3] = {\n");
    //filestr.append(_mapptr->name + "_maplayer0,\n");
    //filestr.append(_mapptr->name + "_maplayer1,\n");
    //filestr.append(_mapptr->name + "_maplayer2,\n");
    //filestr.append("}\n");


    uint16_t **mapptr[3];
    mapptr[0] = _mapptr->maplayer0;
    mapptr[1] = _mapptr->maplayer1;
    mapptr[2] = _mapptr->maplayer2;

    for ( int i = 0; i < 3; i++ )
    {
        filestr.append(QString("uint16_t " + _mapptr->name + "_maplayer%1[%2][%3] = { \n")
                       .arg(i)
                       .arg(_mapptr->size_h)
                       .arg(_mapptr->size_w));

        for( int posy = 0; posy < _mapptr->size_h; posy ++ )
        {
            filestr.append("{\n");
            for( int posx = 0; posx < _mapptr->size_w; posx ++ )
            {
                filestr.append(QString("0x%1,").arg(mapptr[i][posy][posx]&0xffff,4,16,QLatin1Char('0')));
                if(( posx != 0 )&&( posx % 24 == 23 )) filestr.append('\n');
            }
            filestr.append("},\n");
        }
        filestr.append("}\n");
    }

    QFile file(path + '/' + _mapptr->name + ".c");

    file.open(QFile::WriteOnly);
    file.write(filestr.toLatin1());
    file.close();

    //_mapptr->size_h
    */


    if( imageList.isEmpty()) return;

    QString headFileStr,mapList;
    mapList.append("/* \nuint8_t **mapLibList = {\n");
    headFileStr.append( "   #ifndef MAPPACKLIB_H\n\
                            #define MAPPACKLIB_H\n");

    ///Users/chaoxuanzhe/qt/GameTools/GameTools/mappainter.cpp:152: warning: macro name is a reserved identifier
    //headFileStr

    for( int i = 0; i < imageList.size(); i++ )
    {
        QImage *img = new QImage();

        QString name = imageList.at(i).mid( imageList.at(i).lastIndexOf('/') + 1,
                                            imageList.at(i).lastIndexOf('.') - imageList.at(i).lastIndexOf('/') - 1);

        img->load(imageList.at(i));
        int countx = img->width() / 24;
        int county = img->height() / 24;

        qDebug() << name + ".png" << QString("Size:%1").arg(countx * county);

        //qDebug()<<imageList.at(i)<<name<<QString("%1,%2").arg(countx).arg(county);

        QString binfile;


        headFileStr.append(QString("extern const uint8_t map_")
                           + name
                           + QString("data[%1][%2];\n").arg(countx*county).arg(24*24*2));

        mapList.append(QString("map_") + name + "data,\n");

        binfile.append("#include \"mappacklib.h\"\n");
        binfile.append(QString("const uint8_t map_") + name + QString("data[%1][%2]").arg(countx*county).arg(24*24*2));
        binfile.append(QString("={ \n"));

        int *colorUserBuff = new int[65536];
        for( int t = 0; t < 65536; t++ )
        {
            colorUserBuff[i] = 0;
        }

        for(int posy = 0; posy < county; posy++ )
        {
            for(int posx = 0; posx < countx; posx++ )
            {
                binfile.append("{ // -- \n");
                for(int i = 0; i < 576; i++ )
                {
                   quint16 color = quint16(TORGB565(img->pixelColor(posx * 24 + i % 24, posy * 24 + i / 24).rgb()));
                   colorUserBuff[color] = 1;

                   binfile.append(QString("0x%1, ").arg(color >> 8, 2, 16, QLatin1Char('0')));
                   binfile.append(QString("0x%1, ").arg(color % 0xff, 2, 16, QLatin1Char('0')));
                   if(( i != 0 )&&( i % 24 == 23 ))binfile.append(" \n");
                }
                binfile.append("}, \n");
            }
        }
        binfile.append("}; \n");

        for( int t = 0; t < 65536; t++ )
        {
            if(colorUserBuff[i] == 0)
            {
                binfile.append(QString("//uint16_t Transparent = %1; \n").arg(t));
                break;
            }
        }

        QString Generate = path +'/' + name + ".c";
        //qDebug()<<Generate;
        QFile file(Generate);

        file.open(QFile::WriteOnly);
        file.write(binfile.toLatin1());
        file.close();

        delete[] colorUserBuff;
        delete img;
    }

    mapList.append("};\n*/");
    headFileStr.append(mapList);
    headFileStr.append("#endif\n");

    QString Generate = path +'/' + "mappacklib.h";
    QFile file(Generate);
    file.open(QFile::WriteOnly);
    file.write(headFileStr.toLatin1());
    file.close();
}
