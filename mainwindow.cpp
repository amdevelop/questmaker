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
#include "interioritem.h"
#include "subjectitem.h"
#include "itembackground.h"
#include "questnamedialog.h"

#include "textdialog.h"

#ifdef WIN32
#include <qjson/src/parser.h>
#include <qjson/src/serializer.h>
#else
#include <qjson/parser.h>
#include <qjson/serializer.h>
#endif

#include "itemdialog.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_settings("questmaker.ini", QSettings::IniFormat)
{
    ui = new Ui::MainWindow;

    m_scene = 0;
    m_act = 0;
    m_episode = 0;


    ui->setupUi(this);

    //    item_creator.setPropetryWidget(ui->tableWidget);

    connect(ui->actionOpen, SIGNAL(triggered()), SLOT(slotFileOpen()));
    //connect(ui->actionCreate, SIGNAL(triggered()), SLOT(slotCreateScene()));

    connect(ui->actionPublish, SIGNAL(triggered()), SLOT(slotPublish()));

    connect(ui->actionCreate_episode, SIGNAL(triggered()), SLOT(slotCreateEpisode()));
    connect(ui->actionClose_episode, SIGNAL(triggered()), SLOT(slotCloseEpisode()));

    connect(ui->actionCreate_item, SIGNAL(toggled(bool)), SLOT(slotCreateItem(bool)));
    connect(ui->actionOpenFile, SIGNAL(triggered()), SLOT(slotOpenFileToProperties()));

    connect(ui->actionAdd_Interior, SIGNAL(triggered()), SLOT(slotOpenFileToInterior()));
    connect(ui->actionAdd_subject, SIGNAL(triggered()), SLOT(slotOpenFileToSubject()));

    connect(ui->actionMove_up, SIGNAL(triggered()), SLOT(slotMoveUp()));
    connect(ui->actionMove_down, SIGNAL(triggered()), SLOT(slotMoveDown()));

    connect(ui->actionZoom_In, SIGNAL(triggered()), SLOT(slotZoomIn()));
    connect(ui->actionZoom_Out, SIGNAL(triggered()), SLOT(slotZoomOut()));

//    connect(ui->treeView->selectionModel(),
//            SIGNAL(currentChanged(QModelIndex,QModelIndex)),
//            SLOT(slotCurrentChanged(QModelIndex,QModelIndex)));

    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), SLOT(slotTreeWidgetClicked(QModelIndex)));
    connect(ui->treeView, SIGNAL(createAct()), SLOT(slotCreateAct()));
    connect(ui->treeView, SIGNAL(createScene()), SLOT(slotCreateScene()));

    m_item_creator = 0;

    connect(ui->graphicsView,
            SIGNAL(itemCreated(QString,QPolygonF)),
            SLOT(slotItemCreated(QString,QPolygonF)));

//    ui->graphicsView->horizontalScrollBar()->hide();
//    ui->graphicsView->verticalScrollBar()->hide();

    connect(ui->tableView,
            SIGNAL(doubleClicked(QModelIndex)),
            SLOT(slotTableDoubleClicked(QModelIndex)));

    move(m_settings.value("WindowPosX", 0).toInt(),
         m_settings.value("WindowPosY", 0).toInt());
    resize(m_settings.value("WindowWidth", 500).toInt(),
           m_settings.value("WindowHeight", 500).toInt());

    ui->splitter->setSizes(QList<int>() <<
                           m_settings.value("SplitterSceneSize", 250).toInt() <<
                           m_settings.value("SplitterTreeSize", 250).toInt());

    ui->actionPublish->setEnabled(false);
    ui->actionOpenFile->setEnabled(false);
    ui->actionCreate_item->setEnabled(false);
    ui->actionAdd_Interior->setEnabled(false);
    ui->actionAdd_subject->setEnabled(false);
    ui->actionClose_episode->setEnabled(false);
    ui->actionZoom_In->setEnabled(false);
    ui->actionZoom_Out->setEnabled(false);

    ui->actionMove_up->setEnabled(false);
    ui->actionMove_down->setEnabled(false);

    QString last_quest = m_settings.value("LastWorkQuest").toString();

    if(!last_quest.isEmpty())
        openQuest(last_quest);

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
        openQuest(file_path);
}

