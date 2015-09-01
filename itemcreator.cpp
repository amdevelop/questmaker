#include "itemcreator.h"

#include "questscene.h"

#include "questitem.h"

#include "episodeitem.h"
#include "actitem.h"
#include "sceneitem.h"

#include "itemitem.h"
#include "subjectitem.h"
#include "interioritem.h"

#include "itembackground.h"

#include <qjson/serializer.h>

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

ItemItem* ItemCreator::createItemItem(InteriorItem* parent, QString title, QPolygonF polygon)
{
    ItemItem *tmp_item = new ItemItem(title, polygon);
    parent->appendRow(tmp_item);

    return tmp_item;
}

ItemItem* ItemCreator::createSubjectItem(SceneItem* parent,
                                         QString file_path,
                                         QString title)
{
    ItemItem *tmp_item = new SubjectItem(title);
    parent->appendRow(tmp_item);

    return tmp_item;
}

ItemItem* ItemCreator::createInteriorItem(SceneItem* parent,
                           QString file_path,
                           QString title)
{
    ItemItem *tmp_item = new InteriorItem(title);
    tmp_item->setProperty("image_path", file_path);
    parent->appendRow(tmp_item);

    return tmp_item;
}


void ItemCreator::removeItem(QuestItem* item)
{
    if(dynamic_cast<SceneItem*>(item))
        emit sceneRemoved((SceneItem*)(item));

    removeRow(item->row(), item->parent()->index());
}

