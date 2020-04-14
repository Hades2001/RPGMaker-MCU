#ifndef ELVESRESOURCES_H
#define ELVESRESOURCES_H

#include <QWidget>
#include <QImage>
#include <QFile>
#include <QFileDialog>
#include <QtDebug>
#include <QStandardItem>
#include <QPainter>

#include <sysdef.h>



typedef struct ElvesPack{
    int colorMode;
    int width;
    int height;
    int ElvesNumber;
    Elves_t *Elves;
}ElvesPack_t;

namespace Ui {
class ElvesResources;
}

class ElvesResources : public QWidget
{
    Q_OBJECT

public:
    explicit ElvesResources(QWidget *parent = nullptr);
    ~ElvesResources();
    void drawimage(QImage *image);
    int  autoCheckElves();
    Elves_t getElvesColorMap(QString name, int posx,int posy, int width, int height);
    int SaveElvesFile(ElvesPack_t *elpack, QString filepath);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_bn_import_pressed();
    void on_bn_AutoFind_pressed();
    void on_le_width_textChanged(const QString &arg1);
    void on_le_height_textChanged(const QString &arg1);
    void on_bn_Generate_pressed();

signals:
    void widgetResize(int w,int h);

private:
    Ui::ElvesResources *ui;

    int _ElvesWidth = 10;
    int _ElvesHeight = 10;

private:
    QImage ElvesImage;
    QImage ElvesImage_draw;
};

#endif // ELVESRESOURCES_H
