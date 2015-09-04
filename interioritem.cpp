#include "interioritem.h"

int InteriorItem::interior_counter = 0;

InteriorItem::InteriorItem(QString title, QuestItem::ItemType type) :
    QuestItem(type,
        (title.isEmpty())?"Interior" + QString::number(getItemCounter()):title)
{
    setProperty("scene_x", 0);
    setProperty("scene_y", 0);
}
