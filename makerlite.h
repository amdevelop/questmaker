#ifndef MAKERLITE_H
#define MAKERLITE_H

#include <QtWidgets/QMainWindow>

namespace Ui {
class MakerLite;
}

class MakerLite : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MakerLite(QWidget *parent = 0);
    ~MakerLite();
    
private:
    Ui::MakerLite *ui;
};

#endif // MAKERLITE_H
