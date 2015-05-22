#include "episodeitem.h"
#include <QDir>

int EpisodeItem::episode_counter = 0;

EpisodeItem::EpisodeItem(QString title) :
    QuestItem(QuestItem::TypeEpisode, title)
{
    m_id = episode_counter++;

    setProperty("cover", QVariant(), QuestItem::TypeValueFileType);
    setProperty("description", QVariant());
}

QVariant EpisodeItem::toJson(QString save_path,
                             bool *)
{
    QVariantMap all_map;
    QVariantMap episode_map = properties();
    QVariantList act_list;


    QFileInfo copy_fi(property("cover").toString());
    QFile::copy(property("cover").toString(),
                save_path + "/" + copy_fi.fileName());

    // DIRTY hack!!!
    episode_map.insert("cover", copy_fi.fileName());

    for(int i = 0 ; i < rowCount(); i++)
    {
        QuestItem *q_item = (QuestItem*)child(i);
        act_list << q_item->toJson(save_path);
    }

    episode_map.insert("id", m_id);
    episode_map.insert("acts", act_list);

    all_map.insert("episode", episode_map);
    return all_map;
}
