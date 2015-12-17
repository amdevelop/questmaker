#include "questscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPolygonItem>

#include "itemdialog.h"

#include "itemcontroller.h"

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
    m_active_handel = 0;
    m_active_id = -1;

    m_move_flag = false;

    drawEmpty();

    m_controller = new ItemController(this);
}

QuestScene::~QuestScene()
{
    delete m_controller;
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
                    if(m_item_to_id.keys().contains(tmp_item))
                    {
                        m_active_item = tmp_item;
                        m_active_id = m_item_to_id.value(m_active_item);
                        emit itemSelected(m_item_to_id.value(m_active_item));
                        m_controller->hold(m_active_item);
                        m_active_handel = 0;

                    }
                    else if(m_controller->handelType(tmp_item) != ItemController::HandelInvalid)
                    {
                        m_active_handel = tmp_item;
                    }

                    if(m_active_item || m_active_handel)
                    {
                        m_offset_x = event->scenePos().x() - tmp_item->x();
                        m_offset_y = event->scenePos().y() - tmp_item->y();
                        break;
                    }
                }
                else
                    continue;
            }
        }
        else
        {
            if(m_active_item)
            {
                m_active_item = 0;
                m_active_id = -1;
            }
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
    {
        QGraphicsItem * move_item;

        if(m_active_handel)
            move_item = m_active_handel;
        else
            move_item = m_active_item;

        if(move_item)
        {
            int item_x = event->scenePos().x() - m_offset_x;
            int item_y = event->scenePos().y() - m_offset_y;

            move_item->setPos(item_x, item_y);

            if(move_item == m_active_handel)
                m_controller->moveHandel(move_item);
            else if(move_item == m_active_item)
            {
                if(m_move_flag)
                    m_move_flag = true;
                m_controller->hold(m_active_item);
            }
        }

    }
    case ModeResize:
        break;
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
        if(m_active_handel)
        {
            QGraphicsItem *tl_item = m_controller->handel(ItemController::HandelTopLeft);

            emit itemUpdate(m_item_to_id.value(m_controller->holdItem()),
                            QRectF((tl_item->x() + m_offset_x) / width(),
                                   (tl_item->y() + m_offset_y) / height(),
                                   m_controller->boundingRect().size().width() / (float)width(),
                                   m_controller->boundingRect().size().height() / (float)height()));
        }
        else if(m_active_item)
        {
            if(m_move_flag)
            {
                emit itemPosChanged(m_item_to_id.value(m_active_item),
                                    m_active_item->x() / width(),
                                    m_active_item->y() / height());
                m_move_flag = false;
            }
        }

        m_active_item = 0;
        m_active_handel = 0;

        m_offset_x = m_offset_y = 0;
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

bool QuestScene::addIteriorItem(const QString& file_path,
                                qreal x, qreal y,
                                qreal scale_scene_w, qreal scale_scene_h,
                                int id)
{
    QGraphicsItem* item = 0;

    QPixmap pixmap(file_path);

    if(scale_scene_w == -1 && scale_scene_w == -1)
    {
        scale_scene_w = pixmap.width() / width();
        scale_scene_h = pixmap.height() / height();
    }

    pixmap = pixmap.scaled(width() * scale_scene_w,
                           height() * scale_scene_h);

    if(!pixmap.isNull())
    {
        item = addPixmap(pixmap);

        m_item_to_id.insert(item, id);
        m_id_to_item.insert(id, item);

        item->setPos(x * width(),
                     y * height());

        if(m_active_id == id)
            m_controller->hold(item);

        return true;
    }
    else
        return false;
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
    // нельзя просто так взять и сделать clear()
    // потому что есть еще служебные итемы
    // типа контроллера
    QList<QGraphicsItem*> remove_list;

    foreach (QGraphicsItem* item, items()) {
        if(!m_controller->contains(item))
            remove_list << item;
    }

    foreach (QGraphicsItem* item, remove_list) {
        removeItem(item);
    }

    m_scene_pixmap = 0;
    m_item_to_id.clear();
    m_id_to_item.clear();

    m_controller->release();
    m_controller->hide();
}

QGraphicsItem* QuestScene::graphicsItemFromId(int id)
{
    return m_id_to_item.value(id, 0);
}

void QuestScene::setActiveItem(int id)
{
    QGraphicsItem* tmp_item = graphicsItemFromId(id);
    m_controller->hold(tmp_item);
    m_active_handel = 0;
}

