#ifndef CONNECTION_H
#define CONNECTION_H

#include <QTimer>
#include <QTcpSocket>
#include <QObject>
#include <QImage>

/* Terrible state machine turd for connecting some tcp port to read info and map */
class QTcpSocket;

class Connection : public QObject
{
    Q_OBJECT
public:
    explicit Connection(QObject *parent, QString host, int port);

    QByteArray info() { return info_json; }
    QImage map() { return map_image; }

    QString host() { return connection_host; }
    int port() { return connection_port; }

signals:
    void infoUpdated();
    void mapUpdated();

private slots:
    void readyRead();
    void requestInfo();
    void requestMap();
    void timeout();



private:
    enum Status { Idle, WaitMap, WaitInfo };

    void startConnection();
    void tryReadCommand();
    int tryReadSize();
    void tryReadInfo();
    void tryReadMap();

    QTcpSocket* socket;
    QByteArray info_json;
    QByteArray map_name;
    QImage map_image;
    Status connection_status;
    QTimer watchdog_timer, poll_timer;
    QString connection_host;
    int connection_port;
    int expected_data_length;
};

#endif // CONNECTION_H
