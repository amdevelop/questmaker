#include "actitem.h"

#include <QDir>

int ActItem::act_counter = 0;

ActItem::ActItem(QString title)
    : QuestItem(QuestItem::TypeAct, title)
{
    m_id = act_counter++;

    setProperty("description", QVariant());
}

QVariant ActItem::toJson(QString save_path,
                         bool*)
{
    QVariantMap act_map = properties();

    QString new_dir_path = save_path + "/" + QString::number(m_id);
    QDir dir(new_dir_path);
    if(!dir.exists())
    {
        if(dir.mkdir(new_dir_path))
        {
            QVariantList scenes_list;
            for(int i = 0 ; i < rowCount(); i++)
            {
                QuestItem *q_item = (QuestItem*)child(i);
                scenes_list << q_item->toJson(new_dir_path);
            }

            act_map.insert("scenes", scenes_list);
            act_map.insert("id", m_id);
        }
    }
    return act_map;
}

