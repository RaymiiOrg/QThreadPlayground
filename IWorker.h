#pragma once

#include <QObject>

class IWorker : public QObject
{
    Q_OBJECT
public:
    explicit IWorker(const int& id, QObject *parent = nullptr) : QObject(parent), _id(id) {};
    virtual ~IWorker() {};

public slots:
    virtual void doWork(const int id, const QVariant &parameter) = 0;

signals:
    void resultReady(const int id, const QVariant &result);

protected:
    int getId() { return _id; }

private:
    int _id;
};

