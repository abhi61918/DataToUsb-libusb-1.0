#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTimer>
#include "usbexample.h"


int main(int argc, char *argv[])
{
    QTimer timer;

    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    QObject::connect(&timer, SIGNAL(timeout()), &app, SLOT(quit()));

    timer.setInterval(1000);
    timer.setSingleShot(true);
    timer.start();

    UsbExample example;
    return app.exec();
}
