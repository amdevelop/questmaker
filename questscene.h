#ifndef QUESTSCENE_H
#define QUESTSCENE_H

#include <QGraphicsScene>

class SceneItem;

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
        ModeCreateItem
    } m_mode;


    QString m_title;

    SceneItem *m_scene_item;

public:
    explicit QuestScene(QObject *parent = 0);

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

    bool setBackgroundPixmap(const QString& file_path);

    void createItem();
    void endCreateItem();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

signals:
    void itemCreated(QString);

public slots:

};

#endif // QUESTSCENE_H
