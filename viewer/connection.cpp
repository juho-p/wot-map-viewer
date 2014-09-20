#include "connection.h"
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>

#include "util.h"

Connection::Connection(QObject *parent, QString host, int port) :
    QObject(parent)
{
    socket = new QTcpSocket(this);
    connection_host = host;
    connection_port = port;

    QObject::connect(socket,
                static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),
                [](QAbstractSocket::SocketError error) {
        qWarning() << "TCP error: " << error;
    });
    connect(socket, &QTcpSocket::readyRead, this, &Connection::readyRead);

    watchdog_timer.setInterval(5000);
    watchdog_timer.setSingleShot(false);
    watchdog_timer.start();
    connect(&watchdog_timer, &QTimer::timeout, this, &Connection::timeout);

    poll_timer.setInterval(120);
    poll_timer.setSingleShot(true);
    connect(&poll_timer, &QTimer::timeout, this, &Connection::requestInfo);

    startConnection();
}

void Connection::requestInfo() {
    if (connection_status != Idle) return;

    if (socket->state() == QAbstractSocket::ConnectedState) {
        watchdog_timer.start();
        socket->write("INFO\n");
        socket->flush();
    } else {
        poll_timer.start();
    }
}

void Connection::requestMap() {
    qDebug() << "request map";
    watchdog_timer.start();
    socket->write("MINIMAP\n");
    socket->write(map_name);
    socket->write("\n");
    socket->flush();
}

void Connection::readyRead() {
    watchdog_timer.start();
    if (connection_status == Idle) {
        tryReadCommand();
    }
    if (connection_status == Idle) return;

    if (connection_status == WaitInfo) {
        tryReadInfo();
    } else if (connection_status == WaitMap) {
        tryReadMap();
    }
}

void Connection::timeout() {
    connection_status = Idle;
    socket->disconnectFromHost();
    startConnection();
}

void Connection::startConnection() {
    poll_timer.stop();
    map_name.clear();
    expected_data_length = 0;
    connection_status = Idle;
    socket->connectToHost(connection_host, connection_port);
    requestInfo();
}

void Connection::tryReadCommand() {
    if (socket->bytesAvailable() < 9) return;
    auto line = socket->readLine();
    if (line == "INFO\n")
        connection_status = WaitInfo;
    else if (line == "MINIMAP\n")
        connection_status = WaitMap;
    else
        qWarning() << "Got weird response: " << line;
}

int Connection::tryReadSize() {
    if (expected_data_length) return expected_data_length;
    if (socket->bytesAvailable() < 7) return 0;
    auto line = socket->readLine();
    line.chop(1);
    expected_data_length = line.toInt();
    return expected_data_length;
}

void Connection::tryReadInfo() {
    int sz = tryReadSize();
    if (sz && socket->bytesAvailable() >= sz) {
        expected_data_length = 0;
        info_json = socket->read(sz);
        connection_status = Idle;
        emit infoUpdated();

        auto newmap = QJsonDocument::fromJson(info_json)
                .object()["map"].toString().toUtf8();

        connection_status = Idle;
        if (!newmap.isEmpty() && newmap != map_name) {
            map_name = newmap;
            requestMap();
        } else {
            poll_timer.start();
        }
    }
}

void Connection::tryReadMap() {
    int sz = tryReadSize();
    if (sz && socket->bytesAvailable() >= sz) {
        expected_data_length = 0;
        QByteArray data = socket->read(sz);
        map_image = readDDS(data);
        connection_status = Idle;
        emit mapUpdated();
        poll_timer.start();
    }
}
