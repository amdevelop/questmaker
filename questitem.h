#ifndef QUESTITEM_H
#define QUESTITEM_H

#include <QStandardItem>

#include "questitemnotifier.h"

class QuestItem : public QStandardItem
{
public:
    enum ItemType
    {
        TypeEpisode,
        TypeAct,
        TypeScene,

        TypeItemBackGround,
        TypeItemItem
    };

    QuestItemNotifier *m_notifier;

private:
    ItemType m_type;

    QMap<QString, QVariant> m_properties;

public:
    QuestItem(ItemType t, QString);
    ~QuestItem();

    virtual QVariant toJson(bool *ok = 0);

    ItemType itemType() const
    {
        return m_type;
    }

    void setProperty(QString, QVariant);
    QVariant property(QString, bool *ok = 0);

    QMap<QString, QVariant> properties()
    {
        return m_properties;
    }

    QuestItemNotifier* notifier()
    {
        return m_notifier;
    }
};

#endif // QUESTITEM_H
