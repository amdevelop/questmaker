#ifndef SCENEVISUALIZER_H
#define SCENEVISUALIZER_H

#include <QGraphicsView>
#include <QModelIndex>

class SceneItem;
class QuestScene;

class QStandardItem;

class SceneVisualizer : public QGraphicsView
{
    Q_OBJECT

    SceneItem *m_item;
    QuestScene *m_scene;
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

signals:
    void itemCreated(QString, QPolygonF);
};

#endif // SCENEVISUALIZER_H
