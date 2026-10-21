#include <QCoreApplication>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QTimer>
#include <QUrl>

#include "DeskBridge.h"
#include "DeskAgentControl.h"

int main(int argc, char **argv) {
    QGuiApplication app(argc, argv);
    QString backend = QStringLiteral("/data/qt-full/dmxdesk-backend");
    QString map = QStringLiteral("/usr/share/dmxdesk/vibra.desk.json");
    QString power;
    QString qml = QStringLiteral("/data/qt-full/qml/Main.qml");
    QString capture;
    QString font;
    QString tap;
    bool agent_stdio = false;
    for (int i = 1; i < argc; i++) {
        if (QString::fromLocal8Bit(argv[i]) == QStringLiteral("--agent-stdio")) {
            agent_stdio = true;
            continue;
        }
        if (i + 1 >= argc)
            return 2;
        const QString option = QString::fromLocal8Bit(argv[i++]);
        const QString value = QString::fromLocal8Bit(argv[i]);
        if (option == QStringLiteral("--backend")) backend = value;
        else if (option == QStringLiteral("--map")) map = value;
        else if (option == QStringLiteral("--qml")) qml = value;
        else if (option == QStringLiteral("--power")) power = value;
        else if (option == QStringLiteral("--capture")) capture = value;
        else if (option == QStringLiteral("--font")) font = value;
        else if (option == QStringLiteral("--tap")) tap = value;
        else return 2;
    }
    if (!font.isEmpty() && QFontDatabase::addApplicationFont(font) < 0)
        return 1;
    DeskBridge bridge(backend, map, power);
    QObject::connect(&bridge, &DeskBridge::failed, &app, [] { QCoreApplication::exit(1); });
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("deskBridge"), &bridge);
    engine.load(QUrl::fromLocalFile(qml));
    if (engine.rootObjects().isEmpty())
        return 1;
    auto *window = qobject_cast<QQuickWindow *>(engine.rootObjects().first());
    if (!window)
        return 1;
    if (agent_stdio)
        new DeskAgentControl(&bridge, window, &app);
    if (!tap.isEmpty()) {
        const QStringList coordinates = tap.split(',');
        bool valid_x = false;
        bool valid_y = false;
        const int x = coordinates.value(0).toInt(&valid_x);
        const int y = coordinates.value(1).toInt(&valid_y);
        if (coordinates.size() != 2 || !valid_x || !valid_y)
            return 2;
        QTimer::singleShot(1200, &app, [&bridge, x, y] {
            bridge.touch(QStringLiteral("D"), 0, x, y);
            bridge.touch(QStringLiteral("U"), 0, x, y);
        });
    }
    if (!capture.isEmpty()) {
        QTimer::singleShot(3000, &app, [window, capture] {
            if (window->grabWindow().save(capture))
                QCoreApplication::quit();
            else
                QCoreApplication::exit(1);
        });
    }
    return app.exec();
}
