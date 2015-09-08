#include "scenevisualizer.h"

#include "questscene.h"
#include "sceneitem.h"

#include "interioritem.h"
#include "itemitem.h"

#include <QMessageBox>

SceneVisualizer::SceneVisualizer(QWidget* parent)
    : QGraphicsView(parent)
{
    m_item = 0;

    m_scene = new QuestScene();

    m_scene->setSceneRect(0, 0, 300, 200);

    connect(m_scene, SIGNAL(itemCreated(QString,QPolygonF)), SIGNAL(itemCreated(QString,QPolygonF)));
    connect(m_scene,
            SIGNAL(itemPosChanged(int,qreal,qreal)),
            SLOT(slotItemPosChanged(int,qreal,qreal)));


    setScene(m_scene);
}

void SceneVisualizer::setSceneItem(SceneItem* scene_item)
{
    m_item = scene_item;
}

#include <QDebug>

void SceneVisualizer::update()
{
    m_scene->clear();
    m_scene->reset();

    if(m_item)
    {
        qDebug() << m_item->property("background");
        if(m_item->property("background").isValid())
        {
            if(!m_scene->setBackgroundPixmap(
                        m_item->property("background").toString()))
                m_scene->drawEmpty();
        }
        else
            m_scene->drawEmpty();

        for(int i = 0;
            i < m_item->rowCount();
            i++)
        {

            InteriorItem* interior_item =
                    dynamic_cast<InteriorItem*>(m_item->child(i));
            if(interior_item)
            {
                QString file_path =
                        interior_item->property("image").toString();


                int id = m_scene->addIteriorItem(
                            file_path,
                            interior_item->sceneX(),
                            interior_item->sceneY(),
                            interior_item->property("scene_scale_x").toDouble(),
                            interior_item->property("scene_scale_y").toDouble());

                if(id != -1)
                    m_graph_to_model.insert(id, interior_item);

                for(int j = 0;
                    j < interior_item->rowCount();
                    j++)
                {
                    ItemItem* item_item =
                            (ItemItem*)interior_item->child(j);

                    if(item_item)
                        m_scene->addPolygon(item_item->drawPolygon(m_scene->width(), m_scene->height()),
                                            QPen(Qt::blue, 2),
                                            QBrush(QColor(255,255,255,127)));


                }
            }
        }
    }
    else
        m_scene->drawEmpty();
}

bool SceneVisualizer::createItem()
{
    if(m_item)
    {
        setMouseTracking(true);
        m_scene->createItem();
    }

    return (bool)m_item;
}

void SceneVisualizer::endCreateItem()
{
    setMouseTracking(false);
    m_scene->endCreateItem();
}

void SceneVisualizer::slotRowsRemoved(const QModelIndex&,
                      int,
                      int)
{
    update();
}

void SceneVisualizer::slotSceneRemoved(SceneItem* item)
{
    if(m_item == item)
    {
        m_scene->clear();
        m_scene->reset();
        m_item = 0;

        m_scene->drawEmpty();
    }
}

//void SceneVisualizer::slotItemPosChanged(int id, QPointF point)
//{
//    InteriorItem * item = m_graph_to_model.value(id);
//    item->setSceneX(point.x() / m_scene->width());
//    item->setSceneY(point.y() / m_scene->height());

//    update();
//}

void SceneVisualizer::slotItemPosChanged(int id,
                                         qreal x,
                                         qreal y)
{
    InteriorItem * item = m_graph_to_model.value(id);
    item->setSceneX(x);
    item->setSceneY(y);

    update();
}
