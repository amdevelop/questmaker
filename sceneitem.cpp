#include "sceneitem.h"

#include <QDir>

int SceneItem::scene_counter = 0;

SceneItem::SceneItem(QuestScene* scene, QString title) :
    QuestItem(QuestItem::TypeScene, title)
{
    m_id = scene_counter++;

    m_scene = scene;

    setProperty("description", QVariant(), QuestItem::TypeValueText);
    setProperty("background", QVariant(), QuestItem::TypeValueImage);
//    setProperty("order", QVariant());
//    setProperty("in_effect", QVariant());
//    setProperty("out_effect", QVariant());
}

QVariant SceneItem::toJson(QString save_path,
                           bool *)
{
    QVariantMap scene_map = properties();
    QVariantList item_list;

    QString new_dir_path = save_path + "/" + QString::number(m_id);
    QDir dir(new_dir_path);
    if(!dir.exists())
    {
        if(dir.mkdir(new_dir_path))
        {
            QFileInfo copy_fi(property("background").toString());
            QFile::copy(property("background").toString(),
                        new_dir_path + "/" + copy_fi.fileName());

            // DIRTY hack!!!
            scene_map.insert("background", copy_fi.fileName());

            for(int i = 0 ; i < rowCount(); i++)
            {
                QuestItem *q_item = (QuestItem*)child(i);
                item_list << q_item->toJson(new_dir_path);
            }

            scene_map.insert("id", m_id);
            scene_map.insert("items", item_list);
        }
    }
    return scene_map;
}
