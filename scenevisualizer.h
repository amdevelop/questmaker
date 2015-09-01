#ifndef SCENEVISUALIZER_H
#define SCENEVISUALIZER_H

#include <QGraphicsView>
#include <QModelIndex>

class SceneItem;
class QuestScene;

class ItemItem;

class QStandardItem;

class SceneVisualizer : public QGraphicsView
{
    Q_OBJECT

    SceneItem *m_item;
    QuestScene *m_scene;

    QMap <int, ItemItem*> m_graph_to_model;
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

public slots:
    void slotRowsRemoved(const QModelIndex&, int, int);
    void slotSceneRemoved(SceneItem*);

    // model changing...
    void slotItemPosChanged(int id, QPointF point);

signals:
    void itemCreated(QString, QPolygonF);
};

#endif // SCENEVISUALIZER_H
