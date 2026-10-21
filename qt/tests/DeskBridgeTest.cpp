#include <QCoreApplication>
#include <QElapsedTimer>
#include <QFile>
#include <QTemporaryDir>
#include <QThread>

#include "../DeskBridge.h"

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    QTemporaryDir dir;
    if (!dir.isValid())
        return 1;
    const QString script = dir.filePath("mock-backend.sh");
    const QString record = dir.filePath("touch.txt");
    QFile mock(script);
    if (!mock.open(QIODevice::WriteOnly))
        return 2;
    mock.write("#!/bin/sh\nprintf '@STATE {broken}\\n'\n"
               "printf '@STA'\n"
               "sleep 0.05\n"
               "printf 'TE {\"page\":1,\"link\":3}\\n'\n"
               "IFS= read -r line\nprintf '%s\\n' \"$line\" > \"$3\"\n");
    mock.close();
    mock.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner |
                        QFileDevice::ExeOwner);
    DeskBridge bridge(script, record);
    QElapsedTimer deadline;
    deadline.start();
    while (bridge.state().isEmpty() && deadline.elapsed() < 3000) {
        app.processEvents();
        QThread::msleep(5);
    }
    if (bridge.state().value("page").toInt() != 1 || !bridge.running())
        return 3;
    bridge.touch("X", 1, 42, 43);
    bridge.touch("D", 1, 42, 43);
    QFile result(record);
    while (!result.exists() && deadline.elapsed() < 3000) {
        app.processEvents();
        QThread::msleep(5);
    }
    if (!result.open(QIODevice::ReadOnly))
        return 4;
    const QByteArray line = result.readAll();
    if (!line.startsWith("D 1 42 43 "))
        return 5;
    while (bridge.running() && deadline.elapsed() < 3000) {
        app.processEvents();
        QThread::msleep(5);
    }
    if (bridge.running() || !bridge.state().isEmpty())
        return 6;
    return 0;
}
