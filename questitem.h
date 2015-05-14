#ifndef QUESTITEM_H
#define QUESTITEM_H

#include <QStandardItem>

#include "questitemnotifier.h"

class QuestItem : public QStandardItem
{
public:
    enum ItemType
    {
        TypeEpisode = 0,
        TypeAct,
        TypeScene,

        TypeItemBackGround,
        TypeItemItem
    };

    enum ItemValueType
    {
        TypeValueNoType = 0,
        TypeValueFileType
    };

    enum QuestRoles
    {
        RoleEditType = Qt::UserRole + 1
    };

    QuestItemNotifier *m_notifier;

private:
    ItemType m_type;

    QMap<QString, QStandardItem*> m_properties;

    QStandardItemModel m_property_model;

public:
    QuestItem(ItemType t, QString);
    ~QuestItem();

    virtual QVariant toJson(QString save_path,
                            bool *ok = 0);

    ItemType itemType() const
    {
        return m_type;
    }

    void setProperty(QString, QVariant = QVariant(), ItemValueType valueType = TypeValueNoType);
    QVariant property(QString, bool *ok = 0);

    QMap<QString, QVariant> properties();

    QStandardItemModel* propertyModel()
    {
        return &m_property_model;
    }

    QuestItemNotifier* notifier()
    {
        return m_notifier;
    }
};

#endif // QUESTITEM_H
