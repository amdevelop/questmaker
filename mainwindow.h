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
    ItemCreator item_creator;

    QuestScene *m_scene;

    EpisodeItem *m_episode;
    ActItem* m_act;



    void createScene();
    void createAct();

    void setActiveSceneFromItem(SceneItem*);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void slotFileOpen();
    void slotCreateItem(bool);

    void slotCreateScene();
    void slotCreateAct();
    void slotTreeWidgetClicked(QTreeWidgetItem*,int);

    void slotItemCreated(QString);

    void slotCreateEpisode();
};

#endif // MAINWINDOW_H
