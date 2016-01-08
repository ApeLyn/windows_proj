#include "preferences.h"
#include "ui_preferences.h"
#include "mainwindow.h"
#include <QSettings>

preferences::preferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::preferences)
{
    ui->setupUi(this);

    switch2gen();

    connect(ui->generalButton,SIGNAL(clicked(bool)),this,SLOT(switch2gen()));
    connect(ui->editButton,SIGNAL(clicked(bool)),this,SLOT(switch2edit()));
    connect(ui->themeButton,SIGNAL(clicked(bool)),this,SLOT(switch2theme()));
    connect(ui->cssButton,SIGNAL(clicked(bool)),this,SLOT(switch2css()));
    connect(ui->t_part2,SIGNAL(activated(int)),this,SLOT(change_theme()));
    connect(ui->c_part2,SIGNAL(activated(int)),this,SLOT(change_css()));

}

void preferences::switch2gen() {
    set_env('g', true);
    set_env('e', false);
    set_env('t', false);
    set_env('c', false);

    return;
}

void preferences::switch2edit() {
    set_env('g', false);
    set_env('e', true);
    set_env('t', false);
    set_env('c', false);

    return;
}

void preferences::switch2theme() {
    set_env('g', false);
    set_env('e', false);
    set_env('t', true);
    set_env('c', false);

    return;
}

void preferences::switch2css() {
    set_env('g', false);
    set_env('e', false);
    set_env('t', false);
    set_env('c', true);

    return;
}

void preferences::set_env(char env_id, bool env) {

    switch (env_id) {
        case 'g': {
            ui->g_part1->setVisible(env);
            ui->g_part2->setVisible(env);
            ui->g_part3->setVisible(env);
            ui->g_part4->setVisible(env);
            ui->g_part5->setVisible(env);
            ui->g_part6->setVisible(env);
            ui->g_part7->setVisible(env);
            break;
        }
        case 'c': {
            ui->c_part1->setVisible(env);
            ui->c_part2->setVisible(env);
            ui->c_part3->setVisible(env);
            break;
        }
        case 'e': {
            ui->e_part1->setVisible(env);
            ui->e_part2->setVisible(env);
            ui->e_part3->setVisible(env);
            ui->e_part4->setVisible(env);
            ui->e_part5->setVisible(env);
            ui->e_part6->setVisible(env);
            ui->e_part7->setVisible(env);
            ui->e_part8->setVisible(env);
            ui->e_part9->setVisible(env);
            break;
        }
        case 't': {
            ui->t_part1->setVisible(env);
            ui->t_part2->setVisible(env);
            ui->t_part3->setVisible(env);
            break;
        }
    }

    return;
}

/*
void preferences::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange: {
            ui->retranslateUi(this);
            break;
        }
        default: {
            break;
        }
    }
}
*/

void preferences::change_theme() {
    int value = ui->t_part2->currentIndex();
    MainWindow *parent = (MainWindow*)(this->parent());
    parent->change_theme(value);
}

void preferences::change_css() {
    int value = ui->c_part2->currentIndex();
    MainWindow *parent = (MainWindow*)(this->parent());
    parent->change_css(value);
}

void preferences::writeSettings(QSettings &settings, const QString &prefix) {
    settings.beginGroup(prefix);
    settings.setValue(THEME, ui->t_part2->currentIndex());
    // settings.setValue();
    settings.endGroup();
}

void preferences::readSettings(QSettings &settings, const QString &prefix) {
    settings.beginGroup(prefix);
    ui->t_part2->setCurrentIndex(settings.value(THEME,0).toInt());
    ui->t_part2->activated(settings.value(THEME,0).toInt());
    settings.endGroup();
}

preferences::~preferences()
{
    delete ui;
}
