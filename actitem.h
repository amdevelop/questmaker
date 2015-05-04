#ifndef ACTITEM_H
#define ACTITEM_H

#include "questitem.h"

class EpisodeItem;

class ActItem : public QuestItem
{
    friend class ItemCreator;

    static int act_counter;
    int m_id;
    ActItem(EpisodeItem *parent);

public:

    int id() const
    {
    return m_id;
    }
};

#endif // ACTITEM_H
