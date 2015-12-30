#ifndef FINDREPLACEFORM_H
#define FINDREPLACEFORM_H

#include <QWidget>
#include <QTextCursor>
#include <QPlainTextEdit>
#include <QSettings>


namespace Ui {
    class FindReplaceForm;
}

class QTextEdit;
class QSettings;

class FindReplaceForm : public QWidget {
    Q_OBJECT
public:
    FindReplaceForm(QWidget *parent = 0);
    virtual ~FindReplaceForm();

    void setTextEdit(QPlainTextEdit *textEdit_);
    void hideReplaceWidgets();
    virtual void writeSettings(QSettings &settings, const QString &prefix = "FindReplaceDialog");
    virtual void readSettings(QSettings &settings, const QString &prefix = "FindReplaceDialog");

public slots:
    void find(bool down);
    void find();
    void findNext() { find(true); }
    void findPrev() { find(false); }
    void replace();
    void replaceAll();

protected:
    void changeEvent(QEvent *e);

protected slots:
    void textToFindChanged();
    void textToReplaceChanged();

protected:
    Ui::FindReplaceForm *ui;
    QTextCursor textCursor;
    QPlainTextEdit *textEdit;
};


#endif // FINDREPLACEFORM_H
