#ifndef SCENEVISUALIZER_H
#define SCENEVISUALIZER_H

#include <QGraphicsView>
#include <QModelIndex>

class SceneItem;
class QuestScene;

class InteriorItem;
class ItemController;
class ItemCreator;

class QStandardItem;
class QStandardItemModel;
class QItemSelectionModel;

class SceneVisualizer : public QGraphicsView
{
    Q_OBJECT

    QuestScene *m_scene;

    QStandardItemModel* m_model;
    SceneItem *m_item;

    ItemController* m_controller;

    QMap <int, InteriorItem*> m_graph_to_model;

    QItemSelectionModel* m_selection_model;

    double m_zoom;

public:

    enum ZoomType
    {
        ZoomIn,
        ZoomOut,
        ZoomOnetoOne
    };

    SceneVisualizer(QWidget* parent = 0);
    void update();

    void setSceneItem(SceneItem*);
    SceneItem *sceneItem()
    {
        return m_item;
    }

    bool createItem();
    void endCreateItem();

    // todo: выпилить этот метод и брать из модели
    void setSelectionModel(QItemSelectionModel* selection_model);
    void setModel(QStandardItemModel*);

    void zoom(ZoomType);
    void isZoomEnabled(bool& in, bool& out);

public slots:
    void slotRowsRemoved(const QModelIndex&, int, int);
    void slotSceneRemoved(SceneItem*);

    // model changing...
    void slotItemPosChanged(int id, qreal x, qreal y);
    void slotItemUpdate(int id, QRectF rect);

    void slotItemSelected(int);

    void slotCurrentChanged(QModelIndex,QModelIndex);

    void slotRowsRemoved(QModelIndex&, int, int);
signals:
    void itemCreated(QString, QPolygonF);
};

#endif // SCENEVISUALIZER_H
