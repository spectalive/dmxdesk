#include <QCoreApplication>
#include <QEvent>
#include <QFile>
#include <QGuiApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickWindow>
#include <QUrl>
#include <QVariantList>
#include <QVariantMap>

#include <cstdio>

#include "DeskLayoutTestBridge.h"
#include "advance_scene.h"
#include "scene_items.h"

// 2026-11-02: on the tablet a tab switch spent 150-260 ms of CPU in the scene
// graph's sync, because the desk destroyed and rebuilt its tiles whenever a
// page had fewer controls than the last. Snapshots and page round trips must
// reuse the items that are there.
int main(int argc, char **argv) {
    QGuiApplication app(argc, argv);
    QFile fixture(QStringLiteral(DESK_TEST_FIXTURE));
    if (!fixture.open(QIODevice::ReadOnly))
        return 1;
    const QVariantMap pages = QJsonDocument::fromJson(fixture.readAll()).object().toVariantMap();
    const QVariantMap direct = pages.value("directo").toMap();
    const QVariantMap prism = pages.value("prisma").toMap();
    DeskLayoutTestBridge bridge;
    bridge.insert("state", direct);
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("deskBridge", &bridge);
    engine.load(QUrl::fromLocalFile(argc > 1 ? QString::fromLocal8Bit(argv[1])
                                           : QStringLiteral(DESK_TEST_QML)));
    if (engine.rootObjects().size() != 1)
        return 2;
    auto *window = qobject_cast<QQuickWindow *>(engine.rootObjects().first());
    if (!window)
        return 3;
    advance_scene(300);

    const qsizetype settled = scene_items(window->contentItem()).size();
    // Anything at all on a tick; on a page change, the tiles, which are the
    // expensive part (each holds every content variant).
    int destroyed = 0;
    int tilesDestroyed = 0;
    for (QQuickItem *item : scene_items(window->contentItem())) {
        QObject::connect(item, &QObject::destroyed, [&destroyed] { ++destroyed; });
        if (item->objectName().startsWith("layout-control:"))
            QObject::connect(item, &QObject::destroyed, [&tilesDestroyed] { ++tilesDestroyed; });
    }

    QVariantMap tick = direct;
    QVariantList dials = tick.value("dials").toList();
    QVariantMap dial = dials[0].toMap();
    dial.insert("bpm", dial.value("bpm").toInt() + 1);
    dials[0] = dial;
    tick.insert("dials", dials);
    bridge.insert("state", tick);
    advance_scene(100);
    // Repeaters free what they drop with deleteLater; count it now.
    QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
    const qsizetype afterTick = scene_items(window->contentItem()).size();
    std::printf("tick: destroyed=%d items %lld -> %lld\n", destroyed,
                static_cast<long long>(settled), static_cast<long long>(afterTick));
    if (destroyed != 0 || afterTick != settled)
        return 4;

    bridge.insert("state", prism);
    advance_scene(300);
    QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
    int shown = 0;
    for (QQuickItem *item : scene_items(window->contentItem()))
        if (item->objectName().startsWith("layout-control:") && item->isVisible())
            ++shown;
    std::printf("prism: tiles destroyed=%d shown=%d of %lld\n", tilesDestroyed, shown,
                static_cast<long long>(prism.value("controls").toList().size()));
    if (tilesDestroyed != 0 || shown != prism.value("controls").toList().size())
        return 5;

    bridge.insert("state", direct);
    advance_scene(300);
    QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
    const qsizetype back = scene_items(window->contentItem()).size();
    std::printf("back: tiles destroyed=%d items %lld -> %lld\n", tilesDestroyed,
                static_cast<long long>(settled), static_cast<long long>(back));
    if (tilesDestroyed != 0 || back != settled)
        return 6;
    std::puts("snapshot reuse: tick and page round trip kept every item");
    return 0;
}
