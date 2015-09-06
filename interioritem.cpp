#include "interioritem.h"

#include <QDir>

int InteriorItem::interior_counter = 0;

InteriorItem::InteriorItem(QString title, QuestItem::ItemType type) :
    QuestItem(type,
        (title.isEmpty())?(QObject::tr("Interior") + QString::number(getItemCounter())):(title))
{
    m_id = getItemCounter(true);

    m_scene_x = 0.0;
    m_scene_y = 0.0;

    setProperty("type", "interior");
}

QVariant InteriorItem::toJson(QString save_path, bool *)
{
    QVariantMap item_map = properties();

    QString new_dir_path = save_path + "/" + QString::number(m_id);
    QDir dir(new_dir_path);
    if(!dir.exists())
    {
        if(dir.mkdir(new_dir_path))
        {
            QFileInfo copy_fi(property("image").toString());
            QFile::copy(property("image").toString(),
                        new_dir_path + "/" + copy_fi.fileName());

            // DIRTY hack!
            item_map.insert("image", copy_fi.fileName());
        }

        item_map.insert("id", m_id);
        item_map.insert("scene_x", m_scene_x);
        item_map.insert("scene_y", m_scene_y);
    }

    return item_map;
}
