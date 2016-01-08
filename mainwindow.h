#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QUndoStack>
#include <QUndoView>
#include <QSettings>
#include "highlighter.h"
#include "finddialog.h"
#include "findreplacedialog.h"
#include "preferences.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void change_theme(int theme);
    void change_css(int theme);
    ~MainWindow();


protected:
    bool eventFilter (QObject *obj, QEvent *e ) ;
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
    void writeSettings();
    void readSettings();
    QString getHtml(const QString& str);

public slots:
    //file control
    void fnew();
    void fopen();
    void fopenRecent();
//     void fclose();
    void fsave();
    void fexport();
//     void frename();
//
//     //Edit control
    void eundo();
    void eredo();
//     void edelete();
    void efind();
    void efindreplace();
//
//     //view control
    void vlf_x_y(int x, int y);
    void vlf_1_1();
    void vlf_2_1();
    void vlf_1_2();
//
//     //help control
//     void help();
    void about();

    //WinMarker control
    void wpreferences();
  //  void wquit();

    //action
    void Current_Date();
    void Current_Time();
    void insertLeft();
    void insertRight();
    void insertSpace();
    void insertAnd();
    void insertLink();
    void insertImage();
    void insertLine();
    void insertComment();
    void insertCode();
    void help();

private:
    Ui::MainWindow *ui;
    QMenu* fileMenu;
    QMenu* recentFilesMenu;

    FindDialog *m_findDialog;
    FindReplaceDialog *m_findReplaceDialog;
    preferences *m_preferences;

    QAction* openAction;
    QList<QAction*> recentFileActionList;
    const int maxFileNr;

    QString currentFilePath;
    QLabel *imageLabel;

    void createActionsAndConnections();
    void createMenus();

    void loadFile(const QString& filePath);
    void adjustForCurrentFile(const QString& filePath);
    void updateRecentActionList();
    void saveFile(const QString& filePath);
    void savePdf(const QString& filePath);
    QString get_file_content();
    QUndoStack *undoStack;
    QUndoView *undoView;

    Highlighter *highlighter;

    //add preference
};

#endif // MAINWINDOW_H
