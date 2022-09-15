#include "Worker.h"
#include "LogAllEventsFilter.h"
#include <QDebug>
#include <QThread>
#include <QRandomGenerator>

Worker::Worker(const int& id, QObject *parent)
    : IWorker(id, parent)
{
#ifdef LOG_METACALLS
    auto* evf = new LogAllEventsFilter("worker " + QString::number(id), this);
    installEventFilter(evf);
#endif
}



void Worker::doWork(const int id, const QVariant &parameter) {
    if(id != getId())
        return;

    QString result;
    result = parameter.toString() + QString::number(getId());
    QThread::msleep(5500);
    emit resultReady(getId(), result);
}
