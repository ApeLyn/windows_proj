#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>
#include <QLabel>
#include <QSettings>
#include <QTextStream>
#include "finddialog.h"
#include "findreplacedialog.h"
#include "preferences.h"
#include "highlighter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), maxFileNr(10)
{
    imageLabel = new QLabel;
    QCoreApplication::setOrganizationName("Shieh");
    QCoreApplication::setOrganizationDomain("shieh.com");
    QCoreApplication::setApplicationName("WM");

    // setCentralWidget(imageLabel);

    ui->setupUi(this);
    //file
    connect(ui->actionNew,SIGNAL(triggered()),this,SLOT(fnew()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(fopen()));
    // connect(ui->actionOpen_Recent,SIGNAL(triggered()),this,SLOT(fopenRecent()));
    connect(ui->actionClose,SIGNAL(clicked(bool)),this,SLOT(fclose()));
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(fsave()));
    connect(ui->actionRename,SIGNAL(clicked(bool)),this,SLOT(frename()));
    //edit
    connect(ui->actionUndo,SIGNAL(triggered()),this,SLOT(eundo()));
    connect(ui->actionRedo,SIGNAL(triggered()),this,SLOT(eredo()));
    connect(ui->actionDelete,SIGNAL(clicked(bool)),this,SLOT(edelete()));
    connect(ui->actionFind,SIGNAL(triggered()),this,SLOT(efind()));
    connect(ui->actionFind_Replace, SIGNAL(triggered()), this, SLOT(efindreplace()));
    //view
    connect(ui->actionLeft_right_1_1,SIGNAL(triggered()),this,SLOT(vlf_1_1()));
    connect(ui->actionLeft_right_1_2,SIGNAL(triggered()),this,SLOT(vlf_1_2()));
    connect(ui->actionLeft_right_2_1,SIGNAL(triggered()),this,SLOT(vlf_2_1()));
    //help
    connect(ui->actionMou_help,SIGNAL(clicked(bool)),this,SLOT(help()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about()));

    //WinMarker
    connect(ui->actionPreferences,SIGNAL(triggered()),this,SLOT(wpreferences()));
    //connect(ui->actionQuit,SIGNAL(clicked(bool)),this,SLOT(wquit()));

    ui->actionUndo->setShortcuts(QKeySequence::Undo);
    ui->actionRedo->setShortcuts(QKeySequence::Redo);

    change_theme(0);
    highlighter = new Highlighter(ui->plainTextEdit->document());

    createActionsAndConnections();
}


void MainWindow::change_theme(int theme) {

    switch (theme) {
        case 0: {
            ui->plainTextEdit->setStyleSheet("background: white;color: black;font-size: 15px;");
            break;
        }
        case 1: {
            ui->plainTextEdit->setStyleSheet("background: white;color: black;font-size: 22px;");
            break;
        }
        case 2: {
            ui->plainTextEdit->setStyleSheet("background: black;color: white;font-size: 15px;");
            break;
        }
        case 3: {
            ui->plainTextEdit->setStyleSheet("background: black;color: white;font-size: 22px;");
            break;
        }
        case 4: {
            ui->plainTextEdit->setStyleSheet("background: white;color: blue;");
            break;
        }
    }

    return;
}


void MainWindow::fnew()
{
    fsave();
}


void MainWindow::fopen()
{
    QString filePath = QFileDialog::getOpenFileName(
                       this, tr("Open File"), "",
                       tr("text files (*.md *.txt)"));
    if (!filePath.isEmpty()) {
        loadFile(filePath);
    }
}


void MainWindow::loadFile(const QString &filePath){
    QFile inputFile(filePath);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          ui->plainTextEdit->appendPlainText(line);
       }
       inputFile.close();
    }
    this->setWindowTitle(filePath);

    adjustForCurrentFile(filePath);
}


void MainWindow::fopenRecent(){
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        loadFile(action->data().toString());
    }
}


