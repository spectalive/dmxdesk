#include <QColor>
#include <QFile>
#include <QFileInfo>
#include <QGuiApplication>
#include <QImage>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QPointer>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickWindow>
#include <QRectF>
#include <QScopedPointer>
#include <QUrl>
#include <QVariantList>
#include <QVariantMap>

#include <cmath>
#include <cstdio>

#include "../desk_view_audit.h"
#include "DeskLayoutTestBridge.h"
#include "advance_scene.h"

int main(int argc, char **argv) {
    QGuiApplication app(argc, argv);
    QFile fixture(QStringLiteral(DESK_TEST_FIXTURE));
    if (!fixture.open(QIODevice::ReadOnly))
        return 1;
    QVariantMap state = QJsonDocument::fromJson(fixture.readAll()).object().toVariantMap();
    QVariantList controls = state.value("controls").toList();
    QVariantMap flash = controls[0].toMap();
    flash.insert("holdProgress", -1);
    controls[0] = flash;
    state.insert("controls", controls);
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
    advance_scene(300);
    QPointer<QQuickItem> tile;
    for (QQuickItem *item : window->contentItem()->childItems())
        if (item->objectName() == "layout-control:direct-effects" && item->x() == 552 && item->y() == 184)
            tile = item;
    if (!tile)
        return 4;
    const QRectF geometry = tile->mapRectToScene(tile->boundingRect());
    auto *remaining = tile->findChild<QQuickItem *>("hold-remaining");
    auto *marker = tile->findChild<QQuickItem *>("selection-marker");
    if (!remaining || !marker)
        return 5;
    const QImage idle = window->grabWindow();

    // 2026-10-24: snapshot replacement must not recreate or restart the tile.
    flash.insert("pressed", true);
    flash.insert("selected", true);
    flash.insert("holdProgress", 0);
    controls[0] = flash;
    state.insert("controls", controls);
    bridge.insert("state", state);
    advance_scene(35);
    if (!tile || !tile->property("pressed").toBool() || marker->width() <= 0 ||
        marker->width() >= tile->width() - 36)
        return 6;
    const QImage attack = window->grabWindow();
    if (attack.pixelColor(560, 200) == idle.pixelColor(560, 200))
        return 7;
    advance_scene(280);
    if (std::abs(marker->width() - (tile->width() - 36)) > 0.1 ||
        std::abs(remaining->width() - (tile->width() - 16)) > 0.1)
        return 8;
    if (desk_view_audit(window->contentItem()) != 0)
        return 9;

    flash.insert("holdProgress", 500);
    controls[0] = flash;
    state.insert("controls", controls);
    bridge.insert("state", state);
    advance_scene(120);
    window->grabWindow();
    if (!tile || std::abs(remaining->width() - (tile->width() - 16) / 2) > 0.1 ||
        tile->mapRectToScene(tile->boundingRect()) != geometry)
        return 10;
    advance_scene(250);
    if (std::abs(remaining->width() - (tile->width() - 16) / 2) > 0.1)
        return 11;

    flash.insert("holdProgress", 1000);
    controls[0] = flash;
    state.insert("controls", controls);
    bridge.insert("state", state);
    advance_scene(220);
    if (tile->property("pressed").toBool() || remaining->width() != 0)
        return 12;
    window->grabWindow();
    if (desk_view_audit(window->contentItem()) != 0)
        return 13;

    flash.insert("holdProgress", -1);
    flash.insert("pressed", false);
    flash.insert("selected", false);
    controls[0] = flash;
    state.insert("controls", controls);
    bridge.insert("state", state);
    advance_scene(320);
    const QImage released = window->grabWindow();
    if (!tile || marker->opacity() != 0 || remaining->isVisible() ||
        released.pixelColor(560, 200) != idle.pixelColor(560, 200))
        return 14;
    for (QObject *object : window->findChildren<QObject *>())
        if (object->property("running").toBool())
            return 16;

#if QT_VERSION >= QT_VERSION_CHECK(6, 12, 0)
    QQmlComponent preferenceComponent(&engine, QUrl::fromLocalFile(
        QFileInfo(QStringLiteral(DESK_TEST_QML)).path() + "/DeskMotionPreference.qml"));
    QScopedPointer<QObject> preference(preferenceComponent.create());
    if (!preference)
        return 17;
    preference->setProperty("preference", 1);
    if (!preference->property("reduced").toBool())
        return 18;
    preference->setProperty("preference", 0);
    if (preference->property("reduced").toBool())
        return 19;
    QQmlComponent numberComponent(&engine, QUrl::fromLocalFile(
        QFileInfo(QStringLiteral(DESK_TEST_QML)).path() + "/DeskNumberMotion.qml"));
    QScopedPointer<QObject> number(numberComponent.create());
    if (!number)
        return 20;
    QObject *numberPreference = number->property("preference").value<QObject *>();
    numberPreference->setProperty("preference", 1);
    if (number->property("duration").toInt() != 0)
        return 21;
#endif

    flash.insert("pressed", true);
    flash.insert("selected", true);
    flash.insert("holdProgress", 0);
    controls[0] = flash;
    state.insert("controls", controls);
    bridge.insert("state", state);
    advance_scene(35);
    flash.insert("pressed", false);
    flash.insert("selected", false);
    flash.insert("holdProgress", -1);
    controls[0] = flash;
    state.insert("controls", controls);
    bridge.insert("state", state);
    advance_scene(5);
    for (QQuickItem *mark : tile->findChildren<QQuickItem *>("animated-mark"))
        if (std::abs(mark->rotation()) > 0.001)
            return 22;
    advance_scene(300);

    flash.insert("pressed", true);
    flash.insert("holdProgress", 250);
    controls[0] = flash;
    state.insert("controls", controls);
    state.insert("link", 0);
    bridge.insert("state", state);
    advance_scene(20);
    std::printf("disconnect: pressed=%d countdown-visible=%d opacity=%.3f\n",
                tile && tile->property("pressed").toBool(), remaining->isVisible(), tile ? tile->opacity() : -1);
    if (!tile || tile->property("pressed").toBool() || remaining->isVisible() ||
        std::abs(tile->opacity() - 0.6) > 0.001)
        return 15;
    std::puts("motion: persistent tiles, interpolated selection, immediate press, real countdown, cap, release and disconnect passed");
    return 0;
}
