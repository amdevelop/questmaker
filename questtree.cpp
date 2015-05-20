#include "questtree.h"

#include <QMenu>
#include <QContextMenuEvent>

#include "questitem.h"

#include <QDebug>

#include "itemcreator.h"

QuestTree::QuestTree(QWidget* parent)
    : QTreeView(parent)
{
    m_episodeMenu = new QMenu(tr("Episode menu"), this);
    m_actMenu = new QMenu(tr("Act menu"), this);
    m_sceneMenu = new QMenu(tr("Scene menu"), this);

    m_episodeMenu->addAction(tr("Create act"), this, SIGNAL(createAct()));
    m_actMenu->addAction(tr("Create scene"), this, SIGNAL(createScene()));

    m_sceneMenu->addAction(tr("Up"));
    m_sceneMenu->addAction(tr("Down"));
    m_sceneMenu->addSeparator();

    connect(m_sceneMenu->addAction(tr("Delete")),
            SIGNAL(triggered()),
            SLOT(slotDeleteItem()));
}

void QuestTree::contextMenuEvent(QContextMenuEvent *evt)
{
    m_contexMenuItem = dynamic_cast<QuestItem*>(
                ((QStandardItemModel*)model())->itemFromIndex(
                    indexAt(evt->pos())));

    if(m_contexMenuItem)
    {
        switch(m_contexMenuItem->itemType())
        {
        case QuestItem::TypeEpisode:
            m_episodeMenu->exec(evt->globalPos());
            break;
        case QuestItem::TypeAct:
            m_actMenu->exec(evt->globalPos());
            break;
        case QuestItem::TypeScene:
        case QuestItem::TypeItemItem:
            m_sceneMenu->exec(evt->globalPos());
            break;
        case QuestItem::TypeItemBackGround:
            break;
        }
    }
}

void QuestTree::slotDeleteItem()
{
    if(m_contexMenuItem)
    {
        ((ItemCreator*)model())->removeItem(m_contexMenuItem);
        m_contexMenuItem = 0;
    }
}

