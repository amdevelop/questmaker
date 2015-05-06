#ifndef QUESTNAMEDIALOG_H
#define QUESTNAMEDIALOG_H

#include <QDialog>

namespace Ui {
class QuestNameDialog;
}

class QuestNameDialog : public QDialog
{
    Q_OBJECT

    bool m_accepted;
    
    explicit QuestNameDialog(QWidget *parent = 0);
    ~QuestNameDialog();

    bool isAccepted() const
    {
        return m_accepted;
    }

public:
    static QString getQuestSaveName(QWidget *parent = 0);
    
private:
    Ui::QuestNameDialog *ui;

private slots:
    void slotAccepted();
    void slotRejected();
};

#endif // QUESTNAMEDIALOG_H