void MainWindow::openQuest(QString file_path)
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
            m_settings.setValue("LastWorkQuest",
                                file_path);

            ui->treeView->setModel(0);
            ui->graphicsView->setModel(0);

            if(m_item_creator)
                delete m_item_creator;

            m_item_creator = new ItemCreator;

            ui->treeView->setModel(m_item_creator);

            ui->graphicsView->setSelectionModel(
                        ui->treeView->selectionModel()); // m_item_creator->selectionModel());

            connect(ui->treeView->selectionModel(),
                    SIGNAL(currentChanged(QModelIndex,QModelIndex)),
                    SLOT(slotCurrentChanged(QModelIndex,QModelIndex)));

            ui->graphicsView->setModel(m_item_creator);

            ui->actionPublish->setEnabled(true);
            ui->actionClose_episode->setEnabled(true);

            bool in, out;
            ui->graphicsView->isZoomEnabled(in, out);

            ui->actionZoom_In->setEnabled(in);
            ui->actionZoom_Out->setEnabled(out);

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

                ActItem *act_item = m_item_creator->createActItem();
                //createActEmpty();

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

                    SceneItem* scene_item = m_item_creator->createSceneItem(act_item, 0);

                    QVariantMap scene_map = scene_var.toMap();

                    foreach (QString key, scene_map.keys()) {
                        if(
                                scene_map.value(key).type() != QVariant::List &&
                                scene_map.value(key).type() != QVariant::Map )
                            if(key != "id")
                                scene_item->setProperty(key, scene_map.value(key));
                    }


                    QString scene_path = act_ath + "/" +
                            scene_map.value("id").toString() + "/";

                    scene_item->setProperty("background",
                                            scene_path +
                                            scene_map.value("background").toString());


                    QVariantList items = scene_map.value("items").toList();

                    foreach (QVariant items_var, items) {

                        QVariantMap item_map = items_var.toMap();

                        QString interior_path = scene_path +
                                item_map.value("id").toString() + "/" +
                                item_map.value("image").toString();

                        InteriorItem* tmp_item = 0;
                        if(item_map.value("type").toString() == "interior")
                        {
                            tmp_item = m_item_creator->createInteriorItem(
                                        scene_item,
                                        interior_path,
                                        ui->graphicsView->scene()->width(),
                                        ui->graphicsView->scene()->height());
                        }
                        else if(item_map.value("type").toString() == "subject")
                        {
                            SubjectItem* subj_item;
                            tmp_item = subj_item = m_item_creator->createSubjectItem(
                                        scene_item,
                                        interior_path,
                                        ui->graphicsView->scene()->width(),
                                        ui->graphicsView->scene()->height(),
                                        item_map.value("title").toString());

                            QVariantList polygons = item_map.value("polygons").toList();

                            foreach (QVariant one_poly_list, polygons) {
                                QPolygonF polygon;
                                foreach (QVariant point_var, one_poly_list.toList())
                                {
                                    polygon << QPointF(
                                                   point_var.toList()[0].toDouble(),
                                            point_var.toList()[1].toDouble());
                                }

                                m_item_creator->createItemItem(subj_item, "", polygon);
                            }
                        }

                        if(tmp_item)
                        {
                            tmp_item->setProperty("title",
                                                  item_map.value("title"));
                            tmp_item->setProperty("scene_scale_x",
                                                  item_map.value("scene_scale_x"));
                            tmp_item->setProperty("scene_scale_y",
                                                  item_map.value("scene_scale_y"));

                            tmp_item->setSceneX(item_map.value("scene_x").toReal());
                            tmp_item->setSceneY(item_map.value("scene_y").toReal());
                        }
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
    m_item_creator->setHorizontalHeaderLabels(
                QStringList() << tr("The same Quest"));
    connect(m_item_creator, SIGNAL(rowsRemoved(const QModelIndex&,
                                               int,
                                               int)),
            ui->graphicsView, SLOT(slotRowsRemoved(const QModelIndex&,
                                                   int,
                                                   int)));


    connect(m_item_creator,
            SIGNAL(sceneRemoved(SceneItem*)),
            ui->graphicsView,
            SLOT(slotSceneRemoved(SceneItem*)));

    ui->treeView->setModel(m_item_creator);

    ui->graphicsView->setSelectionModel(
                ui->treeView->selectionModel()); //m_item_creator->selectionModel());

    connect(ui->treeView->selectionModel(),
            SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            SLOT(slotCurrentChanged(QModelIndex,QModelIndex)));

    createAct();

    ui->treeView->expandAll();

    ui->actionPublish->setEnabled(true);
    ui->actionClose_episode->setEnabled(true);

    bool in, out;
    ui->graphicsView->isZoomEnabled(in, out);

    ui->actionZoom_In->setEnabled(in);
    ui->actionZoom_Out->setEnabled(out);

//    slotTreeWidgetClicked(m_item_creator->selectionModel()->currentIndex());
}

