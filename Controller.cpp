#include "Controller.h"
#include "IWorker.h"
#include "Worker.h"
#include "LogAllEventsFilter.h"

#include "singleshot_connect.h"

#include <QThread>
#include <QDebug>

Controller::Controller(QObject *parent)
    : QObject{parent}
{
#ifdef LOG_METACALLS
    auto* evf = new LogAllEventsFilter("controller", this);
    installEventFilter(evf);
#endif
}


void Controller::makeThread()
{
    int tid = ++_tidIncrement;
    workerThreads[tid] = new QThread();

    IWorker *worker = new Worker(tid);
    worker->moveToThread(workerThreads[tid]);

         // When thread is finished (after calling quit)
         // make sure the thread and the worker running
         // in that thread are deleted
    connect(workerThreads[tid], &QThread::finished, worker, &QObject::deleteLater);
    connect(workerThreads[tid], &QThread::finished, workerThreads[tid], &QObject::deleteLater);

         // Example Parameter for Worker to operate on
    QString param = "Test";

         // once controller emits operate signal, worker
         // should start processing. Once the result is
         // ready, the controller should process the results.
    KDToolBox::connectSingleShot(this, &Controller::operate, worker, &IWorker::doWork);
    KDToolBox::connectSingleShot(worker, &IWorker::resultReady, this, &Controller::handleResults);

         // start the thread (not the worker)
    workerThreads[tid]->start();

    appendResult("Running " + QString::number(tid));
    setRunningThreadCount(runningThreadCount()+1);

         // this signal actually starts the worker
    emit operate(tid, param);

    // poor-mans emulate a single shot connection
    // if you do not want to use the kdab single shot
    // remember to disconnect, otherwise all workers
    // will process again.
    // worker->disconnect(this, &Controller::operate, worker, &IWorker::doWork);
}


void Controller::handleResults(const int id, const QVariant &result)
{
    if(!workerThreadExistsAndValid(id))
        return;

    appendResult("Done " + QString::number(id) + ": " + result.toString());
    setRunningThreadCount(runningThreadCount()-1);


    stopAndCleanupWorkerThread(id);
}

const QString &Controller::result() const
{
    return _result;
}

void Controller::appendResult(const QString &newResult)
{
    auto currentResult = result();
    setResult(currentResult + "\n" + newResult);
}

void Controller::setResult(const QString &newResult)
{
    if (_result == newResult)
        return;
    _result = newResult;
    emit resultChanged(_result);
}

int Controller::runningThreadCount() const
{
    return _runningThreadCount;
}

void Controller::setRunningThreadCount(int newRunningThreadCount)
{
    if (_runningThreadCount == newRunningThreadCount)
        return;
    _runningThreadCount = newRunningThreadCount;
    emit runningThreadCountChanged(_runningThreadCount);
}

bool Controller::workerThreadExistsAndValid(const int id)
{
    // check if our std::map of thread* still has
    // this key and if so, it should not be a nullptr
    bool keyExists = (workerThreads.count(id) > 0);
    bool valueIsNullptr = (workerThreads[id] == nullptr);
    return (keyExists || !valueIsNullptr);
}

void Controller::stopAndCleanupWorkerThread(const int id)
{
  // tell the thread to stop, give it some
  // time to process that event and then remove
  // it from our thread* map.
    workerThreads[id]->quit();
    workerThreads[id]->wait(1);
    workerThreads.erase(id);
}
