#pragma once

#include <QList>

class QQuickItem;

// Every item under root in the visual tree, root included.
QList<QQuickItem *> scene_items(QQuickItem *root);
