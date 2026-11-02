#include "scene_items.h"

#include <QQuickItem>

QList<QQuickItem *> scene_items(QQuickItem *root) {
    QList<QQuickItem *> items{root};
    for (QQuickItem *child : root->childItems())
        items += scene_items(child);
    return items;
}
