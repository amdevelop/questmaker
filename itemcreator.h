#ifndef ITEMCREATOR_H
#define ITEMCREATOR_H

#include <QStandardItemModel>

class QuestScene;

class QuestItem;

class EpisodeItem;
class ActItem;
class SceneItem;
class ItemItem;
class ItemBackground;

class PropertyWidget;

class ItemCreator : public QStandardItemModel
{
    Q_OBJECT

    EpisodeItem* m_episode;

    PropertyWidget *m_propertyWidget;

    void connectItem(QuestItem*);
public:
    explicit ItemCreator(QObject *parent = 0);

    QVariant toJson() const;

    void setPropetryWidget(PropertyWidget* pw)
    {
        m_propertyWidget = pw;
    }

    ActItem* createActItem();
    SceneItem* createSceneItem(ActItem*, QuestScene*);
    ItemItem* createItemItem(SceneItem*, QString, QPolygonF polygon);

};

#endif // ITEMCREATOR_H
