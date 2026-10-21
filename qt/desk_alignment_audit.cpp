#include "desk_alignment_audit.h"

#include <QHash>
#include <QList>
#include <QPointF>
#include <QQuickItem>
#include <QStringList>

#include <cmath>
#include <cstdio>

int desk_alignment_audit(QQuickItem *root) {
    if (!root)
        return 0;

    QHash<QString, QList<qreal>> left_groups;
    QHash<QString, QList<qreal>> center_groups;
    QHash<QString, QList<QPointF>> mirror_groups;
    QHash<QString, qreal> mirror_parent_centers;
    QList<QQuickItem *> pending{root};
    int findings = 0;

    while (!pending.isEmpty()) {
        QQuickItem *item = pending.takeLast();
        if (!item->isVisible())
            continue;
        pending.append(item->childItems());
        for (const QString &tag : item->objectName().split(';', Qt::SkipEmptyParts)) {
            if (tag == "align-x-parent") {
                QQuickItem *parent = item->parentItem();
                if (!parent)
                    continue;
                const qreal actual = item->mapToScene(QPointF(item->width() / 2, 0)).x();
                const qreal expected = parent->mapToScene(QPointF(parent->width() / 2, 0)).x();
                if (std::abs(actual - expected) > 1.5) {
                    std::printf("@AGENT ALIGN %s %.1f\n", tag.toUtf8().constData(), actual - expected);
                    findings++;
                }
            } else if (tag.startsWith("align-left:")) {
                left_groups[tag].append(item->mapToScene(QPointF(0, 0)).x());
            } else if (tag.startsWith("align-center:")) {
                center_groups[tag].append(item->mapToScene(QPointF(item->width() / 2, 0)).x());
            } else if (tag.startsWith("align-mirror:")) {
                QQuickItem *parent = item->parentItem();
                if (!parent)
                    continue;
                mirror_groups[tag].append(item->mapToScene(QPointF(item->width() / 2, item->height() / 2)));
                mirror_parent_centers[tag] = parent->mapToScene(QPointF(parent->width() / 2, 0)).x();
            }
        }
    }

    for (auto it = left_groups.cbegin(); it != left_groups.cend(); ++it) {
        const QList<qreal> &positions = it.value();
        if (positions.size() < 2) {
            std::printf("@AGENT ALIGN %s missing-peer\n", it.key().toUtf8().constData());
            findings++;
            continue;
        }
        for (qreal position : positions) {
            if (std::abs(position - positions.first()) <= 1.5)
                continue;
            std::printf("@AGENT ALIGN %s %.1f\n", it.key().toUtf8().constData(), position - positions.first());
            findings++;
        }
    }
    for (auto it = center_groups.cbegin(); it != center_groups.cend(); ++it) {
        const QList<qreal> &positions = it.value();
        if (positions.size() < 2) {
            std::printf("@AGENT ALIGN %s missing-peer\n", it.key().toUtf8().constData());
            findings++;
            continue;
        }
        for (qreal position : positions) {
            if (std::abs(position - positions.first()) <= 1.5)
                continue;
            std::printf("@AGENT ALIGN %s %.1f\n", it.key().toUtf8().constData(), position - positions.first());
            findings++;
        }
    }
    for (auto it = mirror_groups.cbegin(); it != mirror_groups.cend(); ++it) {
        const QList<QPointF> &centers = it.value();
        if (centers.size() != 2) {
            std::printf("@AGENT ALIGN %s expected-two-icons\n", it.key().toUtf8().constData());
            findings++;
            continue;
        }
        const qreal actual = (centers[0].x() + centers[1].x()) / 2;
        const qreal expected = mirror_parent_centers.value(it.key());
        if (std::abs(actual - expected) > 1.5 || std::abs(centers[0].y() - centers[1].y()) > 1.5) {
            std::printf("@AGENT ALIGN %s %.1f\n", it.key().toUtf8().constData(), actual - expected);
            findings++;
        }
    }
    std::printf("@AGENT ALIGNMENT %d\n", findings);
    std::fflush(stdout);
    return findings;
}
