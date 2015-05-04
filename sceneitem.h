#ifndef SCENEITEM_H
#define SCENEITEM_H

#include "questitem.h"

class QuestScene;

class ActItem;

class SceneItem : public QuestItem
{
    friend class ItemCreator;

    QuestScene* m_scene;
    SceneItem(QuestScene* scene, ActItem *parent = 0);

public:
    QuestScene* scene()
    {
        return m_scene;
    }
};

#endif // SCENEITEM_H
