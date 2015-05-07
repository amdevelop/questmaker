#ifndef SCENEITEM_H
#define SCENEITEM_H

#include "questitem.h"

class QuestScene;

class ActItem;

class SceneItem : public QuestItem
{
    friend class ItemCreator;

    int m_id;
    static int scene_counter;

    QuestScene* m_scene;
    SceneItem(QuestScene* scene, QString title);

public:
    QuestScene* scene()
    {
        return m_scene;
    }

    int id() const
    {
        return m_id;
    }

    QVariant toJson(bool *ok);
};

#endif // SCENEITEM_H
