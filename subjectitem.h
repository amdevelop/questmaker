#ifndef SUBJECTITEM_H
#define SUBJECTITEM_H

#include "interioritem.h"

class SubjectItem : public InteriorItem
{
    friend class ItemCreator;

protected:
    SubjectItem(QString title = QString());
};

#endif // SUBJECTITEM_H
