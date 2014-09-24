/*
 * WoT map viewer
 *
 * author: Juho Peltonen
 * license: GPL3, see gpl-3.0.txt included in the package
 */

#include "minimap.h"

#include <QPainter>
#include <QtDebug>
#include <QFile>

Minimap::Minimap()
{
    QFile config("connection.txt");
    config.open(QIODevice::ReadOnly);
    if (config.error()) {
        qWarning() << "Error while reading connection.txt: " << config.errorString();
    }
    auto address = config.readAll();
    int colon_index = address.indexOf(':');
    int newline_index = address.indexOf('\n');
    QString host = "localhost";
    int port = 4444;
    if (colon_index > 0) {
        host = address.left(colon_index);
        port = address.mid(colon_index + 1, newline_index > 0 ? newline_index - colon_index - 1 : -1).toInt();
    } else {
        qWarning() << "Could not get valid address from connection.txt, using default config";
    }
    qDebug() << "host: " << host << " port: " << port;
    connection = new Connection(this, host, port);
    connect(connection, &Connection::mapUpdated, this, &Minimap::changeMap);
    connect(connection, &Connection::infoUpdated, this, &Minimap::infoChanged);
}

void Minimap::paint(QPainter *painter) {
    painter->drawPixmap(QRect(0, 0, width(), height()), map_image);
}

void Minimap::changeMap() {
    map_image = QPixmap::fromImage(connection->map());
    update();
}
