#ifndef QUESTTREE_H
#define QUESTTREE_H

#include <QTreeWidget>

class QuestTree : public QTreeWidget
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
