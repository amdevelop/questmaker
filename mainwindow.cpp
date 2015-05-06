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
#include "questnamedialog.h"

#include <qjson/serializer.h>


#include <QDebug>

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

//    item_creator.setPropetryWidget(ui->tableWidget);

    connect(ui->actionOpen, SIGNAL(triggered()), SLOT(slotFileOpen()));
    connect(ui->actionCreate, SIGNAL(triggered()), SLOT(slotCreateScene()));

    connect(ui->actionPublish, SIGNAL(triggered()), SLOT(slotPublish()));

    connect(ui->actionCreate_episode, SIGNAL(triggered()), SLOT(slotCreateEpisode()));

    connect(ui->toolCreateItem, SIGNAL(toggled(bool)), SLOT(slotCreateItem(bool)));

    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), SLOT(slotTreeWidgetClicked(QModelIndex)));
    connect(ui->treeView, SIGNAL(createAct()), SLOT(slotCreateAct()));
    connect(ui->treeView, SIGNAL(createScene()), SLOT(slotCreateScene()));

    ui->treeView->setModel(&item_creator);

    ui->graphicsView->horizontalScrollBar()->hide();
    ui->graphicsView->verticalScrollBar()->hide();

    move(m_settings.value("WindowPosX", 0).toInt(), m_settings.value("WindowPosY", 0).toInt());
    resize(m_settings.value("WindowWidth", 500).toInt(), m_settings.value("WindowHeight", 500).toInt());

    ui->splitter->setSizes(QList<int>() <<
                           m_settings.value("SplitterSceneSize", 250).toInt() <<
                           m_settings.value("SplitterTreeSize", 250).toInt());

//    ui->tableWidget->verticalHeader()->hide();

//    ui->tableWidget->setColumnCount(2);
//    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Value"));
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

void MainWindow::slotPublish()
{
    QString save_file_path = QFileDialog::getExistingDirectory(this, "", m_settings.value("SaveDir", QDir::homePath()).toString());

    m_settings.setValue("SaveDir",save_file_path);

    QString quest_name;
    quest_name = QuestNameDialog::getQuestSaveName(this);

    if(!quest_name.isEmpty())
    {
        save_file_path += "/" + quest_name + ".json";

        QFile file(save_file_path);
        if(file.open(QIODevice::WriteOnly))
        {
            bool ok;
            QJson::Serializer().serialize(item_creator.toJson(), &file, &ok);

            qDebug() << QJson::Serializer().serialize(item_creator.toJson());

            if(!ok)
                QMessageBox::critical(0, tr("Critical!"), tr("Can't save quest!"));
        }
    }
}


void MainWindow::slotCreateEpisode()
{
//    m_episode = new EpisodeItem(ui->treeWidget->invisibleRootItem());
//    m_episode->setData(0, Qt::DisplayRole, tr("Episode"));

    createAct();

    ui->treeView->expandAll();
}

/// SCENE

void MainWindow::createScene()
{
    m_scene = new QuestScene;
    connect(m_scene, SIGNAL(itemCreated(QString,QPolygonF)), SLOT(slotItemCreated(QString,QPolygonF)));
    m_scene->setSceneRect(0, 0, 300, 200);

    SceneItem* scene_item = item_creator.createSceneItem(m_act, m_scene);
    scene_item->setData(m_scene->title(), Qt::DisplayRole);

    m_scene->setSceneItem(scene_item);

    ui->graphicsView->setScene(m_scene);

//    ui->treeView->setCurrentItem(scene_item);

    ui->tableView->setModel(scene_item->propertyModel());
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
    m_act = item_creator.createActItem();
    createScene();
}

void MainWindow::slotCreateAct()
{
        createAct();
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

void MainWindow::slotItemCreated(QString title, QPolygonF polygon)
{
    QuestScene *qs = (QuestScene*)sender();

    ItemItem *q_item = item_creator.createItemItem(qs->sceneItem(), title, polygon);

    ui->treeView->setCurrentIndex(q_item->index());
    ui->tableView->setModel(q_item->propertyModel());
}

void MainWindow::slotTreeWidgetClicked(QModelIndex item)
{
    QuestItem* q_item = dynamic_cast<QuestItem*>(item_creator.itemFromIndex(item));

    if(q_item)
    {
        ui->tableView->setModel(q_item->propertyModel());

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

