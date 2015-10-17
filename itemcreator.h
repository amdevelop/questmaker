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

class InteriorItem;
class SubjectItem;

class ItemBackground;

class PropertyWidget;

class ItemCreator : public QStandardItemModel
{
    Q_OBJECT

    EpisodeItem* m_episode;
   // QItemSelectionModel m_selection_model;

public:
    explicit ItemCreator(QObject *parent = 0);

    QVariant toJson(QString save_path) const;

    EpisodeItem* episodeItem()
    {
        return m_episode;
    }

    QItemSelectionModel* selectionModel()
    {
        return 0; // &m_selection_model;
    }

    ActItem* createActItem();
    SceneItem* createSceneItem(ActItem*, QuestScene*);
    ItemItem* createItemItem(SubjectItem*, QString, QPolygonF polygon);

    SubjectItem* createSubjectItem(SceneItem* parent,
                                   QString file_path,
                                   qreal scene_w,
                                   qreal scene_h,
                                   QString title = QString());

    InteriorItem* createInteriorItem(SceneItem*,
                                     QString file_path,
                                     qreal scene_w,
                                     qreal scene_h,
                                     QString title = QString());


    void removeItem(QuestItem*);

signals:
    void sceneRemoved(SceneItem*);

};

#endif // ITEMCREATOR_H