void MainWindow::createActionsAndConnections(){
    // ui->actionOpen->setShortcut(QKeySequence::Open);
    QAction* recentFileAction = 0;
    for(int i = 0; i < maxFileNr; i++){
        recentFileAction = new QAction(this);
        recentFileAction->setVisible(false);
        QObject::connect(recentFileAction, SIGNAL(triggered()),
                                     this, SLOT(fopenRecent()));
        recentFileActionList.append(recentFileAction);
    }

    for(int i = 0; i < maxFileNr; i++) {
        ui->menu_Open_Recent->addAction(recentFileActionList.at(i));
    }
    updateRecentActionList();
}


void MainWindow::updateRecentActionList(){
    QSettings settings;
    QStringList recentFilePaths = settings.value("recentFiles").toStringList();

    int itEnd = 0;
    if(recentFilePaths.size() <= maxFileNr) {
        itEnd = recentFilePaths.size();
    }
    else {
        itEnd = maxFileNr;
    }

    for (int i = 0; i < itEnd; i++) {
        QString strippedName = QFileInfo(recentFilePaths.at(i)).fileName();
        recentFileActionList.at(i)->setText(strippedName);
        recentFileActionList.at(i)->setData(recentFilePaths.at(i));
        recentFileActionList.at(i)->setVisible(true);
    }

    for (int i = itEnd; i < maxFileNr; i++) {
        recentFileActionList.at(i)->setVisible(false);
    }
}


void MainWindow::adjustForCurrentFile(const QString &filePath){
    currentFilePath = filePath;
    setWindowFilePath(currentFilePath);

    QSettings settings;
    QStringList recentFilePaths =
            settings.value("recentFiles").toStringList();
    recentFilePaths.removeAll(filePath);
    recentFilePaths.prepend(filePath);
    while (recentFilePaths.size() > maxFileNr)
        recentFilePaths.removeLast();
    settings.setValue("recentFiles", recentFilePaths);

    // see note
    updateRecentActionList();
}


void MainWindow::fsave() {
    QString filePath = QFileDialog::getSaveFileName(
                       this, tr("Save File"), "",
                       tr("text files (*.md *.txt)"));
    if (!filePath.isEmpty()) {
        saveFile(filePath);
    }

    return;
}


void MainWindow::saveFile(const QString &filePath) {
    QFile f(filePath);
    f.open( QIODevice::WriteOnly );

    f.write(ui->plainTextEdit->toPlainText().toUtf8());

    f.close();

    return;
}


void MainWindow::eundo() {
    ui->plainTextEdit->undo();

    return;
}

void MainWindow::eredo() {
    ui->plainTextEdit->redo();

    return;
}


void MainWindow::efind() {
    FindDialog *dialog = new FindDialog(this);
    dialog->setModal(false);
    dialog->setTextEdit(ui->plainTextEdit);
    dialog->show();

    return;
}


void MainWindow::efindreplace() {
    FindReplaceDialog *dialog = new FindReplaceDialog(this);
    dialog->setModal(false);
    dialog->setTextEdit(ui->plainTextEdit);
    dialog->show();

    return;
}


void MainWindow::vlf_x_y(int x, int y) {
    ui->horizontalLayout_1->setStretch(x, y);

    return;
}


void MainWindow::vlf_1_1() {
    vlf_x_y(0, 1);
    vlf_x_y(1, 1);

    return;
}


void MainWindow::vlf_2_1() {
    vlf_x_y(0, 2);
    vlf_x_y(1, 1);
    return;
}


void MainWindow::vlf_1_2() {
    vlf_x_y(0, 1);
    vlf_x_y(1, 2);

    return;
}


void MainWindow::about()
{
    QMessageBox::about(this, tr("About WM"),
                       tr("About WM."));
}

//new function interface
//functions in preference to be realised
void MainWindow::wpreferences()
{
    preferences *pre = new preferences(this);
    pre->show();
    return;

}

MainWindow::~MainWindow()
{
    delete ui;
}
