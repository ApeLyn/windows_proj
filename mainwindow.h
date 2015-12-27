#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QUndoStack>
#include <QUndoView>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    //file control
    void fnew();
    void fopen();
    void fopenRecent();
//     void fclose();
    void fsave();
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
private:
    Ui::MainWindow *ui;
    QMenu* fileMenu;
    QMenu* recentFilesMenu;

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

    QUndoStack *undoStack;
    QUndoView *undoView;
};

#endif // MAINWINDOW_H
