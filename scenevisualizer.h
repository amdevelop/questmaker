#ifndef SCENEVISUALIZER_H
#define SCENEVISUALIZER_H

#include <QGraphicsView>
#include <QModelIndex>

class SceneItem;
class QuestScene;

class InteriorItem;
class ItemController;

class QStandardItem;
class QItemSelectionModel;

class SceneVisualizer : public QGraphicsView
{
    Q_OBJECT

    SceneItem *m_item;
    QuestScene *m_scene;

    ItemController* m_controller;

    QMap <int, InteriorItem*> m_graph_to_model;

    QItemSelectionModel* m_selection_model;
public:
    SceneVisualizer(QWidget* parent = 0);

    void setSceneItem(SceneItem*);
    SceneItem *sceneItem()
    {
        return m_item;
    }

    bool createItem();
    void endCreateItem();

    void update();

    void setSelectionModel(QItemSelectionModel* selection_model)
    {
        m_selection_model = selection_model;
    }

public slots:
    void slotRowsRemoved(const QModelIndex&, int, int);
    void slotSceneRemoved(SceneItem*);

    // model changing...
    void slotItemPosChanged(int id, qreal x, qreal y);
    void slotItemUpdate(int id, QRectF rect);

    void slotItemSelected(int);
signals:
    void itemCreated(QString, QPolygonF);
};

#endif // SCENEVISUALIZER_H
