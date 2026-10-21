#include "DeskBridge.h"

#include <QJsonDocument>
#include <QJsonObject>

DeskBridge::DeskBridge(const QString &backend, const QString &map,
                       const QString &power, QObject *parent)
    : QObject(parent) {
    clock_.start();
    connect(&process_, &QProcess::readyReadStandardOutput, this, &DeskBridge::readOutput);
    connect(&process_, &QProcess::started, this, &DeskBridge::runningChanged);
    connect(&process_, &QProcess::finished, this, [this] {
        state_.clear();
        emit stateChanged();
        emit runningChanged();
        emit failed();
    });
    connect(&process_, &QProcess::errorOccurred, this, [this] { emit failed(); });
    process_.setProgram(backend);
    QStringList arguments = {"--qt-pipe", "--map", map};
    if (!power.isEmpty())
        arguments += {"--power", power};
    process_.setArguments(arguments);
    process_.setProcessChannelMode(QProcess::ForwardedErrorChannel);
    process_.start();
}

DeskBridge::~DeskBridge() {
    process_.closeWriteChannel();
    if (!process_.waitForFinished(500)) {
        process_.terminate();
        process_.waitForFinished(500);
    }
}

QVariantMap DeskBridge::state() const { return state_; }

bool DeskBridge::running() const { return process_.state() == QProcess::Running; }

void DeskBridge::touch(const QString &kind, int slot, int x, int y) {
    if (!running() || kind.size() != 1 || !QStringLiteral("DMUC").contains(kind) ||
        slot < 0 || slot >= 10 || x < -10000 || x > 10000 || y < -10000 || y > 10000)
        return;
    const QByteArray line = QStringLiteral("%1 %2 %3 %4 %5\n")
                                .arg(kind).arg(slot).arg(x).arg(y)
                                .arg(clock_.msecsSinceReference()).toUtf8();
    process_.write(line);
}

void DeskBridge::readOutput() {
    output_ += process_.readAllStandardOutput();
    if (output_.size() > 262144) {
        output_.clear();
        return;
    }
    while (true) {
        const qsizetype end = output_.indexOf('\n');
        if (end < 0)
            break;
        const QByteArray line = output_.left(end);
        output_.remove(0, end + 1);
        if (!line.startsWith("@STATE "))
            continue;
        QJsonParseError error;
        const QJsonDocument json = QJsonDocument::fromJson(line.mid(7), &error);
        if (error.error != QJsonParseError::NoError || !json.isObject())
            continue;
        const QVariantMap next = json.object().toVariantMap();
        if (next != state_) {
            state_ = next;
            emit stateChanged();
        }
    }
}
