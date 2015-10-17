#ifndef QUESTSCENE_H
#define QUESTSCENE_H

#include <QGraphicsScene>
#include <QMap>

class SceneItem;
class ItemController;


class QuestScene : public QGraphicsScene
{
    Q_OBJECT

    static int scene_counter;
    int m_id;

    QGraphicsRectItem *m_empty_scene;

    QGraphicsPixmapItem *m_scene_pixmap;

    QPolygonF m_item_polygon;
    QGraphicsPolygonItem *m_polygon_item;

    QList <QGraphicsPolygonItem*> m_items;

    QVector <QPointF> m_points_vector;

    int m_item_point_counter;

    enum QuestSceneMode
    {
        ModeNormal,
        ModeCreateItem,
        ModeResize
    } m_mode;


    QString m_title;

    SceneItem *m_scene_item;

    QGraphicsItem* m_active_item;
    QGraphicsItem* m_active_handel;
    int m_active_id;

    QList <QGraphicsItem*> m_interior;
    QList <QGraphicsItem*> m_subjects;

    qreal m_offset_x;
    qreal m_offset_y;

    QMap <QGraphicsItem*, int> m_item_to_id;
    QMap <int, QGraphicsItem*> m_id_to_item;

    ItemController* m_controller;

public:
    explicit QuestScene(QObject *parent = 0);
    ~QuestScene();

    QString title() const
    {
        return m_title;
    }

    void setTitle(const QString& t)
    {
        m_title = t;
    }

    void setSceneItem(SceneItem* item)
    {
        m_scene_item = item;
    }

    SceneItem* sceneItem()
    {
        return m_scene_item;
    }

    void reset();

    void drawEmpty();

    bool setBackgroundPixmap(const QString& file_path);

    bool addIteriorItem(const QString& file_path,
                        qreal x, qreal y,
                        qreal scale_scene_w = -1, qreal scale_scene_h = -1,
                        int id = -1);
    bool addSubjectItem(const QString& file_path);

    void createItem();
    void endCreateItem();

    QGraphicsItem* graphicsItemFromId(int id);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

signals:
    void itemCreated(QString, QPolygonF);

//    void itemPosChanged(int, QPointF);
    void itemPosChanged(int, qreal x, qreal y);
    void itemUpdate(int, QRectF);

    void itemSelected(int);

public slots:

};

#endif // QUESTSCENE_H
