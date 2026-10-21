#ifndef DESK_AGENT_CONTROL_H
#define DESK_AGENT_CONTROL_H

#include <QByteArray>
#include <QObject>
#include <QSocketNotifier>

class DeskBridge;
class QQuickWindow;

class DeskAgentControl final : public QObject {
public:
    DeskAgentControl(DeskBridge *bridge, QQuickWindow *window, QObject *parent = nullptr);

private:
    void readInput();
    void execute(const QByteArray &line);

    DeskBridge *bridge_;
    QQuickWindow *window_;
    QSocketNotifier input_;
    QByteArray pending_;
    bool tap_pending_ = false;
};

#endif
