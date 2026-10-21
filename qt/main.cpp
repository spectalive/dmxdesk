#include <QCoreApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QTimer>
#include <QUrl>

#include "DeskBridge.h"

int main(int argc, char **argv) {
    QGuiApplication app(argc, argv);
    QString backend = QStringLiteral("/data/qt-full/dmxdesk-backend");
    QString map = QStringLiteral("/usr/share/dmxdesk/vibra.desk.json");
    QString power;
    QString qml = QStringLiteral("/data/qt-full/qml/Main.qml");
    QString capture;
    for (int i = 1; i < argc; i++) {
        if (i + 1 >= argc)
            return 2;
        const QString option = QString::fromLocal8Bit(argv[i++]);
        const QString value = QString::fromLocal8Bit(argv[i]);
        if (option == QStringLiteral("--backend")) backend = value;
        else if (option == QStringLiteral("--map")) map = value;
        else if (option == QStringLiteral("--qml")) qml = value;
        else if (option == QStringLiteral("--power")) power = value;
        else if (option == QStringLiteral("--capture")) capture = value;
        else return 2;
    }
    DeskBridge bridge(backend, map, power);
    QObject::connect(&bridge, &DeskBridge::failed, &app, [] { QCoreApplication::exit(1); });
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("deskBridge"), &bridge);
    engine.load(QUrl::fromLocalFile(qml));
    if (engine.rootObjects().isEmpty())
        return 1;
    if (!capture.isEmpty()) {
        auto *window = qobject_cast<QQuickWindow *>(engine.rootObjects().first());
        if (!window)
            return 1;
        QTimer::singleShot(3000, &app, [window, capture] {
            if (window->grabWindow().save(capture))
                QCoreApplication::quit();
            else
                QCoreApplication::exit(1);
        });
    }
    return app.exec();
}
