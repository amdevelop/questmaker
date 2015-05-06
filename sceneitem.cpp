#include "sceneitem.h"

SceneItem::SceneItem(QuestScene* scene, QString title) :
    QuestItem(QuestItem::TypeScene, title)
{
    m_scene = scene;

    setProperty("background", QVariant());
    setProperty("order", QVariant());
    setProperty("in_effect", QVariant());
    setProperty("out_effect", QVariant());
}

QVariant SceneItem::toJson(bool *)
{
    QVariantMap scene_map = properties();
    QVariantList item_list;

    for(int i = 0 ; i < rowCount(); i++)
    {
        QuestItem *q_item = (QuestItem*)child(i);
        item_list << q_item->toJson();
    }

    scene_map.insert("items", item_list);

    return scene_map;
}
