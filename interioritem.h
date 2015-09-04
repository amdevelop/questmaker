#ifndef INTERIORITEM_H
#define INTERIORITEM_H

#include "questitem.h"


class InteriorItem : public QuestItem
{
    static int interior_counter;
public:
    explicit InteriorItem(QString title,
                          QuestItem::ItemType type = QuestItem::TypeInterior);

    static int getItemCounter()
    {
        return interior_counter++;
    }
};

#endif // INTERIORITEM_H
