#ifndef QUESTTREE_H
#define QUESTTREE_H

#include <QTreeView>

class QuestTree : public QTreeView
{
    Q_OBJECT

    QMenu *m_episodeMenu;
    QMenu *m_actMenu;
    QMenu *m_sceneMenu;

public:
    QuestTree(QWidget* parent = 0);

protected:
    void contextMenuEvent(QContextMenuEvent *);

signals:
    void createAct();
    void createScene();
};

#endif // QUESTTREE_H
