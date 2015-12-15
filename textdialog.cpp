#include "textdialog.h"
#include "ui_textdialog.h"

TextDialog::TextDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextDialog)
{
    ui->setupUi(this);

    m_ok = false;

    connect(ui->buttonBox,
            SIGNAL(accepted()),
            SLOT(slotAccepted()));

    connect(ui->buttonBox,
            SIGNAL(rejected()),
            SLOT(slotRejected()));
}

TextDialog::~TextDialog()
{
    delete ui;
}

bool TextDialog::getText(QString &text,
                         QString initial_text,
                         QWidget *parent)
{
    TextDialog td(parent);

    td.ui->textEdit->setText(initial_text);

    td.exec();

    text = td.ui->textEdit->toPlainText();

    return td.isOk();
}

QString TextDialog::text() const
{
    return ui->textEdit->toPlainText();
}


void TextDialog::slotAccepted()
{
    m_ok = true;
}

void TextDialog::slotRejected()
{
    m_ok = false;
}

