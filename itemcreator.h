#ifndef ITEMCREATOR_H
#define ITEMCREATOR_H

#include <QObject>

class QuestScene;

class QuestItem;

class EpisodeItem;
class ActItem;
class SceneItem;
class ItemItem;
class ItemBackground;

class PropertyWidget;

class ItemCreator : public QObject
{
    Q_OBJECT

    PropertyWidget *m_propertyWidget;

    void connectItem(QuestItem*);
public:
    explicit ItemCreator(QObject *parent = 0);

    void setPropetryWidget(PropertyWidget* pw)
    {
        m_propertyWidget = pw;
    }

    ActItem* createActItem(EpisodeItem*);
    SceneItem* createSceneItem(ActItem*, QuestScene*);
    ItemItem* createItemItem(SceneItem*, QString);

};

#endif // ITEMCREATOR_H
