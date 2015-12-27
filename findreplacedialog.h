#ifndef FINDREPLACEDIALOG_H
#define FINDREPLACEDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>


namespace Ui {
    class FindReplaceDialog;
}

class QTextEdit;
class QSettings;


class FindReplaceDialog : public QDialog {
    Q_OBJECT
public:
    FindReplaceDialog(QWidget *parent = 0);
    virtual ~FindReplaceDialog();

    void setTextEdit(QPlainTextEdit *textEdit);

    virtual void writeSettings(QSettings &settings, const QString &prefix = "FindReplaceDialog");
    virtual void readSettings(QSettings &settings, const QString &prefix = "FindReplaceDialog");

public slots:
    void findNext();
    void findPrev();

protected:
    void changeEvent(QEvent *e);

    Ui::FindReplaceDialog *ui;
};


#endif // FINDREPLACEDIALOG_H
