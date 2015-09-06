#ifndef INTERIORITEM_H
#define INTERIORITEM_H

#include "questitem.h"


class InteriorItem : public QuestItem
{
    static int interior_counter;

    int m_id;

    double m_scene_x;
    double m_scene_y;

public:
    explicit InteriorItem(QString title,
                          QuestItem::ItemType type = QuestItem::TypeInterior);

    static int getItemCounter(bool inc = false)
    {
        if(inc)
            return interior_counter++;
        else
            return interior_counter;
    }

    void setSceneX(double x)
    {
        m_scene_x = x;
    }

    double sceneX() const
    {
        return m_scene_x;
    }

    void setSceneY(double y)
    {
        m_scene_y = y;
    }

    double sceneY() const
    {
        return m_scene_y;
    }


    QVariant toJson(QString save_path, bool *);
};

#endif // INTERIORITEM_H
