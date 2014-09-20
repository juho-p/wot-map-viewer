#include "siema.h"

#include <QPainter>
#include <QtDebug>

Siema::Siema()
{
    connection = new Connection(this, "127.0.0.1", 4444);
    connect(connection, &Connection::mapUpdated, this, &Siema::changeMap);
    connect(connection, &Connection::infoUpdated, this, &Siema::infoChanged);
}

void Siema::paint(QPainter *painter) {
    painter->drawPixmap(QRect(0, 0, width(), height()), map_image);
}

void Siema::changeMap() {
    map_image = QPixmap::fromImage(connection->map());
    update();
}
