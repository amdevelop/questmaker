#include "makerlite.h"
#include "ui_makerlite.h"

MakerLite::MakerLite(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MakerLite)
{
    ui->setupUi(this);


//    connect()
}

MakerLite::~MakerLite()
{
    delete ui;
}
