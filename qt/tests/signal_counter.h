#pragma once

#include <QObject>

// Counts how many times the signals connected to it fire.
class SignalCounter final : public QObject {
    Q_OBJECT
public:
    int count = 0;
public slots:
    void hit() { ++count; }
};
