#ifndef DESK_BRIDGE_H
#define DESK_BRIDGE_H

#include <QByteArray>
#include <QElapsedTimer>
#include <QObject>
#include <QProcess>
#include <QVariantMap>

class DeskBridge final : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantMap state READ state NOTIFY stateChanged)
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)

public:
    explicit DeskBridge(const QString &backend, const QString &map,
                        const QString &power = {}, QObject *parent = nullptr);
    ~DeskBridge() override;
    QVariantMap state() const;
    bool running() const;
    Q_INVOKABLE void touch(const QString &kind, int slot, int x, int y);

signals:
    void stateChanged();
    void runningChanged();
    void failed();

private:
    void readOutput();
    QProcess process_;
    QByteArray output_;
    QVariantMap state_;
    QElapsedTimer clock_;
};

#endif
