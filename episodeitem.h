#ifndef EPISODEITEM_H
#define EPISODEITEM_H

#include "questitem.h"

class EpisodeItem : public QuestItem
{
    friend class ItemCreator;

    explicit EpisodeItem(QString title);

public:

    QVariant toJson(bool *ok = 0);
};

#endif // EPISODEITEM_H