void MainWindow::slotCloseEpisode()
{
    if(m_item_creator)
    {
        ui->treeView->setModel(0);
        ui->graphicsView->setSelectionModel(0);
        ui->graphicsView->setModel(0);

        delete m_item_creator;
        m_item_creator = 0;

        ui->actionPublish->setEnabled(false);
        ui->actionOpenFile->setEnabled(false);
        ui->actionCreate_item->setEnabled(false);
        ui->actionAdd_Interior->setEnabled(false);
        ui->actionClose_episode->setEnabled(false);
        ui->actionZoom_In->setEnabled(false);
        ui->actionZoom_Out->setEnabled(false);
    }
}

void MainWindow::slotOpenFileToInterior()
{
        QString file_path = QFileDialog::getOpenFileName(this, "", m_settings.value("LastOpenedDir", QDir::homePath()).toString());

        if(!file_path.isEmpty())
        {
            QFileInfo fi(file_path);
            m_settings.setValue("LastOpenedDir", fi.canonicalFilePath());


            if(m_item_creator)
            {
                m_item_creator->createInteriorItem(ui->graphicsView->sceneItem(),
                                                   file_path,
                                                   ui->graphicsView->scene()->width(),
                                                   ui->graphicsView->scene()->height());

                ui->graphicsView->update();
            }
            else
            {
                QMessageBox::critical(this,
                                      tr("Critical"),
                                      tr("Can't open interior file"));
            }
        }
}

void MainWindow::slotOpenFileToSubject()
{
    QString file_path = QFileDialog::getOpenFileName(this, "", m_settings.value("LastOpenedDir", QDir::homePath()).toString());

    if(!file_path.isEmpty())
    {
        QFileInfo fi(file_path);
        m_settings.setValue("LastOpenedDir", fi.canonicalFilePath());


        if(m_item_creator)
        {
            m_item_creator->createSubjectItem(ui->graphicsView->sceneItem(),
                                              file_path,
                                              ui->graphicsView->scene()->width(),
                                              ui->graphicsView->scene()->height(),
                                              ItemDialog::getItemTitle());

            ui->graphicsView->update();
        }
        else
        {
            QMessageBox::critical(this,
                                  tr("Critical"),
                                  tr("Can't open interior file"));
        }
    }
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

//        ui->graphicsView->setSceneItem(scene_item);
//        ui->graphicsView->update();

        ui->graphicsView->setModel(m_item_creator);
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
//    InteriorItem* interior_item =
//            dynamic_cast<InteriorItem*>
//            (m_item_creator->itemFromIndex(
//                 m_item_creator->selectionModel()->currentIndex()));

    SubjectItem* subject_item = dynamic_cast<SubjectItem*>(m_interior_item);

    if(subject_item)
    {
        ItemItem *q_item = m_item_creator->createItemItem(subject_item,
                                                          title,
                                                          polygon);

        ui->treeView->setCurrentIndex(q_item->index());

        q_item->setProperty("title", title);
        ui->tableView->setModel(q_item->propertyModel());

        ui->graphicsView->update();

        return q_item;
    }

    return 0;
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
        QStandardItemModel* standart_model =
                (QStandardItemModel*)item.model();

        QStandardItem* standart_item =
                standart_model->itemFromIndex(item);

        switch (item.data(QuestItem::RoleEditType).toInt()) {
        case QuestItem::TypeValueFile:
        case QuestItem::TypeValueImage:
        {
            QString file_path = QFileDialog::getOpenFileName(this, "", m_settings.value("LastOpenedDir", QDir::homePath()).toString());

            if(!file_path.isEmpty())
            {
                standart_item->setData(file_path,
                                       Qt::DisplayRole);
            }
        }
            break;
        case QuestItem::TypeValueText:
        {
            QString text;
            if(TextDialog::getText(text,
                                   standart_item->data(Qt::DisplayRole).toString()))
                standart_item->setData(text,
                                       Qt::DisplayRole);
        }
        default:
            break;
        }
    }
}

