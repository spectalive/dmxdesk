#include <QFile>
#include <QGuiApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickWindow>

#include <cstdio>

#include "../desk_layout_audit.h"
#include "DeskLayoutTestBridge.h"

int main(int argc, char **argv) {
    QGuiApplication app(argc, argv);
    QFile fixture(QStringLiteral(DESK_TEST_FIXTURE));
    if (!fixture.open(QIODevice::ReadOnly))
        return 1;
    const QVariantMap state = QJsonDocument::fromJson(fixture.readAll()).object().toVariantMap();
    for (int scenario = 0; scenario < 9; scenario++) {
        DeskLayoutTestBridge bridge;
        bridge.insert("state", state);
        QQmlApplicationEngine engine;
        engine.rootContext()->setContextProperty("deskBridge", &bridge);
        engine.load(QUrl::fromLocalFile(QStringLiteral(DESK_TEST_QML)));
        if (engine.rootObjects().size() != 1)
            return 2;
        auto *window = qobject_cast<QQuickWindow *>(engine.rootObjects().first());
        if (!window)
            return 3;
        app.processEvents();
        QList<QQuickItem *> pads;
        for (QQuickItem *item : window->contentItem()->childItems()) {
            if (item->objectName() == "layout-control:direct-effects")
                pads.append(item);
        }
        if (pads.size() != 7)
            return 4;
        // 2026-10-22: the preview mixed legacy rectangles with inset ones.
        if (scenario == 1) {
            for (int i : {0, 1, 3, 4}) {
                pads[i]->setX(i % 3 == 0 ? 540 : 659);
                pads[i]->setY(i < 3 ? 184 : 276);
                pads[i]->setWidth(111);
                pads[i]->setHeight(84);
            }
        }
        if (scenario == 2) pads[6]->setY(342);
        if (scenario == 3) pads[2]->setHeight(84);
        if (scenario == 4) pads[1]->setX(pads[1]->x() + 3);
        if (scenario == 5) pads[5]->setVisible(false);
        if (scenario == 6) pads[2]->setX(1020);
        if (scenario == 7) pads[6]->setWidth(pads[6]->width() - 4);
        if (scenario == 8) pads[1]->setScale(1.1);
        window->grabWindow();
        const int findings = desk_layout_audit(window->contentItem());
        if ((scenario == 0 && findings != 0) || (scenario != 0 && findings == 0)) {
            std::fprintf(stderr, "layout scenario %d: unexpected %d findings\n", scenario, findings);
            return 10 + scenario;
        }
        std::printf("layout scenario %d: %s\n", scenario, scenario == 0 ? "accepted" : "rejected");
    }
    return 0;
}
