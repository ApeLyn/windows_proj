#include "preferences.h"
#include "ui_preferences.h"

preferences::preferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::preferences)
{
    ui->setupUi(this);
}

preferences::~preferences()
{
    delete ui;
}
