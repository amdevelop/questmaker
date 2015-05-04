#ifndef ITEMITEM_H
#define ITEMITEM_H

#include "questitem.h"

class SceneItem;

class ItemItem : public QuestItem
{
    friend class ItemCreator;

    ItemItem(SceneItem* parent);
};

#endif // ITEMITEM_H
