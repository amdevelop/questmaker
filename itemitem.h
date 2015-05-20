#ifndef ITEMITEM_H
#define ITEMITEM_H

#include "questitem.h"

class SceneItem;

class ItemItem : public QuestItem
{
    friend class ItemCreator;

    static int item_counter;
    int m_id;

    ItemItem(QString title, QPolygonF polygon = QPolygonF());

    QPolygonF m_polygon;

public:
    QPolygonF polygon() const
    {
        return m_polygon;
    }

    QPolygonF drawPolygon(int w, int h) const;

    int id() const
    {
        return m_id;
    }

    QVariant toJson(QString save_path, bool *);
};

#endif // ITEMITEM_H
