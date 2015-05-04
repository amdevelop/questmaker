#include "actitem.h"

int ActItem::act_counter = 0;

ActItem::ActItem(EpisodeItem *parent)
    : QuestItem(QuestItem::TypeAct, (QTreeWidgetItem*)parent)
{
    m_id = act_counter++;
}
