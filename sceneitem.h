#ifndef SCENEITEM_H
#define SCENEITEM_H

#include "questitem.h"

class QuestScene;

class ActItem;

class SceneItem : public QuestItem
{
    friend class ItemCreator;

    QuestScene* m_scene;
    SceneItem(QuestScene* scene, QString title);

public:
    QuestScene* scene()
    {
        return m_scene;
    }

    QVariant toJson(bool *ok);
};

#endif // SCENEITEM_H
