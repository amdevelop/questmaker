#ifndef SUBJECTITEM_H
#define SUBJECTITEM_H

#include "itemitem.h"

class SubjectItem : public ItemItem
{
    friend class ItemCreator;

    SubjectItem(QString title, QPolygonF polygon = QPolygonF());
public:
};

#endif // SUBJECTITEM_H
