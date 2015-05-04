#ifndef QUESTITEMNOTIFIER_H
#define QUESTITEMNOTIFIER_H

#include <QObject>

class QuestItemNotifier : public QObject
{
    Q_OBJECT

    friend class QuestItem;

    void emitPropertyChanged();
    explicit QuestItemNotifier(QObject *parent = 0);

signals:

    void propertyChanged();


};

#endif // QUESTITEMNOTIFIER_H
