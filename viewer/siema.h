/*
 * WoT map viewer
 *
 * author: Juho Peltonen
 * license: GPL3, see gpl-3.0.txt included in the package
 */

#ifndef SIEMA_H
#define SIEMA_H

#include <QQuickPaintedItem>
#include <QPixmap>
#include "connection.h"

/* Just show some image and give some info property */

class Siema : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString host READ host)
    Q_PROPERTY(int port READ port)
    Q_PROPERTY(QString info READ info NOTIFY infoChanged)
public:
    Siema();

    QString host() { return connection->host(); }
    int port() { return connection->port(); }
    QString info() { return connection->info(); }

signals:
    void infoChanged();

private slots:
    void changeMap();


protected:
    void paint(QPainter *painter);

    QPixmap map_image;
    Connection* connection;
};

#endif // SIEMA_H
