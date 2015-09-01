#ifndef INTERIORITEM_H
#define INTERIORITEM_H

#include "itemitem.h"

class InteriorItem : public ItemItem
{
public:
    explicit InteriorItem(QString title, QPolygonF polygon = QPolygonF());
    
};

#endif // INTERIORITEM_H
