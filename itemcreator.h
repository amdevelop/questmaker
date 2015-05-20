#ifndef ITEMCREATOR_H
#define ITEMCREATOR_H

#include <QStandardItemModel>
#include <QItemSelectionModel>

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
    QItemSelectionModel m_selection_model;

public:
    explicit ItemCreator(QObject *parent = 0);

    QVariant toJson(QString save_path) const;

    EpisodeItem* episodeItem()
    {
        return m_episode;
    }

    QItemSelectionModel* selectionModel()
    {
        return &m_selection_model;
    }

    ActItem* createActItem();
    SceneItem* createSceneItem(ActItem*, QuestScene*);
    ItemItem* createItemItem(SceneItem*, QString, QPolygonF polygon);

    void removeItem(QuestItem*);

signals:
    void sceneRemoved(SceneItem*);

};

#endif // ITEMCREATOR_H
