#include "questitem.h"

QuestItem::QuestItem(ItemType t, QTreeWidgetItem *parent)
    : QTreeWidgetItem(parent)
{
    m_type = t;
    m_notifier = new QuestItemNotifier;

    setProperty("title", QVariant());
}

QuestItem::~QuestItem()
{
    delete m_notifier;
}

void QuestItem::setProperty(QString name, QVariant value)
{
    m_properties.insert(name, value);

    m_notifier->emitPropertyChanged();
}

QVariant QuestItem::property(QString name, bool *ok)
{
    if(ok)
        *ok = m_properties.contains(name);

    return m_properties.value(name);
}
