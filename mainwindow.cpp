#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //file
    connect(ui->actionNew,SIGNAL(clicked(bool)),this,SLOT(fnew()));
    connect(ui->actionOpen,SIGNAL(clicked(bool)),this,SLOT(fopen()));
    connect(ui->actionOpen_Recent,SIGNAL(clicked(bool)),this,SLOT(fopenRecent()));
    connect(ui->actionClose,SIGNAL(clicked(bool)),this,SLOT(fclose()));
    connect(ui->actionSave,SIGNAL(clicked(bool)),this,SLOT(fsave()));
    connect(ui->actionRename,SIGNAL(clicked(bool)),this,SLOT(frename()));
    //edit
    connect(ui->actionUndo,SIGNAL(clicked(bool)),this,SLOT(eundo()));
    connect(ui->actionRedo,SIGNAL(clicked(bool)),this,SLOT(eredo()));
    connect(ui->actionDelete,SIGNAL(clicked(bool)),this,SLOT(edelete()));
    connect(ui->actionFind,SIGNAL(clicked(bool)),this,SLOT(efind()));
    //view
    connect(ui->actionLeft_right_1_1,SIGNAL(clicked(bool)),this,SLOT(vlf_1_1()));
    connect(ui->actionLeft_right_1_2,SIGNAL(clicked(bool)),this,SLOT(vlf_1_2()));
    connect(ui->actionLeft_right_2_1,SIGNAL(clicked(bool)),this,SLOT(vlf_2_1()));
    //help
    connect(ui->actionMou_help,SIGNAL(clicked(bool)),this,SLOT(help()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
