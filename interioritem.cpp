#include "interioritem.h"

InteriorItem::InteriorItem(QString title, QPolygonF polygon) :
    ItemItem(
        (title.isEmpty())?"Interior" + QString::number(ItemItem::getItemCounter()):title,
        polygon,
        QuestItem::TypeInterior)
{
    setProperty("scene_x", 0);
    setProperty("scene_y", 0);
}
