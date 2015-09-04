#include "subjectitem.h"

SubjectItem::SubjectItem(QString)
    : InteriorItem(QObject::tr("Subject") + QString::number(InteriorItem::getItemCounter()),
                   QuestItem::TypeSubject)
{
}
