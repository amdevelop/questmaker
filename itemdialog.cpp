#include "itemdialog.h"
#include "ui_itemdialog.h"

int ItemDialog::item_counter = 0;

ItemDialog::ItemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ItemDialog)
{
    ui->setupUi(this);
}

ItemDialog::~ItemDialog()
{
    delete ui;
}

QString ItemDialog::getItemTitle(QWidget *parent)
{
    ItemDialog dlg(parent);
    dlg.exec();

    if(dlg.ui->leItemName->text().isEmpty())
        return "Item" + QString::number(item_counter++);
    else
        return dlg.ui->leItemName->text();
}
