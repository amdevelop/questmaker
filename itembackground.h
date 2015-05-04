#ifndef ITEMBACKGROUND_H
#define ITEMBACKGROUND_H

#include "questitem.h"

class ItemBackground : public QuestItem
{
    friend class ItemCreator;
    ItemBackground(QTreeWidgetItem* parent);
};

#endif // ITEMBACKGROUND_H
