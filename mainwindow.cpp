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

#include <qjson/parser.h>
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
    connect(ui->toolOpenFile, SIGNAL(clicked()), SLOT(slotOpenFileToProperties()));

    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), SLOT(slotTreeWidgetClicked(QModelIndex)));
    connect(ui->treeView, SIGNAL(createAct()), SLOT(slotCreateAct()));
    connect(ui->treeView, SIGNAL(createScene()), SLOT(slotCreateScene()));

    m_item_creator = 0;

    connect(ui->graphicsView, SIGNAL(itemCreated(QString,QPolygonF)), SLOT(slotItemCreated(QString,QPolygonF)));
    ui->graphicsView->horizontalScrollBar()->hide();
    ui->graphicsView->verticalScrollBar()->hide();

    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), SLOT(slotTableDoubleClicked(QModelIndex)));


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
        m_settings.setValue("LastOpenedDir", fi.path());

        QFile file(file_path);
        if(file.open(QIODevice::ReadOnly))
        {
            QJson::Parser p;
            QVariant json_data = p.parse(&file);
            QString episode_path = m_settings.value("LastOpenedDir").toString();

            if(json_data.type() != QVariant::Invalid)
            {
                ui->treeView->setModel(0);
                delete m_item_creator;
                m_item_creator = 0;

                m_item_creator = new ItemCreator;
                ui->treeView->setModel(m_item_creator);


                QVariantMap episode = json_data.toMap().value("episode").toMap();


                foreach (QString key, episode.keys()) {

                    if(
                            episode.value(key).type() != QVariant::List &&
                            episode.value(key).type() != QVariant::Map )
                        if(key != "id")
                            m_item_creator->episodeItem()->setProperty(key, episode.value(key));
                }

                m_item_creator->episodeItem()->setProperty("cover",
                                                           episode_path + "/" + episode.value("cover").toString());


                QVariantList acts = episode.value("acts").toList();

                foreach (QVariant act_var, acts) {

                    QuestItem *act_item = createActEmpty();


                    QVariantMap act_map = act_var.toMap();
                    QString act_ath = episode_path + "/" + act_map.value("id").toString();

                    foreach (QString key, act_map.keys()) {
                        if(
                                act_map.value(key).type() != QVariant::List &&
                                act_map.value(key).type() != QVariant::Map )
                            if(key != "id")
                                act_item->setProperty(key, act_map.value(key));
                    }

                    QVariantList scenes = act_map
                            .value("scenes")
                            .toList();

                    foreach (QVariant scene_var, scenes) {

                        QuestItem *scene_item = createScene();

                        QVariantMap scene_map = scene_var.toMap();

                        foreach (QString key, scene_map.keys()) {
                            if(
                                    scene_map.value(key).type() != QVariant::List &&
                                    scene_map.value(key).type() != QVariant::Map )
                                if(key != "id")
                                    scene_item->setProperty(key, scene_map.value(key));
                        }


                        QString scene_path = act_ath + "/" +
                                scene_map.value("id").toString() + "/" +
                                scene_map.value("background").toString();

                        scene_item->setProperty("background", scene_path);


                        QVariantList items = scene_map.value("items").toList();

                        foreach (QVariant items_var, items) {

                            QVariantMap item_map = items_var.toMap();
                            QPolygonF polygon;
                            foreach (QVariant point_var, item_map.value("polygon").toList())
                            {
                                polygon << QPointF(
                                               point_var.toList()[0].toInt(),
                                        point_var.toList()[1].toInt());
                            }

                            QuestItem *item_item = createItem(
                                        item_map.value("title").toString(),
                                        polygon);

                            foreach (QString key, item_map.keys()) {
                                if(
                                        item_map.value(key).type() != QVariant::List &&
                                        item_map.value(key).type() != QVariant::Map )
                                    if(key != "id")
                                        item_item->setProperty(key, item_map.value(key));
                            }

                            if(item_map.value("detail").type() != QVariant::Invalid &&
                                    item_map.value("detail").toString() != "")
                                item_item->setProperty("detail",
                                                       scene_path + "/" + item_map.value("detail").toString());
                        }
                    }
                }

            }
            else
            {
                QMessageBox::critical(this,
                                      tr("Critical!"),
                                      tr("Bad JSON-data!"));
            }
        }
    }
}

void MainWindow::slotOpenFileToProperties()
{
    QString file_path = QFileDialog::getOpenFileName(this, "", m_settings.value("LastOpenedDir", QDir::homePath()).toString());

    if(!file_path.isEmpty())
    {
        QFileInfo fi(file_path);
        m_settings.setValue("LastOpenedDir", fi.canonicalFilePath());

        if(ui->tableView->currentIndex() != QModelIndex())
        {
            QStandardItemModel* std_model;
            std_model = (QStandardItemModel*)ui->tableView->model();

            QStandardItem* tmp_item =
                    std_model->itemFromIndex(ui->tableView->currentIndex());

            if(tmp_item)
                tmp_item->setData(file_path, Qt::DisplayRole);

            ui->graphicsView->update();

            //            tmp_item
        }
    }
}

