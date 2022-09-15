#pragma once

#include <IWorker.h>
#include <QObject>

class Worker : public IWorker
{
    Q_OBJECT
public:
    explicit Worker(const int& id, QObject *parent = nullptr);

public slots:
    void doWork(const int id, const QVariant &parameter) override;

private:
    bool _started = false;
};
