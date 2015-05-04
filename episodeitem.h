#ifndef EPISODEITEM_H
#define EPISODEITEM_H

#include "questitem.h"

class EpisodeItem : public QuestItem
{
public:
    explicit EpisodeItem(QTreeWidgetItem *parent);
};

#endif // EPISODEITEM_H
