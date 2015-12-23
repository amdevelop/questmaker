#include "itemcontroller.h"

#include "questscene.h"

#include <QGraphicsItem>

#include <QDebug>

ItemController::ItemController(QuestScene* scene)
{
    m_scene = scene;

    m_items = QVector<QGraphicsItem*>(HandelBottomLeft + 1);

    m_handle_size = 6;
    m_hold_item = 0;

    QGraphicsItem *tmp_item;
    tmp_item = m_scene->addEllipse(0,0,m_handle_size,m_handle_size,QPen(Qt::red, 2), QBrush(Qt::red));
    m_item_to_type.insert(tmp_item,
                          HandelTopLeft);
    m_items[HandelTopLeft] = tmp_item;
    tmp_item->hide();
    tmp_item->setZValue(1000 + HandelTopLeft);

    tmp_item = m_scene->addEllipse(0,0,m_handle_size,m_handle_size,QPen(Qt::red, 2), QBrush(Qt::red));
    m_item_to_type.insert(tmp_item,
                          HandelTopRight);
    m_items[HandelTopRight] = tmp_item;
    tmp_item->hide();
    tmp_item->setZValue(1000 + HandelTopRight);

    tmp_item = m_scene->addEllipse(0,0,m_handle_size,m_handle_size,QPen(Qt::red, 2), QBrush(Qt::red));
    m_item_to_type.insert(tmp_item,
                          HandelBottomRight);
    m_items[HandelBottomRight] = tmp_item;
    tmp_item->hide();
    tmp_item->setZValue(1000 + HandelBottomRight);

    tmp_item = m_scene->addEllipse(0,0,m_handle_size,m_handle_size,QPen(Qt::red, 2), QBrush(Qt::red));
    m_item_to_type.insert(tmp_item,
                          HandelBottomLeft);
    m_items[HandelBottomLeft] = tmp_item;
    tmp_item->hide();
    tmp_item->setZValue(1000 + HandelBottomLeft);


    m_lines = QVector<QGraphicsLineItem*>(LineBottom + 1);

    m_lines[LineLeft] = m_scene->addLine(QLineF(QPointF(0,0),
                                               QPointF(0,0)),
                                         QPen(Qt::red, 1));
    m_lines[LineLeft]->setZValue(1000);
    m_lines[LineLeft]->hide();

    m_lines[LineTop] = m_scene->addLine(QLineF(QPointF(),
                                               QPointF()),
                                        QPen(Qt::red, 1));
    m_lines[LineTop]->setZValue(1000);
    m_lines[LineTop]->hide();

    m_lines[LineRight] = m_scene->addLine(QLineF(QPointF(),
                                               QPointF()),
                                          QPen(Qt::red, 1));
    m_lines[LineRight]->setZValue(1000);
    m_lines[LineRight]->hide();

    m_lines[LineBottom] = m_scene->addLine(QLineF(QPointF(),
                                               QPointF()),
                                           QPen(Qt::red, 1));
    m_lines[LineBottom]->setZValue(1000);
    m_lines[LineBottom]->hide();
}

void ItemController::hold(QGraphicsItem *item)
{
    m_hold_item = item;

    QGraphicsItem* handel_item = m_items[HandelTopLeft];
    handel_item->setPos(item->x() + item->boundingRect().left() - m_handle_size / 2,
                        item->y() + item->boundingRect().top() - m_handle_size / 2);
    handel_item->show();

    handel_item = m_items[HandelTopRight];
    handel_item->setPos(item->x() + item->boundingRect().right() - m_handle_size / 2,
                        item->y() + item->boundingRect().top() - m_handle_size / 2);
    handel_item->show();

    handel_item = m_items[HandelBottomRight];
    handel_item->setPos(item->x() + item->boundingRect().right() - m_handle_size / 2,
                        item->y() + item->boundingRect().bottom() - m_handle_size / 2);
    handel_item->show();

    handel_item = m_items[HandelBottomLeft];
    handel_item->setPos(item->x() + item->boundingRect().left() - m_handle_size / 2,
                        item->y() + item->boundingRect().bottom() - m_handle_size / 2);
    handel_item->show();

    drawLines();
}

