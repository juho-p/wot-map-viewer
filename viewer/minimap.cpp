/*
 * WoT map viewer
 *
 * author: Juho Peltonen
 * license: GPL3, see gpl-3.0.txt included in the package
 */

#include "minimap.h"

#include <QPainter>
#include <QtDebug>

Minimap::Minimap()
{
    connection = new Connection(this, "127.0.0.1", 4444);
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
