#ifndef SYSDEF_H
#define SYSDEF_H

#include <QObject>
#include <QImage>

typedef struct mappack{
    QString name;
    int Group;
    int size_w;
    int size_h;
    uint16_t **maplayer0;
    uint16_t **maplayer1;
    uint16_t **maplayer2;
    QImage mapimage;
}mappack_t;

//[15][14:12][11][10][9:0]


#define TORGB565(color) ((((color) >> 19) & 0x1f) << 11) \
                       |((((color) >> 10) & 0x3f) << 5) \
                       |(((color) >> 3) & 0x1f)


typedef struct colorIndex{
    QRgb index[265];
}colorIndex_t;

typedef struct Elves{
    QString name;
    int pixnumber;
    QColor *data;
}Elves_t;

#endif // SYSDEF_H
