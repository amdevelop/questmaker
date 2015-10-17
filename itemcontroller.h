#ifndef ITEMCONTROLLER_H
#define ITEMCONTROLLER_H

#include <QList>
#include <QGraphicsItem>

class QuestScene;
class QuestItem;

class ItemController
{
    friend class QuestScene;

public:
    enum HandelType
    {
        HandelInvalid = -1,
        HandelTopLeft = 0,
        HandelTopRight,
        HandelBottomRight,
        HandelBottomLeft,
        HandelMax
    };
private:
    QuestScene* m_scene;

    int m_handle_size;

    // handels
    QMap <QGraphicsItem*, HandelType> m_item_to_type;
    QVector <QGraphicsItem*> m_items;

    // hold item, may be NULL
    QGraphicsItem* m_hold_item;

    ItemController(QuestScene* scene);
public:

    int handelSize() const
    {
        return m_handle_size;
    }

    void addHandel(HandelType type, QGraphicsItem*item);
    void moveHandel(HandelType type, QPointF point = QPointF());
    void moveHandel(QGraphicsItem*, QPointF point = QPointF());

    QRectF boundingRect() const;

    HandelType handelType(QGraphicsItem* item) const;
    QGraphicsItem* handel(HandelType type);

    void show();
    void hide();

    void hold(QGraphicsItem*);
    QGraphicsItem* holdItem() const
    {
        return m_hold_item;
    }

    void release()
    {
        m_hold_item = 0;
    }
};

#endif // ITEMCONTROLLER_H
