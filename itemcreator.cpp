#include "itemcreator.h"

#include "questscene.h"

#include "questitem.h"

#include "episodeitem.h"
#include "actitem.h"
#include "sceneitem.h"
#include "itemitem.h"
#include "itembackground.h"

#include "propertywidget.h"

ItemCreator::ItemCreator(QObject *parent) :
    QObject(parent)
{
    m_propertyWidget = 0;
}

void ItemCreator::connectItem(QuestItem* item)
{
    if(m_propertyWidget)
        connect(item->notifier(), SIGNAL(propertyChanged()), m_propertyWidget, SLOT(slotPropertyChanged()));
}

ActItem* ItemCreator::createActItem(EpisodeItem* item)
{
    ActItem *tmp_item = new ActItem(item);
    tmp_item->setData(0, Qt::DisplayRole, tr("Act") + QString::number(tmp_item->id()));

    connectItem(tmp_item);

    return tmp_item;
}

SceneItem* ItemCreator::createSceneItem(ActItem* item, QuestScene* scene)
{
    SceneItem *scene_item = new SceneItem(scene, item);

    connectItem(scene_item);

    return scene_item;
}

ItemItem* ItemCreator::createItemItem(SceneItem* parent, QString title)
{
    ItemItem *tmp_item = new ItemItem(parent);
    tmp_item->setData(0, Qt::DisplayRole, title);

    connectItem(tmp_item);

    return tmp_item;
}
