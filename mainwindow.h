#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QSettings>

#include <questscene.h>

#include "itemcreator.h"

namespace Ui {
    class MainWindow;
}

class QTreeWidgetItem;


class ActItem;
class QuestItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QSettings m_settings;
    ItemCreator *m_item_creator;

    QuestScene *m_scene;

    EpisodeItem *m_episode;
    ActItem* m_act;

    InteriorItem* m_interior_item;

    QuestItem* createScene();
    QuestItem* createAct();
    QuestItem* createActEmpty();

    QuestItem* createItem(QString title, QPolygonF);

    void setActiveSceneFromItem(QuestItem *);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void slotFileOpen();
    void slotOpenFileToProperties();
    void slotOpenFileToInterior();
    void slotOpenFileToSubject();

    void slotMoveUp();
    void slotMoveDown();

    void slotCreateItem(bool);

    void slotPublish();

    void slotCreateScene();
    void slotCreateAct();
    void slotTreeWidgetClicked(QModelIndex);
    void slotTableDoubleClicked(QModelIndex);
    void slotItemCreated(QString, QPolygonF);

    void slotCurrentChanged(const QModelIndex&,
                            const QModelIndex&);

    void slotCreateEpisode();
    void slotCloseEpisode();

    void slotZoomIn();
    void slotZoomOut();

};

#endif // MAINWINDOW_H
