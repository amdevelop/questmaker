#include "questitemnotifier.h"

QuestItemNotifier::QuestItemNotifier(QObject *parent) :
    QObject(parent)
{
}

void QuestItemNotifier::emitPropertyChanged()
{
    emit propertyChanged();
}

