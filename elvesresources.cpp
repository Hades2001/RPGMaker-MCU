#include "elvesresources.h"
#include "ui_elvesresources.h"

ElvesResources::ElvesResources(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ElvesResources)
{
    ui->setupUi(this);

    ui->le_height->setValidator(new QIntValidator(5, 1000, this));
    ui->le_width->setValidator(new QIntValidator(5, 1000, this));

    ui->cb_colorMode->addItem("RGB888");
    ui->cb_colorMode->addItem("RGB565");
    ui->cb_colorMode->addItem("Index256");
    ui->cb_colorMode->addItem("Mono");

    ui->tbw_Elves->setColumnCount(4); //设置列数
    ui->tbw_Elves->setColumnWidth(0, 60);  //0 设置列宽
    ui->tbw_Elves->setColumnWidth(1, 100); //1 设置列宽
    ui->tbw_Elves->setColumnWidth(2, 60);  //2 设置列宽
    ui->tbw_Elves->setColumnWidth(3, 60);  //3 设置列宽


    QStringList sListHeader;
    sListHeader << "NO." << "Name" << "PosX" << "PosY";
    ui->tbw_Elves->setHorizontalHeaderLabels(sListHeader);
}

ElvesResources::~ElvesResources()
{
    delete ui;
}
void ElvesResources::resizeEvent(QResizeEvent *event)
{
   Q_UNUSED(event);
   drawimage(&ElvesImage_draw);
}
void ElvesResources::drawimage(QImage *image)
{
    if( image == nullptr ) return;
    QImage imagedraw = image->scaled(ui->label->width(),ui->label->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label->setPixmap(QPixmap::fromImage(imagedraw));
    ui->label->update();
    ui->label->repaint();
}

Elves_t ElvesResources::getElvesColorMap(QString name, int posx,int posy, int width, int height)
{
    Elves_t *elves = new Elves_t;
    elves->name = name;
    elves->pixnumber = width * height;
    elves->data = new QColor[width * height];
    for(int i = 0; i< width * height; i++ )
    {
        elves->data[i] = ElvesImage.pixelColor( ( posx ) + i % width,
                                                ( posy ) + i / width);
    }
    return *elves;
}

int ElvesResources::SaveElvesFile(ElvesPack_t *elpack, QString filepath)
{
    filepath.append(QString("/"));
    QFile hfile( filepath + ui->le_name->text() + ".h");
    hfile.open(QIODevice::WriteOnly | QIODevice::Text);
    QString hfilestr;
    hfilestr.append(QString("#define ELVES_NUM %1\n").arg(elpack->ElvesNumber));
    hfilestr.append(QString("#define ELVES_W %1\n").arg(elpack->width));
    hfilestr.append(QString("#define ELVES_H %1\n").arg(elpack->height));

    for (int i = 0; i< elpack->ElvesNumber; i++ ) {
        hfilestr.append(QString("#define ") +
                        elpack->Elves[i].name+
                        QString(" %1\n").arg(i));
    }
    hfile.write(hfilestr.toLatin1());
    hfile.close();

    QFile cfile(filepath + ui->le_name->text() + ".c");
    cfile.open(QIODevice::WriteOnly );
    QString cfilestr;

    int pixdatanum = (elpack->width*elpack->height*elpack->colorMode)/8;
    cfilestr.append(QString("uint8_t") +
                    ui->le_name->text() +
                    QString("buff[%1][%2]=\n{\n").arg(elpack->ElvesNumber).arg(pixdatanum));
    for( int i = 0; i < elpack->ElvesNumber; i++ )
    {
        cfilestr.append(QString("{  //") + elpack->Elves[i].name + QString(" %1\n").arg(i));
        for( int n = 0; n < (elpack->width*elpack->height); n++ )
        {
            QColor pix = elpack->Elves[i].data[n];
            quint16 data = quint16(TORGB565(pix.rgb()));
            cfilestr.append(QString("0x%1,").arg((data>>8)&0xff,2,16,QLatin1Char('0')));
            cfilestr.append(QString("0x%1,").arg((data)&0xff,2,16,QLatin1Char('0')));
        }
        cfilestr.append("}\n");
    }
    cfilestr.append("};");
    cfile.write(cfilestr.toLatin1());
    cfile.close();
    return -1;
}

int ElvesResources::autoCheckElves()
{
    int scanWidth = 0,scanHeight = 0;
    ElvesImage_draw = ElvesImage;
    QPainter painter;
    painter.begin(&ElvesImage_draw);
    painter.setRenderHint(QPainter::Antialiasing, true);

    ui->tbw_Elves->clear();
    /*
    scanWidth = ( ElvesImage.width() % _ElvesWidth != 0 ) ? ( ElvesImage.width() / _ElvesWidth + 1 ) : ( ElvesImage.width() / _ElvesWidth );
    scanHeight = ( ElvesImage.height() % _ElvesHeight != 0 ) ? ( ElvesImage.height() / _ElvesHeight + 1 ) : ( ElvesImage.height() / _ElvesHeight );
    */

    scanWidth =  ( ElvesImage.width() / _ElvesWidth );
    scanHeight = ( ElvesImage.height() / _ElvesHeight );

    int Elvesnumber = 0;
    bool checkflag = false;
    for ( int y = 0; y < scanHeight; y++ )
    {
        for ( int x = 0; x < scanWidth; x++ )
        {
            checkflag = false;
            for( int i = 0; i < (_ElvesWidth * _ElvesHeight ); i++)
            {
                QColor pix= ElvesImage.pixelColor( ( x * _ElvesWidth ) + i % _ElvesWidth,
                                                   ( y * _ElvesHeight) + i / _ElvesWidth);

                if( pix.alpha() != 0 )
                {
                    checkflag = true;
                    qDebug()<<( x * _ElvesWidth ) + i % _ElvesWidth<<( y * _ElvesHeight) + i / _ElvesWidth<<pix;
                    break;
                }
            }

            if(checkflag != false)
            {
                painter.setPen(QPen(QColor(0, 160, 230), 1));
                painter.drawRect(x * _ElvesWidth,
                                 y * _ElvesHeight,
                                 _ElvesWidth,
                                 _ElvesHeight);

                painter.setPen(QPen(QColor(0, 160, 230), 1));
                painter.setBrush(QColor(255, 255, 255, 128));
                painter.drawRect(x * _ElvesWidth,
                                 y * _ElvesHeight,
                                 _ElvesWidth,
                                 10);
                painter.setBrush(QColor(0, 160, 230, 0));
                painter.setPen(QPen(QColor(0, 0, 0), 1));
                painter.drawText(x * _ElvesWidth,
                                 y * _ElvesHeight,
                                 _ElvesWidth,
                                 10,
                                 Qt::AlignHCenter | Qt::AlignVCenter,
                                 QString("%1").arg(Elvesnumber));

                ui->tbw_Elves->setRowCount(Elvesnumber + 1 );

                QTableWidgetItem *item1 = new QTableWidgetItem;
                item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                item1->setText(QString("%1").arg(Elvesnumber));
                ui->tbw_Elves->setItem(Elvesnumber,0,item1);

                QTableWidgetItem *item2 = new QTableWidgetItem;
                item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                item2->setText(ui->le_name->text() + QString("%1").arg( Elvesnumber ));
                ui->tbw_Elves->setItem(Elvesnumber,1,item2);

                QTableWidgetItem *item3 = new QTableWidgetItem;
                item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                item3->setText(QString("%1").arg( x * _ElvesWidth ));
                ui->tbw_Elves->setItem(Elvesnumber,2,item3);

                QTableWidgetItem *item4 = new QTableWidgetItem;
                item4->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                item4->setText(QString("%1").arg( y * _ElvesHeight ));
                ui->tbw_Elves->setItem(Elvesnumber,3,item4);

                Elvesnumber++;
            }
        }
    }
    painter.end();
    drawimage(&ElvesImage_draw);

    return -1;
}

void ElvesResources::on_bn_import_pressed()
{
    QString apppath = QApplication::applicationDirPath();
    QString imagepath = QFileDialog::getOpenFileName(this,
                                 "open",
                                 apppath,
                                 "image(*.bmp *.png *.jpeg *.jpg )");

    if( imagepath.length() == 0 ) return;

    qDebug()<<imagepath;
    if(( imagepath.lastIndexOf('/') != -1)&&\
       ( imagepath.lastIndexOf('.') != -1))
    {
        QString name = imagepath.mid(imagepath.lastIndexOf('/') + 1,
                                     imagepath.lastIndexOf('.') - imagepath.lastIndexOf('/') - 1);
        ui->le_name->setText(name);
    }

    ElvesImage.load(imagepath);

    /*
    int labwidth = 0, labheight = 0;
    float scale = 0;
    labheight = 480;
    scale = float(ElvesImage.width()) / float(ElvesImage.height());

    labwidth = int( labheight * scale );
    ui->label->resize(labwidth,labheight);
    */
    ElvesImage_draw = ElvesImage;

    drawimage(&ElvesImage_draw);
}

void ElvesResources::on_bn_AutoFind_pressed()
{
    autoCheckElves();
}

void ElvesResources::on_le_width_textChanged(const QString &arg1)
{
    _ElvesWidth = arg1.toInt();
}

void ElvesResources::on_le_height_textChanged(const QString &arg1)
{
    _ElvesHeight = arg1.toInt();
}

void ElvesResources::on_bn_Generate_pressed()
{
    QString pathstr = QFileDialog::getExistingDirectory(this,
                                                    "Save",
                                                    "./");
    int rowCount = ui->tbw_Elves->rowCount();

    ElvesPack_t *elpack = new ElvesPack_t;
    elpack->colorMode = 16;
    elpack->ElvesNumber = rowCount;
    elpack->width = _ElvesWidth;
    elpack->height = _ElvesHeight;
    elpack->Elves = new Elves_t[rowCount];

    for( int i = 0; i< rowCount; i ++)
    {
        QTableWidgetItem *name = ui->tbw_Elves->item(i,1);
        QTableWidgetItem *posx = ui->tbw_Elves->item(i,2);
        QTableWidgetItem *posy = ui->tbw_Elves->item(i,3);

        elpack->Elves[i] = getElvesColorMap(name->text(),
                                            posx->text().toInt(),
                                            posy->text().toInt(),
                                            _ElvesWidth,
                                            _ElvesHeight);
    }

    pathstr.append(QString("/") + ui->le_name->text());
    QDir *GenerateDir = new QDir;

    if( !GenerateDir->exists(pathstr))
    {
        GenerateDir->mkdir(pathstr);
    }
    delete GenerateDir;

    SaveElvesFile(elpack,pathstr);
}
