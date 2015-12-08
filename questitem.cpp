#include "questitem.h"

QuestItem::QuestItem(ItemType t, QString title)
    : QStandardItem(title)
{
    m_type = t;
    m_notifier = new QuestItemNotifier;

    m_graphics_id = -1;

    m_property_model.setHorizontalHeaderLabels(
                QStringList()
                << QObject::tr("Key")
                << QObject::tr("Value"));


    setProperty("title", title);
}

QuestItem::~QuestItem()
{
    delete m_notifier;
}

void QuestItem::setProperty(QString name,
                            QVariant value,
                            ItemValueType value_type)
{
    QStandardItem *value_item = 0;

    if(m_properties.contains(name))
        value_item = m_properties.value(name);
    else
    {
        m_property_model.setRowCount(
                    m_property_model.rowCount() + 1
                    );

        QStandardItem *name_item = new QStandardItem;
        name_item->setData(name, Qt::DisplayRole);
        name_item->setData(value_type, QuestItem::RoleEditType);

        value_item = new QStandardItem;

        m_property_model.setItem(
                    m_property_model.rowCount() - 1,
                    0,
                    name_item);

        m_property_model.setItem(
                    m_property_model.rowCount() - 1,
                    1,
                    value_item);

        m_properties.insert(name, value_item);
    }

    value_item->setData(value, Qt::DisplayRole);

    //    m_notifier->emitPropertyChanged();
}

QVariant QuestItem::property(QString name, bool *ok)
{
    QStandardItem *root_item = m_property_model.invisibleRootItem();
    for(int i = 0;
        i < root_item->rowCount();
        i++)
    {
        if(root_item->child(i)->data(Qt::DisplayRole).toString() == name)
        {
            if(ok)
                *ok = true;
            return root_item->child(i, 1)->data(Qt::DisplayRole);
        }
    }

    if(ok)
        *ok = false;

    return QVariant();
}

QMap<QString, QVariant> QuestItem::properties()
{
    QMap<QString, QVariant> ret_map;

    QStandardItem *root_item = m_property_model.invisibleRootItem();
    for(int i = 0;
        i < root_item->rowCount();
        i++)
    {

        ret_map.insert(root_item->child(i)->data(Qt::DisplayRole).toString(),
                       root_item->child(i, 1)->data(Qt::DisplayRole));
    }

    return ret_map;
}

QVariant QuestItem::toJson(QString, bool *ok)
{
    if(ok)
        *ok = false;

    return QVariant();
}

