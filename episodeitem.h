#ifndef EPISODEITEM_H
#define EPISODEITEM_H

#include "questitem.h"

class EpisodeItem : public QuestItem
{
    friend class ItemCreator;

    static int episode_counter;
    int m_id;

    explicit EpisodeItem(QString title);

public:
    int id() const
    {
        return m_id;
    }

    QVariant toJson(QString save_path,
                    bool *ok = 0);
};

#endif // EPISODEITEM_H
