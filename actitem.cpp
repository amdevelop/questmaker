#include "actitem.h"

int ActItem::act_counter = 0;

ActItem::ActItem(QString title)
    : QuestItem(QuestItem::TypeAct, title)
{
    m_id = act_counter++;
}

QVariant ActItem::toJson(bool *)
{
        QVariantMap act_map = properties();

        QVariantList scenes_list;
        for(int i = 0 ; i < rowCount(); i++)
        {
            QuestItem *q_item = (QuestItem*)child(i);
            scenes_list << q_item->toJson();
        }

        act_map.insert("scenes", scenes_list);

        return act_map;
}

