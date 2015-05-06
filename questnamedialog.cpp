#include "questnamedialog.h"
#include "ui_questnamedialog.h"

QuestNameDialog::QuestNameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuestNameDialog)
{
    ui->setupUi(this);


    connect(ui->buttonBox, SIGNAL(accepted()), SLOT(slotAccepted()));
    connect(ui->buttonBox, SIGNAL(rejected()), SLOT(slotRejected()));

    m_accepted = false;
}

QuestNameDialog::~QuestNameDialog()
{
    delete ui;
}

QString QuestNameDialog::getQuestSaveName(QWidget *parent)
{
    QuestNameDialog dlg(parent);
    dlg.exec();

    if(dlg.isAccepted())
        return dlg.ui->lineEdit->text();
    else
        return "";
}

void QuestNameDialog::slotAccepted()
{
    m_accepted = true;
    close();
}

void QuestNameDialog::slotRejected()
{
    close();
}
