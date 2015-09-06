#include "subjectitem.h"

SubjectItem::SubjectItem(QString title)
    : InteriorItem(title.isEmpty()?(QObject::tr("Subject") + QString::number(getItemCounter(true))):(title),
                   QuestItem::TypeSubject)
{
    setProperty("type", "subject");
}

QVariant SubjectItem::toJson(QString save_path, bool *ok)
{
    QVariantMap subj_item =
            InteriorItem::toJson(save_path, ok).toMap();

    QVariantList polygon_list;
    for(int i = 0 ; i < rowCount(); i++)
    {
        QuestItem *q_item = (QuestItem*)child(i);
        polygon_list << q_item->toJson(save_path);
    }

    subj_item.insert("polygons", polygon_list);

    return subj_item;
}
