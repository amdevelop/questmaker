#include "scenevisualizer.h"

#include "questscene.h"
#include "sceneitem.h"

#include "itemitem.h"

SceneVisualizer::SceneVisualizer(QWidget* parent)
    : QGraphicsView(parent)
{
    m_item = 0;

    m_scene = new QuestScene();

    m_scene->setSceneRect(0, 0, 300, 200);

    connect(m_scene, SIGNAL(itemCreated(QString,QPolygonF)), SIGNAL(itemCreated(QString,QPolygonF)));
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
            ItemItem* item_item = (ItemItem*)m_item->child(i);

            m_scene->addPolygon(item_item->polygon(),
                                QPen(Qt::blue, 2),
                                QBrush(QColor(255,255,255,127)));
        }
    }
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


