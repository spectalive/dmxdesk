#include <QFile>
#include <QGuiApplication>
#include <QImage>
#include <QJsonDocument>
#include <QJsonObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>

#include <cstdio>

#include "DeskLayoutTestBridge.h"
#include "advance_scene.h"

int main(int argc, char **argv) {
    QGuiApplication app(argc, argv);
    QFile fixture(QStringLiteral(DESK_TEST_FIXTURE));
    if (!fixture.open(QIODevice::ReadOnly))
        return 1;
    QVariantMap state = QJsonDocument::fromJson(fixture.readAll()).object().toVariantMap();
    DeskLayoutTestBridge bridge;
    bridge.insert("state", state);
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("deskBridge", &bridge);
    engine.load(QUrl::fromLocalFile(argc > 1 ? QString::fromLocal8Bit(argv[1])
                                           : QStringLiteral(DESK_TEST_QML)));
    if (engine.rootObjects().size() != 1)
        return 2;
    auto *window = qobject_cast<QQuickWindow *>(engine.rootObjects().first());
    if (!window)
        return 3;

    // 2026-10-24: the Mac preview showed an empty shell beside 100%.
    const int levels[] = {-1, 0, 20, 50, 100, 150};
    int ink[6] = {};
    for (int scenario = 0; scenario < 6; scenario++) {
        state.insert("battery", levels[scenario]);
        bridge.insert("state", state);
        advance_scene(220);
        const QImage frame = window->grabWindow();
        if (frame.size() != QSize(1024, 600))
            return 4;
        for (int y = 21; y < 27; y++)
            for (int x = 822; x < 835; x++)
                if (frame.pixelColor(x, y).red() > 100)
                    ink[scenario]++;
        std::printf("battery %d%%: %d lit interior pixels\n", levels[scenario], ink[scenario]);
    }
    if (ink[0] != 0 || ink[1] != 0 || ink[2] <= 0 ||
        ink[2] >= ink[3] || ink[3] >= ink[4] || ink[4] != ink[5] ||
        ink[4] < 60 || ink[3] < ink[4] * 0.35 || ink[3] > ink[4] * 0.65)
        return 5;

    int previous = -1;
    for (int bars = 0; bars <= 3; bars++) {
        state.insert("wifiBars", bars);
        bridge.insert("state", state);
        advance_scene(180);
        const QImage frame = window->grabWindow();
        QRect bounds;
        int lit = 0;
        for (int y = 12; y < 36; y++)
            for (int x = 777; x < 801; x++)
                if (frame.pixelColor(x, y).red() > 100) {
                    bounds |= QRect(x, y, 1, 1);
                    lit++;
                }
        std::printf("wifi %d bars: %d lit pixels, %dx%d ink bounds\n",
                    bars, lit, bounds.width(), bounds.height());
        if (bars == 0 ? lit != 0 : lit <= previous)
            return 6;
        if (bars == 3 && (bounds.width() < 18 || bounds.width() > 20 ||
                          bounds.height() < 13 || bounds.height() > 15 ||
                          qAbs(bounds.center().y() - 24) > 1))
            return 7;
        previous = lit;
    }
    return 0;
}
