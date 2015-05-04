#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

#include <QDir>
#include <QMessageBox>
#include <QScrollBar>

#include "episodeitem.h"
#include "actitem.h"
#include "sceneitem.h"
#include "itemitem.h"
#include "itembackground.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_settings("questmaker.ini", QSettings::IniFormat)
{
    hide();

    ui = new Ui::MainWindow;

    m_scene = 0;
    m_act = 0;
    m_episode = 0;


    ui->setupUi(this);

    item_creator.setPropetryWidget(ui->tableWidget);

    connect(ui->actionOpen, SIGNAL(triggered()), SLOT(slotFileOpen()));
    connect(ui->actionCreate, SIGNAL(triggered()), SLOT(slotCreateScene()));

    connect(ui->actionCreate_episode, SIGNAL(triggered()), SLOT(slotCreateEpisode()));

    connect(ui->toolCreateItem, SIGNAL(toggled(bool)), SLOT(slotCreateItem(bool)));

    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), SLOT(slotTreeWidgetClicked(QTreeWidgetItem*,int)));
    connect(ui->treeWidget, SIGNAL(createAct()), SLOT(slotCreateAct()));
    connect(ui->treeWidget, SIGNAL(createScene()), SLOT(slotCreateScene()));

    ui->graphicsView->horizontalScrollBar()->hide();
    ui->graphicsView->verticalScrollBar()->hide();

    move(m_settings.value("WindowPosX", 0).toInt(), m_settings.value("WindowPosY", 0).toInt());
    resize(m_settings.value("WindowWidth", 500).toInt(), m_settings.value("WindowHeight", 500).toInt());

    ui->splitter->setSizes(QList<int>() <<
                           m_settings.value("SplitterSceneSize", 250).toInt() <<
                           m_settings.value("SplitterTreeSize", 250).toInt());

    ui->tableWidget->verticalHeader()->hide();

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Value"));
}

MainWindow::~MainWindow()
{
    m_settings.setValue("WindowPosX", pos().x());
    m_settings.setValue("WindowPosY", pos().y());
    m_settings.setValue("WindowWidth", size().width());
    m_settings.setValue("WindowHeight", size().height());

    m_settings.setValue("SplitterSceneSize", ui->splitter->sizes()[0]);
    m_settings.setValue("SplitterTreeSize", ui->splitter->sizes()[1]);

    delete ui;
}

void MainWindow::slotFileOpen()
{
    QString file_path = QFileDialog::getOpenFileName(this, "", m_settings.value("LastOpenedDir", QDir::homePath()).toString());

    if(!file_path.isEmpty())
    {
        QFileInfo fi(file_path);
        m_settings.setValue("LastOpenedDir", fi.canonicalFilePath());

        if(m_scene)
        {
            if(!m_scene->setBackgroundPixmap(file_path))
                QMessageBox::critical(this, tr("Critical"), tr("Don't support file format!"));
            else
            {
                m_scene->sceneItem()->setProperty("background", file_path);
//                ItemBackground* tmp_item = item_creator.createItemBackground(m_scene->sceneItem(), fi.fileName() + " " + tr("(background)"));
//                tmp_item->setData(0, Qt::ToolTipRole, fi.canonicalFilePath());
            }
        }
    }
}

void MainWindow::slotCreateEpisode()
{
    m_episode = new EpisodeItem(ui->treeWidget->invisibleRootItem());
    m_episode->setData(0, Qt::DisplayRole, tr("Episode"));

    createAct();

    ui->treeWidget->expandAll();
}

/// SCENE

void MainWindow::createScene()
{
    m_scene = new QuestScene;
    connect(m_scene, SIGNAL(itemCreated(QString)), SLOT(slotItemCreated(QString)));
    m_scene->setSceneRect(0, 0, 300, 200);

    SceneItem* scene_item = item_creator.createSceneItem(m_act, m_scene);
    scene_item->setData(0, Qt::DisplayRole, m_scene->title());

    m_scene->setSceneItem(scene_item);

    ui->graphicsView->setScene(m_scene);
    ui->treeWidget->setCurrentItem(scene_item);

    ui->tableWidget->setQuestItem(scene_item);
}

void MainWindow::slotCreateScene()
{
    if(m_act)
        createScene();
    else
        QMessageBox::warning(this, tr("Warning"), tr("There is no active act!"));
}


/// ACT

void MainWindow::createAct()
{
    m_act = item_creator.createActItem(m_episode);
    createScene();
}

void MainWindow::slotCreateAct()
{
    if(m_episode)
        createAct();
    else
        QMessageBox::warning(this, tr("Warning"), tr("There is no active episode!"));
}

void MainWindow::slotCreateItem(bool t)
{
    if(t)
    {
        ui->graphicsView->setMouseTracking(true);
        m_scene->createItem();
    }
    else
    {
        ui->graphicsView->setMouseTracking(false);
        m_scene->endCreateItem();
    }
}

void MainWindow::slotItemCreated(QString title)
{
    QuestScene *qs = (QuestScene*)sender();

    QuestItem *q_item = item_creator.createItemItem(qs->sceneItem(), title);

    ui->treeWidget->setCurrentItem(q_item);
    ui->tableWidget->setQuestItem(q_item);

}

void MainWindow::slotTreeWidgetClicked(QTreeWidgetItem* item, int)
{
    QuestItem* q_item = dynamic_cast<QuestItem*>(item);

    if(q_item)
    {
        ui->tableWidget->setQuestItem(q_item);

        switch(q_item->itemType())
        {
        case QuestItem::TypeEpisode:
            break;
        case QuestItem::TypeAct:
            m_act = (ActItem*)q_item;
            break;
        case QuestItem::TypeScene:
            setActiveSceneFromItem((SceneItem*)(q_item));
            break;
        case QuestItem::TypeItemItem:
        case QuestItem::TypeItemBackGround:
            setActiveSceneFromItem((SceneItem*)(q_item->parent()));
            break;
        }
    }
}

void MainWindow::setActiveSceneFromItem(SceneItem* item)
{
    m_scene = item->scene();
    ui->graphicsView->setScene(m_scene);
}