void ItemController::addHandel(HandelType type,
                               QGraphicsItem* item)
{
    m_item_to_type.insert(item, type);
}

QGraphicsItem* ItemController::handel(HandelType type)
{
    if(type < HandelMax && type > HandelInvalid)
        return m_items[type];
    else
        return 0;
}

bool ItemController::contains(QGraphicsItem* item)
{
    return m_items.contains(item) || m_lines.contains((QGraphicsLineItem*)item);
}

ItemController::HandelType ItemController::handelType(QGraphicsItem* item) const
{
    return m_item_to_type.value(item, HandelInvalid);
}

void ItemController::show()
{
    foreach (QGraphicsItem* tmp_item, m_item_to_type.keys()) {
        tmp_item->show();
    }

    foreach (QGraphicsItem* line, m_lines) {
        line->show();
    }
}

void ItemController::hide()
{
    foreach (QGraphicsItem* tmp_item, m_item_to_type.keys()) {
        tmp_item->hide();
    }

    foreach (QGraphicsItem* line, m_lines) {
        line->hide();
    }
}

void ItemController::moveHandel(QGraphicsItem* item, QPointF point)
{
    int i = 0;
    for(i = 0; i < m_items.size(); i++) {
        if(m_items[i] == item)
            break;
    }

    if(i < HandelMax)
        moveHandel((HandelType)i, point);
}

void ItemController::moveHandel(HandelType type, QPointF)
{
    QGraphicsItem *src_handle = m_items[type];
    switch (type) {
    case HandelTopLeft:
        m_items[HandelBottomLeft]->setX(src_handle->x());
        m_items[HandelTopRight]->setY(src_handle->y());
        break;
    case HandelTopRight:
        m_items[HandelBottomRight]->setX(src_handle->x());
        m_items[HandelTopLeft]->setY(src_handle->y());
        break;
    case HandelBottomRight:
        m_items[HandelBottomLeft]->setY(src_handle->y());
        m_items[HandelTopRight]->setX(src_handle->x());
        break;
    case HandelBottomLeft:
        m_items[HandelBottomRight]->setY(src_handle->y());
        m_items[HandelTopLeft]->setX(src_handle->x());
        break;
    default:
        break;
    }

    drawLines();
}

QRectF ItemController::boundingRect() const
{
    return QRectF(m_items[HandelTopLeft]->pos(),
                  m_items[HandelBottomRight]->pos());
}

void ItemController::drawLines()
{
    m_lines[LineLeft]->setLine(QLineF(m_items[HandelTopLeft]->pos() + QPointF(m_handle_size / 2, m_handle_size / 2),
                                      m_items[HandelBottomLeft]->pos() + QPointF(m_handle_size / 2, m_handle_size / 2)));
    m_lines[LineLeft]->show();

    m_lines[LineTop]->setLine(QLineF(m_items[HandelTopLeft]->pos() + QPointF(m_handle_size / 2, m_handle_size / 2),
                                      m_items[HandelTopRight]->pos() + QPointF(m_handle_size / 2, m_handle_size / 2)));
    m_lines[LineTop]->show();

    m_lines[LineRight]->setLine(QLineF(m_items[HandelTopRight]->pos() + QPointF(m_handle_size / 2, m_handle_size / 2),
                                      m_items[HandelBottomRight]->pos() + QPointF(m_handle_size / 2, m_handle_size / 2)));
    m_lines[LineRight]->show();

    m_lines[LineBottom]->setLine(QLineF(m_items[HandelBottomLeft]->pos() + QPointF(m_handle_size / 2, m_handle_size / 2),
                                      m_items[HandelBottomRight]->pos() + QPointF(m_handle_size / 2, m_handle_size / 2)));
    m_lines[LineBottom]->show();
}
