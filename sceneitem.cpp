#include "sceneitem.h"

SceneItem::SceneItem(QuestScene* scene, ActItem *parent) :
    QuestItem(QuestItem::TypeScene, (QTreeWidgetItem*)parent)
{
    m_scene = scene;

    setProperty("background", QVariant());
    setProperty("order", QVariant());
    setProperty("in_effect", QVariant());
    setProperty("out_effect", QVariant());
}