void MainWindow::slotPublish()
{
    if(m_item_creator)
    {
        QString save_file_path = QFileDialog::getExistingDirectory(this, "", m_settings.value("SaveDir", QDir::homePath()).toString());

        m_settings.setValue("SaveDir",save_file_path);

        QString quest_name;
        quest_name = QuestNameDialog::getQuestSaveName(this);

        if(!quest_name.isEmpty())
        {
            QString quest_dir_path = save_file_path + "/" + quest_name;
            QDir dir(quest_dir_path);

            if(!dir.exists())
            {
                if(dir.mkdir(quest_dir_path))
                {
                    save_file_path = quest_dir_path + "/" + "quest.json";

                    QFile file(save_file_path);
                    if(file.open(QIODevice::WriteOnly))
                    {
                        bool ok;
                        QJson::Serializer().serialize(m_item_creator->toJson(quest_dir_path), &file, &ok);

                        qDebug() << QJson::Serializer().serialize(m_item_creator->toJson(quest_dir_path));

                        if(!ok)
                            QMessageBox::critical(0, tr("Critical!"), tr("Can't save quest!"));
                    }
                }
            }
        }
    }
}


void MainWindow::slotCreateEpisode()
{
    if(m_item_creator)
    {
        delete m_item_creator;
        ui->treeView->setModel(0);
        m_item_creator = 0;
    }

    m_item_creator = new ItemCreator(this);
    ui->treeView->setModel(m_item_creator);
    createAct();

    ui->treeView->expandAll();
}

/// SCENE

QuestItem* MainWindow::createScene()
{
    SceneItem* scene_item = 0;
    if(m_item_creator)
    {
        scene_item = m_item_creator->createSceneItem(m_act, m_scene);
        scene_item->setData(tr("Scene") + QString::number(scene_item->id()),
                            Qt::DisplayRole);

        ui->graphicsView->setSceneItem(scene_item);
        ui->graphicsView->update();
        ui->treeView->setCurrentIndex(scene_item->index());
        ui->tableView->setModel(scene_item->propertyModel());
    }

    return scene_item;
}

void MainWindow::slotCreateScene()
{
    if(m_act)
        createScene();
    else
        QMessageBox::warning(this, tr("Warning"), tr("There is no active act!"));
}


/// ACT

QuestItem* MainWindow::createAct()
{
    if(m_item_creator)
    {
        m_act = m_item_creator->createActItem();
        createScene();
    }

    return m_act;
}

QuestItem* MainWindow::createActEmpty()
{
    if(m_item_creator)
    {
        m_act = m_item_creator->createActItem();
        return m_act;
    }
    else
        return 0;
}

/// ITEM

QuestItem* MainWindow::createItem(QString title, QPolygonF polygon)
{
    ItemItem *q_item = m_item_creator->createItemItem(ui->graphicsView->sceneItem(),
                                                      title,
                                                      polygon);

    ui->treeView->setCurrentIndex(q_item->index());

    q_item->setProperty("title", title);
    ui->tableView->setModel(q_item->propertyModel());

    ui->graphicsView->update();

    return q_item;
}


void MainWindow::slotCreateAct()
{
    createAct();
}

void MainWindow::slotCreateItem(bool t)
{
    if(t)
        ui->graphicsView->createItem();
    else
        ui->graphicsView->endCreateItem();
}

void MainWindow::slotItemCreated(QString title, QPolygonF polygon)
{
    if(m_item_creator)
        createItem(title, polygon);
}

void MainWindow::slotTableDoubleClicked(QModelIndex item)
{
    if(item.column() == 1)
    {
        qDebug() << item.data(QuestItem::RoleEditType).toInt();
        switch (item.data(QuestItem::RoleEditType).toInt()) {
        case QuestItem::TypeValueFileType:
            QFileDialog::getOpenFileName(this, "", m_settings.value("LastOpenedDir", QDir::homePath()).toString());
            break;
        default:
            break;
        }
    }
}

void MainWindow::slotTreeWidgetClicked(QModelIndex item)
{
    if(m_item_creator)
    {
        QuestItem* q_item = dynamic_cast<QuestItem*>(m_item_creator->itemFromIndex(item));

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
}

void MainWindow::setActiveSceneFromItem(SceneItem* item)
{
    ui->graphicsView->setSceneItem(item);
    ui->graphicsView->update();
}

