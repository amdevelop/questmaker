#include "itemitem.h"

ItemItem::ItemItem(QString title, QPolygon polygon)
    : QuestItem(QuestItem::TypeItemItem, title)
{
    m_polygon = polygon;
}

QVariant ItemItem::toJson(bool *)
{
        QVariantMap item_map = properties();

        QVariantList polygon_list;
        foreach (QPoint point, m_polygon) {

         QVariantList point_list;
         point_list << point.x() << point.y();
         polygon_list << QVariant(point_list);
        }

        item_map.insert("polygon", polygon_list);

        return item_map;
}

