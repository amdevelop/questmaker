#include "propertywidget.h"

#include "questitem.h"

PropertyWidget::PropertyWidget(QWidget* parent)
    : QTableWidget(parent)
{
    setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Value"));
}

void PropertyWidget::setQuestItem(QuestItem* q_item)
{
    for(int i = 0; i < rowCount(); i++)
    {
        m_item->setProperty(item(i, 0)->data(Qt::DisplayRole).toString(),
                            item(i, 1)->data(Qt::DisplayRole));
    }

    clear();
    setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Value"));

    m_item = q_item;

    if(m_item)
        updateTable();
}

void PropertyWidget::updateTable()
{
    setRowCount(m_item->properties().count());

    int j = 0;
    foreach(QString key, m_item->properties().keys())
    {
        setItem(j, 0, new QTableWidgetItem(key));
        setItem(j, 1, new QTableWidgetItem(m_item->properties().value(key).toString()));

        j++;
    }
}

void PropertyWidget::slotPropertyChanged()
{
    if(sender() == m_item->notifier())
        updateTable();
}


