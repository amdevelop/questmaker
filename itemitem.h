#ifndef ITEMITEM_H
#define ITEMITEM_H

#include "questitem.h"

class SceneItem;

class ItemItem : public QuestItem
{
    friend class ItemCreator;

    ItemItem(QString title, QPolygon polygon = QPolygon());

    QPolygon m_polygon;

public:
    QPolygon polygon() const
    {
        return m_polygon;
    }

    QVariant toJson(bool *);
};

#endif // ITEMITEM_H
