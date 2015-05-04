#ifndef PROPERTYWIDGET_H
#define PROPERTYWIDGET_H

#include <QTableWidget>

class QuestItem;

class PropertyWidget : public QTableWidget
{
    Q_OBJECT

    QuestItem *m_item;

    enum
    {
        RoleKey = Qt::UserRole + 1
    };

    void updateTable();

public:
    PropertyWidget(QWidget* parent = 0);

    void setQuestItem(QuestItem*);

private slots:
    void slotPropertyChanged();
};

#endif // PROPERTYWIDGET_H
