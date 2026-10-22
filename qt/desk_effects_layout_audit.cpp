#include "desk_effects_layout_audit.h"

#include <QQuickItem>
#include <QRectF>

#include <algorithm>
#include <cmath>
#include <cstdio>

int desk_effects_layout_audit(QQuickItem *panel, QList<QQuickItem *> pads) {
    const int count = panel->property("auditCount").toInt();
    const int columns = panel->property("auditColumns").toInt();
    if (count < 2 || columns < 1 || pads.size() != count || (count - 1) % columns != 0) {
        std::printf("@AGENT LAYOUT_ERROR effects-count %lld expected %d\n",
                    static_cast<long long>(pads.size()), count);
        return 1;
    }
    std::sort(pads.begin(), pads.end(), [](QQuickItem *a, QQuickItem *b) {
        const QPointF first = a->mapToScene(QPointF());
        const QPointF second = b->mapToScene(QPointF());
        return first.y() == second.y() ? first.x() < second.x() : first.y() < second.y();
    });
    const qreal gap = panel->property("auditGap").toDouble();
    const qreal inset = panel->property("auditInset").toDouble();
    const qreal header = panel->property("auditHeader").toDouble();
    const QRectF area = panel->mapRectToScene(panel->boundingRect());
    const QRectF inner = area.adjusted(inset, header, -inset, -inset);
    const qreal height = pads.first()->mapRectToScene(pads.first()->boundingRect()).height();
    const qreal width = (inner.width() - (columns - 1) * gap) / columns;
    int findings = 0;
    for (int i = 0; i < count - 1; i++) {
        const QRectF rect = pads[i]->mapRectToScene(pads[i]->boundingRect());
        const int column = i % columns;
        const int row = i / columns;
        if (std::abs(rect.width() - width) > 1 || std::abs(rect.height() - height) > 1 ||
            std::abs(rect.left() - (inner.left() + column * (width + gap))) > 1 ||
            std::abs(rect.top() - (inner.top() + row * (height + gap))) > 1) {
            std::printf("@AGENT LAYOUT_ERROR effects-grid row %d column %d\n", row, column);
            findings++;
        }
    }
    const QRectF wide = pads.last()->mapRectToScene(pads.last()->boundingRect());
    const int rows = (count - 1) / columns;
    if (std::abs(wide.left() - inner.left()) > 1 ||
        std::abs(wide.right() - inner.right()) > 1 ||
        std::abs(wide.bottom() - inner.bottom()) > 1 ||
        std::abs(wide.top() - (inner.top() + rows * (height + gap))) > 1) {
        std::puts("@AGENT LAYOUT_ERROR effects-wide-padding-or-gap");
        findings++;
    }
    return findings;
}
