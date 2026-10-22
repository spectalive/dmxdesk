#include "desk_layout_audit.h"

#include <QHash>
#include <QList>
#include <QQuickItem>
#include <QRectF>

#include <cmath>
#include <cstdio>

#include "desk_effects_layout_audit.h"

int desk_layout_audit(QQuickItem *root) {
    QList<QQuickItem *> pending;
    if (root)
        pending.append(root);
    QList<QQuickItem *> controls;
    QHash<QString, QList<QQuickItem *>> groups;
    QHash<QString, QQuickItem *> panels;
    int findings = 0;
    while (!pending.isEmpty()) {
        QQuickItem *item = pending.takeLast();
        if (!item->isVisible() || item->opacity() == 0)
            continue;
        pending.append(item->childItems());
        const QString name = item->objectName();
        if (name.startsWith("layout-control:")) {
            controls.append(item);
            groups[name.mid(15)].append(item);
        } else if (name.startsWith("layout-panel:")) {
            panels.insert(name.mid(13), item);
        }
    }
    if (controls.isEmpty()) {
        std::puts("@AGENT LAYOUT_ERROR no-controls");
        findings++;
    }
    const QRectF viewport = root ? root->mapRectToScene(root->boundingRect()) : QRectF();
    for (qsizetype i = 0; i < controls.size(); i++) {
        const QRectF rect = controls[i]->mapRectToScene(controls[i]->boundingRect());
        if (!std::isfinite(rect.x()) || !std::isfinite(rect.y()) ||
            !std::isfinite(rect.width()) || !std::isfinite(rect.height()) ||
            rect.width() <= 0 || rect.height() <= 0 || !viewport.contains(rect)) {
            std::printf("@AGENT LAYOUT_ERROR bounds %lld\n", static_cast<long long>(i));
            findings++;
        }
        for (qsizetype j = i + 1; j < controls.size(); j++) {
            const QRectF other = controls[j]->mapRectToScene(controls[j]->boundingRect());
            const QRectF overlap = rect.intersected(other);
            if (overlap.width() > 1 && overlap.height() > 1) {
                std::printf("@AGENT LAYOUT_ERROR overlap %lld %lld %.1fx%.1f\n",
                            static_cast<long long>(i), static_cast<long long>(j),
                            overlap.width(), overlap.height());
                findings++;
            }
        }
    }
    for (auto it = panels.cbegin(); it != panels.cend(); ++it)
        findings += desk_effects_layout_audit(it.value(), groups.value(it.key()));
    std::printf("@AGENT LAYOUT %d\n", findings);
    std::fflush(stdout);
    return findings;
}
