#ifndef TEXTDIALOG_H
#define TEXTDIALOG_H

#include <QDialog>

namespace Ui {
class TextDialog;
}

class TextDialog : public QDialog
{
    Q_OBJECT

    bool m_ok;
    
public:
    explicit TextDialog(QWidget *parent = 0);
    ~TextDialog();

    static bool getText(QString &text,
                        QString initial_text,
                        QWidget *parent = 0);

    QString text() const;

    bool isOk() const
    {
        return m_ok;
    }
    
private:
    Ui::TextDialog *ui;

private slots:
    void slotAccepted();
    void slotRejected();
};

#endif // TEXTDIALOG_H
