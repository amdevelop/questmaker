#include "itemcreator.h"

#include "questscene.h"

#include "questitem.h"

#include "episodeitem.h"
#include "actitem.h"
#include "sceneitem.h"
#include "itemitem.h"
#include "itembackground.h"

#ifdef WIN32
#include <qjson/src/serializer.h>
#else
#include <qjson/serializer.h>
#endif

ItemCreator::ItemCreator(QObject *parent) :
    QStandardItemModel(parent),
    m_selection_model(this)
{
   m_episode = new EpisodeItem(tr("Episode"));
   invisibleRootItem()->appendRow(m_episode);
}

QVariant ItemCreator::toJson(QString save_path) const
{
    return m_episode->toJson(save_path);
}

ActItem* ItemCreator::createActItem()
{
    ActItem *tmp_item = new ActItem;

    tmp_item->setData(tr("Act") + QString::number(tmp_item->id()), Qt::DisplayRole);

    m_episode->appendRow(tmp_item);

    return tmp_item;
}

SceneItem* ItemCreator::createSceneItem(ActItem* item, QuestScene* scene)
{
    SceneItem *scene_item = new SceneItem(scene, "");

    item->appendRow(scene_item);

    return scene_item;
}

ItemItem* ItemCreator::createItemItem(SceneItem* parent, QString title, QPolygonF polygon)
{
    ItemItem *tmp_item = new ItemItem(title, polygon);
    parent->appendRow(tmp_item);

    return tmp_item;
}

void ItemCreator::removeItem(QuestItem* item)
{
    if(dynamic_cast<SceneItem*>(item))
        emit sceneRemoved((SceneItem*)(item));

    removeRow(item->row(), item->parent()->index());
}

