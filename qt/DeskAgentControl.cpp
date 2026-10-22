#include "DeskAgentControl.h"

#include <QCoreApplication>
#include <QQuickWindow>
#include <QTimer>

#include <cerrno>
#include <cstdio>
#include <unistd.h>

#include "DeskBridge.h"
#include "desk_view_audit.h"

DeskAgentControl::DeskAgentControl(DeskBridge *bridge, QQuickWindow *window,
                                   QObject *parent)
    : QObject(parent), bridge_(bridge), window_(window),
      input_(STDIN_FILENO, QSocketNotifier::Read, this) {
    connect(&input_, &QSocketNotifier::activated, this, [this] { readInput(); });
    std::puts("@AGENT READY");
    std::fflush(stdout);
}

void DeskAgentControl::readInput() {
    char data[1024];
    const ssize_t count = ::read(STDIN_FILENO, data, sizeof data);
    if (count <= 0) {
        if (count == 0 || (errno != EINTR && errno != EAGAIN))
            input_.setEnabled(false);
        return;
    }
    pending_.append(data, count);
    if (pending_.size() > 4096) {
        pending_.clear();
        std::puts("@AGENT ERROR command too long");
    }
    while (true) {
        const qsizetype end = pending_.indexOf('\n');
        if (end < 0)
            break;
        const QByteArray line = pending_.left(end).trimmed();
        pending_.remove(0, end + 1);
        execute(line);
    }
    std::fflush(stdout);
}

void DeskAgentControl::execute(const QByteArray &line) {
    if (line.startsWith("TAP ")) {
        if (tap_pending_) {
            std::puts("@AGENT ERROR tap busy");
            return;
        }
        const QList<QByteArray> parts = line.simplified().split(' ');
        bool valid_x = false;
        bool valid_y = false;
        const int x = parts.value(1).toInt(&valid_x);
        const int y = parts.value(2).toInt(&valid_y);
        if (parts.size() != 3 || !valid_x || !valid_y) {
            std::puts("@AGENT ERROR invalid tap");
            return;
        }
        tap_pending_ = true;
        bridge_->touch(QStringLiteral("D"), 0, x, y);
        QTimer::singleShot(40, this, [this, x, y] {
            bridge_->touch(QStringLiteral("U"), 0, x, y);
            tap_pending_ = false;
            std::puts("@AGENT OK tap");
            std::fflush(stdout);
        });
        return;
    }
    if (line.startsWith("TOUCH ")) {
        const QList<QByteArray> parts = line.simplified().split(' ');
        bool valid_slot = false;
        bool valid_x = false;
        bool valid_y = false;
        const int slot = parts.value(2).toInt(&valid_slot);
        const int x = parts.value(3).toInt(&valid_x);
        const int y = parts.value(4).toInt(&valid_y);
        if (parts.size() != 5 || parts.value(1).size() != 1 ||
            !QByteArray("DMUC").contains(parts.value(1)) ||
            !valid_slot || !valid_x || !valid_y || slot < 0 || slot >= 10) {
            std::puts("@AGENT ERROR invalid touch");
            return;
        }
        bridge_->touch(QString::fromUtf8(parts[1]), slot, x, y);
        std::puts("@AGENT OK touch");
        return;
    }
    if (line.startsWith("CAPTURE ")) {
        const QString path = QString::fromUtf8(line.mid(8));
        const bool saved = window_->grabWindow().save(path);
        const int findings = desk_view_audit(window_->contentItem());
        if (saved && findings == 0)
            std::puts("@AGENT OK capture");
        else if (findings != 0)
            std::puts("@AGENT ERROR capture audit failed");
        else
            std::puts("@AGENT ERROR capture failed");
        return;
    }
    if (line == "AUDIT") {
        desk_view_audit(window_->contentItem());
        return;
    }
    if (line == "QUIT") {
        std::puts("@AGENT OK quit");
        QCoreApplication::quit();
        return;
    }
    std::puts("@AGENT ERROR unknown command");
}
