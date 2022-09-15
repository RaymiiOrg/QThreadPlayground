#pragma once

#include <QObject>

class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString result READ result WRITE setResult NOTIFY resultChanged)
    Q_PROPERTY(int runningThreadCount READ runningThreadCount WRITE setRunningThreadCount NOTIFY runningThreadCountChanged)

public:
    explicit Controller(QObject* parent = nullptr);

    //Q_PROPERTIES
    const QString &result() const;
    void appendResult(const QString& newResult);
    int runningThreadCount() const;

public slots:
    // requests thread from QML
    void makeThread();

    // handles result from worker thread
    void handleResults(const int id, const QVariant &result);

    // Q_PROPERTY setters
    void setResult(const QString &newResult);
    void setRunningThreadCount(int newRunningThreadCount);

signals:
    // signals workers to start work in worker thread
    void operate(const int id, const QVariant &param);

    //Q_PROPERTY signals
    void resultChanged(const QString &result);
    void runningThreadCountChanged(int runningThreadCount);

private:
    std::map<int, QThread*> workerThreads;
    int _tidIncrement = 0;

    bool workerThreadExistsAndValid(const int id);
    void stopAndCleanupWorkerThread(const int id);

    //Q_PROPERTIES
    QString _result;
    int _runningThreadCount = 0;
};
