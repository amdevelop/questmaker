#include "questscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPolygonItem>

#include "itemdialog.h"

#include <QDebug>

int QuestScene::scene_counter = 0;

QuestScene::QuestScene(QObject *parent) :
    QGraphicsScene(parent)
{
    m_id = scene_counter++;
    m_title = tr("Scene") + QString::number(m_id);

    m_mode = ModeNormal;

    m_polygon_item = 0;
    m_item_point_counter = 0;

    m_scene_pixmap = 0;

    m_empty_scene = addRect(0, 0, 300, 200); //QPolygon() << QPoint(0,0) << )

}

void QuestScene::createItem()
{
    m_mode = ModeCreateItem;
}

void QuestScene::endCreateItem()
{
    m_mode = ModeNormal;

    if(m_polygon_item)
    {
        removeItem(m_polygon_item);
        delete m_polygon_item;
        m_polygon_item = 0;

        m_item_polygon.clear();
        m_item_point_counter = 0;
        m_points_vector.clear();
    }
}

void QuestScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    switch(m_mode)
    {
    case ModeCreateItem:

        m_points_vector << event->scenePos();
        m_item_polygon = QPolygonF(m_points_vector);

        m_item_point_counter++;

        if(!m_polygon_item)
        {
            m_polygon_item = addPolygon(m_item_polygon, QPen(Qt::blue), QBrush(QColor(255,255,255,127)));
            m_polygon_item->setFillRule(Qt::WindingFill);
        }
        else
        {
            m_polygon_item->setPolygon(m_item_polygon);
        }

        qDebug() << event->scenePos();
        break;
    case ModeNormal:
    default:
        break;
    }
}

void QuestScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    switch(m_mode)
    {
    case ModeCreateItem:
        if(m_polygon_item)
        {
            if(m_points_vector.count() > m_item_point_counter + 1)
                m_points_vector.pop_back();

            m_points_vector << event->scenePos();
            m_item_polygon = QPolygonF(m_points_vector);
            m_polygon_item->setPolygon(m_item_polygon);

            qDebug() << event->scenePos();
        }
        break;
    case ModeNormal:
    default:
        break;
    }

}

void QuestScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    switch(m_mode)
    {
    case ModeCreateItem:
        m_item_polygon.append(event->scenePos());
        m_polygon_item->setPolygon(m_item_polygon);

        m_items << m_polygon_item;

        m_polygon_item = 0;
        m_item_polygon.clear();
        m_item_point_counter = 0;
        m_points_vector.clear();

        emit itemCreated(ItemDialog::getItemTitle());
        break;
    case ModeNormal:
    default:
        break;
    }
}

bool QuestScene::setBackgroundPixmap(const QString& file_path)
{
    QPixmap pixmap(file_path);

    if(!pixmap.isNull())
    {
        if(m_scene_pixmap)
        {
            removeItem(m_scene_pixmap);
            delete m_scene_pixmap;
        }

        m_empty_scene->hide();
        m_scene_pixmap = addPixmap(pixmap);
        m_scene_pixmap->setZValue(-1);
        return true;
    }
    else
    {
        return false;
    }
}