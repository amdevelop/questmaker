#include "questscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPolygonItem>

#include "itemdialog.h"

#include <QDebug>

int QuestScene::scene_counter = 0;

QuestScene::QuestScene(QObject *parent) :
    QGraphicsScene(parent)
{
//    m_id = scene_counter++;
    m_title = tr("Scene") + QString::number(m_id);

    m_mode = ModeNormal;

    m_polygon_item = 0;
    m_item_point_counter = 0;

    m_scene_pixmap = 0;

    m_active_item = 0;

    drawEmpty();
}

void QuestScene::drawEmpty()
{
    m_empty_scene = addRect(0, 0, 300, 200);
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

//        qDebug() << event->scenePos();
        break;
    case ModeNormal:
    {
        QList <QGraphicsItem*> item_list =
                items(event->scenePos());


        if(!item_list.isEmpty())
        {
            foreach (QGraphicsItem* tmp_item, item_list) {
                if(tmp_item != m_empty_scene)
                {
                    m_active_item = tmp_item;

                    if(!m_item_to_id.keys().contains(m_active_item))
                        m_active_item = 0;

                    if(m_active_item)
                    {
                        m_offset_x = event->scenePos().x() - m_active_item->x();
                        m_offset_y = event->scenePos().y() - m_active_item->y();
                    }

                    break;
                }
                else
                    continue;
            }
        }
        else
        {
            if(m_active_item)
                m_active_item = 0;
        }
    }
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
            if(m_points_vector.count() > m_item_point_counter)
                m_points_vector.pop_back();

            m_points_vector << event->scenePos();
            m_item_polygon = QPolygonF(m_points_vector);
            m_polygon_item->setPolygon(m_item_polygon);

        }
        break;
    case ModeNormal:
        if(m_active_item)
        {
            int item_x = event->scenePos().x() - m_offset_x;
            int item_y = event->scenePos().y() - m_offset_y;

            m_active_item->setPos(item_x, item_y);

//            emit itemPosChanged(m_item_to_id.value(m_active_item),
//                                m_active_item->pos());

//            emit itemPosChanged(m_item_to_id.value(m_active_item),
//                                item_x / width(),
//                                item_y / height());
        }
    default:
        break;
    }

}

void QuestScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent*)
{
    switch(m_mode)
    {
    case ModeCreateItem:
        if(!m_points_vector.isEmpty())
        {
            m_points_vector.pop_back();
            m_item_polygon = QPolygonF(m_points_vector);
            m_polygon_item->setPolygon(m_item_polygon);

            m_items << m_polygon_item;


            QPolygonF double_polygon;

            foreach (QPointF point, m_points_vector) {
                double_polygon << QPointF(point.x() / width(),
                                          point.y() / height());
            }

//            qDebug() << double_polygon;

            emit itemCreated("", double_polygon);

            m_polygon_item = 0;
            m_item_polygon.clear();
            m_item_point_counter = 0;
            m_points_vector.clear();
        }

        break;
    case ModeNormal:
    default:
        break;
    }
}

void QuestScene::mouseReleaseEvent(QGraphicsSceneMouseEvent*)
{
    switch(m_mode)
    {
    case ModeNormal:
        if(m_active_item)
        {
            emit itemPosChanged(m_item_to_id.value(m_active_item),
                                m_active_item->x() / width(),
                                m_active_item->y() / height());
            m_active_item = 0;
            m_offset_x = m_offset_y = 0;
        }
        break;
    case ModeCreateItem:

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

        m_scene_pixmap = addPixmap(
                    pixmap.scaled(width(),
                                  height()));

        m_scene_pixmap->setZValue(-1);
        return true;
    }
    else
    {
        return false;
    }
}

int QuestScene::addIteriorItem(const QString& file_path,
                               qreal x,
                               qreal y)
{
    QGraphicsItem* item = 0;
    int ret_id = -1;

    QPixmap pixmap(file_path);

    if(!pixmap.isNull())
    {
        item = addPixmap(pixmap);
        ret_id = scene_counter++;
        m_item_to_id.insert(item, ret_id);

        item->setPos(x * width(),
                     y * height());
    }

    return ret_id;
}

bool QuestScene::addSubjectItem(const QString& file_path)
{
    QPixmap pixmap(file_path);

    if(!pixmap.isNull())
    {
        addPixmap(pixmap);
        return true;
    }

    return false;
}

void QuestScene::reset()
{
    m_scene_pixmap = 0;
    m_item_to_id.clear();
}

