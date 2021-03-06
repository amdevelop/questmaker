#include "itemitem.h"

#include <QDebug>

int ItemItem::item_counter = 0;

ItemItem::ItemItem(QString title, QPolygonF polygon, ItemType type)
    : QuestItem(type, title)
{
    m_id = item_counter++;
    m_polygon = polygon;
}

QVariant ItemItem::toJson(QString, bool *)
{
//    QVariantMap item_map = properties();

//    QString new_dir_path = save_path + "/" + QString::number(m_id);
//    QDir dir(new_dir_path);
//    if(!dir.exists())
//    {
//        if(dir.mkdir(new_dir_path))
//        {
//            QFileInfo copy_fi(property("detail").toString());
//            QFile::copy(property("detail").toString(),
//                        new_dir_path + "/" + copy_fi.fileName());

//            // DIRTY hack!
//            item_map.insert("detail", copy_fi.fileName());

            QVariantList polygon_list;
            foreach (QPointF point, m_polygon) {

                QVariantList point_list;
                point_list << point.x() << point.y();
                polygon_list << QVariant(point_list);
            }


//        }
//    }

    return polygon_list;
}

QPolygonF ItemItem::drawPolygon(int w, int h) const
{
    QPolygonF draw_polygon;

    foreach (QPointF point, m_polygon) {
        draw_polygon << QPointF(point.x() * w,
                                point.y() * h);
    }

    return draw_polygon;
}
