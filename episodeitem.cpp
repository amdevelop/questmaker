#include "episodeitem.h"

EpisodeItem::EpisodeItem(QString title) :
    QuestItem(QuestItem::TypeEpisode, title)
{
}

QVariant EpisodeItem::toJson(bool *)
{
    QVariantMap all_map;
    QVariantMap episode_map = properties();
    QVariantList act_list;

    for(int i = 0 ; i < rowCount(); i++)
    {
        QuestItem *q_item = (QuestItem*)child(i);
        act_list << q_item->toJson();
    }

    episode_map.insert("acts", act_list);

    all_map.insert("episode", episode_map);
    return all_map;
}
