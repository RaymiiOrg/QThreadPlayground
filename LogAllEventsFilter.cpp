#include "LogAllEventsFilter.h"

#include <QEvent>
#include <QDebug>

#ifdef LOG_METACALLS
#include <qmetaobject.h>
#include <private/qmetaobject_p.h>
#include <private/qobject_p.h>
#endif


LogAllEventsFilter::LogAllEventsFilter(const QString& name, QObject *parent)
    : QObject{parent}, _name(name)
{

}

bool LogAllEventsFilter::eventFilter(QObject *receiver, QEvent *event) {
    qDebug() << _name << " event \"" << event->type() << "\"";

    #ifdef LOG_METACALLS
    if(event->type() == QEvent::MetaCall) {
        QMetaCallEvent *metaCallEvent = dynamic_cast<QMetaCallEvent *>(event);
        if (metaCallEvent) {
            QString receiverClassName = receiver->metaObject()->className();
            QString signalSignature;
            const auto sender = metaCallEvent->sender();
            const auto senderMeta = sender->metaObject();
            for (auto i = senderMeta->methodOffset(); i < senderMeta->methodCount(); ++i) {
                const auto method = senderMeta->method(i);
                if (QMetaObjectPrivate::signalIndex(method) == metaCallEvent->signalId()) {
                    signalSignature = QString::fromUtf8(method.methodSignature());
                }
            }
            QString senderClassName = senderMeta->className();

            qDebug() << "slot invocation in class "
                     << receiverClassName
                     << " after emitting signal " << signalSignature
                     << " from sender " << senderClassName;
        }
    }
#endif

    return QObject::eventFilter(receiver, event);
}
