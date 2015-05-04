#include "questtree.h"

#include <QMenu>
#include <QContextMenuEvent>
#include "questitem.h"

QuestTree::QuestTree(QWidget* parent)
    : QTreeWidget(parent)
{
    m_episodeMenu = new QMenu(tr("Episode menu"), this);
    m_actMenu = new QMenu(tr("Act menu"), this);
    m_sceneMenu = new QMenu(tr("Scene menu"), this);

    m_episodeMenu->addAction(tr("Create act"), this, SIGNAL(createAct()));
    m_actMenu->addAction(tr("Create scene"), this, SIGNAL(createScene()));

    m_sceneMenu->addAction(tr("Up"));
    m_sceneMenu->addAction(tr("Down"));
    m_sceneMenu->addSeparator();
    m_sceneMenu->addAction(tr("Delete"));
}

void QuestTree::contextMenuEvent(QContextMenuEvent *evt)
{
    QuestItem* tmp_item = dynamic_cast<QuestItem*>(itemAt(evt->pos()));

    if(tmp_item)
    {
        switch(tmp_item->itemType())
        {
        case QuestItem::TypeEpisode:
            m_episodeMenu->exec(evt->globalPos());
            break;
        case QuestItem::TypeAct:
            m_actMenu->exec(evt->globalPos());
            break;
        case QuestItem::TypeScene:
            m_sceneMenu->exec(evt->globalPos());
            break;
        case QuestItem::TypeItemItem:
        case QuestItem::TypeItemBackGround:
            break;
        }
    }
}
