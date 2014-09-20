#include <QtGui/QGuiApplication>
#include <QtQml>
#include "qtquick2applicationviewer.h"

#include "siema.h"
#include <QTimer>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtQuick2ApplicationViewer viewer;
    qmlRegisterType<Siema>("mapviewer.siemapl", 1, 0, "Siema");
    viewer.setMainQmlFile(QStringLiteral("qml/viewer/main.qml"));
    viewer.showExpanded();


    return app.exec();
}
