#ifndef ITEMDIALOG_H
#define ITEMDIALOG_H

#include <QDialog>

namespace Ui {
    class ItemDialog;
}

class ItemDialog : public QDialog
{
    Q_OBJECT

    static int item_counter;

    explicit ItemDialog(QWidget *parent = 0);
    ~ItemDialog();
public:
    static QString getItemTitle(QWidget *parent = 0);

private:
    Ui::ItemDialog *ui;
};

#endif // ITEMDIALOG_H