void MainWindow::slotTreeWidgetClicked(QModelIndex item)
{
    m_interior_item = 0; // wat?!

    if(m_item_creator)
    {
        QuestItem* q_item = dynamic_cast<QuestItem*>(m_item_creator->itemFromIndex(item));

        if(q_item)
        {
            ui->tableView->setModel(q_item->propertyModel());

            setActiveSceneFromItem(q_item);

            switch(q_item->itemType())
            {
            case QuestItem::TypeEpisode:

                ui->actionAdd_Interior->setEnabled(false);
                ui->actionAdd_subject->setEnabled(false);
                ui->actionCreate_item->setEnabled(false);
                if(ui->actionCreate_item->isChecked())
                    ui->actionCreate_item->setChecked(false);

                ui->actionOpenFile->setEnabled(false);

                break;
            case QuestItem::TypeAct:
                m_act = (ActItem*)q_item;

                ui->actionAdd_Interior->setEnabled(false);
                ui->actionAdd_subject->setEnabled(false);
                ui->actionCreate_item->setEnabled(false);
                if(ui->actionCreate_item->isChecked())
                    ui->actionCreate_item->setChecked(false);
                ui->actionOpenFile->setEnabled(false);

                break;
            case QuestItem::TypeScene:

                ui->actionAdd_Interior->setEnabled(true);
                ui->actionAdd_subject->setEnabled(true);
                ui->actionCreate_item->setEnabled(false);
                if(ui->actionCreate_item->isChecked())
                    ui->actionCreate_item->setChecked(false);
                ui->actionOpenFile->setEnabled(true);

                break;
            case QuestItem::TypeInterior:
                m_interior_item = (InteriorItem*)q_item;

                ui->actionAdd_Interior->setEnabled(false);
                ui->actionAdd_subject->setEnabled(false);

                ui->actionCreate_item->setEnabled(false);
                if(ui->actionCreate_item->isChecked())
                    ui->actionCreate_item->setChecked(false);
                ui->actionOpenFile->setEnabled(false);
                break;
            case QuestItem::TypeSubject:
                m_interior_item = (InteriorItem*)q_item;

                ui->actionAdd_Interior->setEnabled(false);
                ui->actionAdd_subject->setEnabled(false);

                ui->actionCreate_item->setEnabled(true);
                ui->actionOpenFile->setEnabled(false);
                break;
            case QuestItem::TypeItemItem:

                ui->actionAdd_Interior->setEnabled(false);
                ui->actionCreate_item->setEnabled(false);
                if(ui->actionCreate_item->isChecked())
                    ui->actionCreate_item->setChecked(false);
                ui->actionOpenFile->setEnabled(false);

            case QuestItem::TypeItemBackGround:
                break;
            }
        }
    }
}

void MainWindow::setActiveSceneFromItem(QuestItem* item)
{
    SceneItem* tmp_item = dynamic_cast<SceneItem*>(item);

    if(!tmp_item)
        while(0 != item)
        {
            item = (QuestItem*)item->parent();
            if(0 != (tmp_item = dynamic_cast<SceneItem*>(item)))
                break;
        }

//    ui->graphicsView->setSceneItem(tmp_item);
//    ui->graphicsView->update();
}

/**
 * @brief этот слот нужен чтобы одновилась таблица свойств
 * после того, как были удалены строки; визуализатор обновляется
 * по сигналу от модели rowsRemoved(...)
 * @param current
 */

void MainWindow::slotCurrentChanged(const QModelIndex& current,
                                    const QModelIndex&)
{
    QuestItem *q_item = (QuestItem*)
            m_item_creator->itemFromIndex(current);

    if(q_item)
        ui->tableView->setModel(q_item->propertyModel());
}

void MainWindow::slotZoomIn()
{
    ui->graphicsView->zoom(SceneVisualizer::ZoomIn);

    bool in, out;
    ui->graphicsView->isZoomEnabled(in, out);

    ui->actionZoom_In->setEnabled(in);
    ui->actionZoom_Out->setEnabled(out);
}

void MainWindow::slotZoomOut()
{
    ui->graphicsView->zoom(SceneVisualizer::ZoomOut);

    bool in, out;
    ui->graphicsView->isZoomEnabled(in, out);

    ui->actionZoom_In->setEnabled(in);
    ui->actionZoom_Out->setEnabled(out);
}

void MainWindow::slotMoveUp()
{
//    if(m_interior_item)
//    {
//        if(m_interior_item->row() != 0)
//        {
//            QStandardItem* parent_item =
//                    m_interior_item->parent();

//            QStandardItem* tmp_item =
//                    parent_item->takeChild(m_interior_item->row());

//            parent_item->insertRow(tmp_item->row() - 1,
//                                   tmp_item);
//            parent_item->takeRow(m_interior_item->row());
//        }
        //        m_item_creator->insertRow();

//    }
}

void MainWindow::slotMoveDown()
{

}
