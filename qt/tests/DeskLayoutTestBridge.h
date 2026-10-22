#ifndef DESK_LAYOUT_TEST_BRIDGE_H
#define DESK_LAYOUT_TEST_BRIDGE_H

#include <QQmlPropertyMap>

class DeskLayoutTestBridge final : public QQmlPropertyMap {
public:
    explicit DeskLayoutTestBridge(QObject *parent = nullptr) : QQmlPropertyMap(this, parent) {}
};

#endif
