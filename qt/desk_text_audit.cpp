#include "desk_text_audit.h"

#include <QList>
#include <QQuickItem>

#include <cstdio>

int desk_text_audit(QQuickItem *root) {
    if (!root)
        return 0;
    QList<QQuickItem *> pending{root};
    int findings = 0;
    while (!pending.isEmpty()) {
        QQuickItem *item = pending.takeLast();
        if (!item->isVisible())
            continue;
        pending.append(item->childItems());
        if (item->metaObject()->indexOfProperty("text") < 0 ||
            item->metaObject()->indexOfProperty("contentWidth") < 0)
            continue;
        const bool truncated = item->property("truncated").toBool();
        const bool wide = item->property("contentWidth").toDouble() > item->width() + 1;
        const bool tall = item->property("contentHeight").toDouble() > item->height() + 1;
        if (!truncated && !wide && !tall)
            continue;
        const QPointF position = item->mapToScene(QPointF(0, 0));
        std::printf("@AGENT OVERFLOW %.0f %.0f %.0f %.0f %d %d %d\n",
                    position.x(), position.y(), item->width(), item->height(),
                    truncated, wide, tall);
        findings++;
    }
    std::printf("@AGENT AUDIT %d\n", findings);
    std::fflush(stdout);
    return findings;
}
