#include "itemcreator.h"

#include "questscene.h"

#include "questitem.h"

#include "episodeitem.h"
#include "actitem.h"
#include "sceneitem.h"
#include "itemitem.h"
#include "itembackground.h"

#include "propertywidget.h"

#include <qjson/serializer.h>

ItemCreator::ItemCreator(QObject *parent) :
    QStandardItemModel(parent)
{
   m_episode = new EpisodeItem(tr("Episode"));
   invisibleRootItem()->appendRow(m_episode);

   m_propertyWidget = 0;
}

void ItemCreator::connectItem(QuestItem* item)
{
    if(m_propertyWidget)
        connect(item->notifier(), SIGNAL(propertyChanged()), m_propertyWidget, SLOT(slotPropertyChanged()));
}

QVariant ItemCreator::toJson() const
{
    return m_episode->toJson();

//    for(int i = 0; i < m_episode->rowCount(); i++)
//    {
//        QuestItem* q_item = m_episode->child(i);

//        QVariantMap act_map = q_item->properties();
//        QVariantList act_scene_list;
//        QuestItem* q_act_item = 0;

//        for(int j = 0; j < q_item->rowCount(); j++)
//        {
//            q_act_item = q_item->child(j);
//            QVariantMap scene_map = q_act_item->properties();
//            ItemItem* q_item_item = 0;

//            for(int k = 0; k < q_act_item->rowCount(); k++)
//            {
//                q_item_item = (ItemItem*)q_act_item->rowCount();

//                QVariantList polygon_list;
//                QVariantList point_list;

//                foreach (QPoint point, q_item_item->polygon()) {
//                    point_list << point.x();
//                    point_list << point.y();

//                    polygon_list << point_list;

//                    point_list.clear();
//                }

//                scene_map.insert("items", polygon_list);
//                polygon_list.clear();

//                act_scene_list << scene_map;
//            }
//        }

//        QVariantList scene_list;
//    }
}


ActItem* ItemCreator::createActItem()
{
    ActItem *tmp_item = new ActItem;

    tmp_item->setData(tr("Act") + QString::number(tmp_item->id()), Qt::DisplayRole);

    m_episode->appendRow(tmp_item);
    connectItem(tmp_item);

    return tmp_item;
}

SceneItem* ItemCreator::createSceneItem(ActItem* item, QuestScene* scene)
{
    SceneItem *scene_item = new SceneItem(scene, "");

    item->appendRow(scene_item);

    connectItem(scene_item);

    return scene_item;
}

ItemItem* ItemCreator::createItemItem(SceneItem* parent, QString title, QPolygonF polygon)
{
    ItemItem *tmp_item = new ItemItem(title, polygon.toPolygon());
    parent->appendRow(tmp_item);

    connectItem(tmp_item);

    return tmp_item;
}
