#pragma once

#include <QObject>

class LogAllEventsFilter : public QObject
{
    Q_OBJECT
public:
    explicit LogAllEventsFilter(const QString& name, QObject *parent = nullptr);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QString _name;
};

