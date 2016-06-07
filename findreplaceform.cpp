#include <QtGui>
#include <QTextEdit>
#include <QRegExp>

#include "findreplaceform.h"
#include "ui_findreplaceform.h"


#define TEXT_TO_FIND "textToFind"
#define TEXT_TO_REPLACE "textToReplace"
#define NEXT_RADIO "nextRadio"
#define PRE_RADIO "preRadio"
#define CASE_CHECK "caseCheck"
#define WHOLE_CHECK "wholeCheck"


FindReplaceForm::FindReplaceForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FindReplaceForm), textEdit(0)
{
    ui->setupUi(this);

    connect(ui->textToFind, SIGNAL(textChanged(QString)), this, SLOT(textToFindChanged()));
    connect(ui->textToReplace, SIGNAL(textChanged(QString)), this, SLOT(textToReplaceChanged()));

    connect(ui->findButton, SIGNAL(clicked()), this, SLOT(find()));
    connect(ui->closeButton, SIGNAL(clicked()), parent, SLOT(close()));

    connect(ui->replaceButton, SIGNAL(clicked()), this, SLOT(replace()));
    connect(ui->replaceAllButton, SIGNAL(clicked()), this, SLOT(replaceAll()));
}

FindReplaceForm::~FindReplaceForm()
{
    delete ui;
}

void FindReplaceForm::hideReplaceWidgets() {
    ui->replaceLabel->setVisible(false);
    ui->textToReplace->setVisible(false);
    ui->replaceButton->setVisible(false);
    ui->replaceAllButton->setVisible(false);
}

void FindReplaceForm::setTextEdit(QPlainTextEdit *textEdit_) {
    textEdit = textEdit_;
    // connect(textEdit, SIGNAL(copyAvailable(bool)), ui->replaceButton, SLOT(setEnabled(bool)));
    // connect(textEdit, SIGNAL(copyAvailable(bool)), ui->replaceAllButton, SLOT(setEnabled(bool)));
    if (textEdit->toPlainText().size() > 0) {
        textCursor.setPosition(0);
        textEdit->setTextCursor(textCursor);
    }
}

void FindReplaceForm::changeEvent(QEvent *e)
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

void FindReplaceForm::textToFindChanged() {
    ui->findButton->setEnabled(ui->textToFind->text().size() > 0);
    return;
}


void FindReplaceForm::textToReplaceChanged() {
    ui->replaceButton->setEnabled(ui->textToFind->text().size() > 0 && ui->textToReplace->text().size() > 0);
    ui->replaceAllButton->setEnabled(ui->textToFind->text().size() > 0 && ui->textToReplace->text().size() > 0);
    return;
}


void FindReplaceForm::find() {
    find(ui->nextRadioButton->isChecked());
}


void FindReplaceForm::find(bool next) {
    if (!textEdit)
        return;

    bool back = !next;

    const QString &toSearch = ui->textToFind->text();

    bool result = false;

    QTextDocument::FindFlags flags;

    if (back)
        flags |= QTextDocument::FindBackward;
    if (ui->caseCheckBox->isChecked())
        flags |= QTextDocument::FindCaseSensitively;
    if (ui->wholeCheckBox->isChecked())
        flags |= QTextDocument::FindWholeWords;

    result = textEdit->find(toSearch, flags);

    if (!result) {
        textCursor.setPosition(0);
        textEdit->setTextCursor(textCursor);
    }
}

void FindReplaceForm::replace() {
    if (!textEdit->textCursor().hasSelection()) {
        find();
        textEdit->textCursor().insertText(ui->textToReplace->text());
    } else {
        textEdit->textCursor().insertText(ui->textToReplace->text());
        find();
    }
}

void FindReplaceForm::replaceAll() {
    if(!textEdit->textCursor().hasSelection()) {
        find();
    }
    while (textEdit->textCursor().hasSelection()){
        textEdit->textCursor().insertText(ui->textToReplace->text());
        find();
    }
}

void FindReplaceForm::writeSettings(QSettings &settings, const QString &prefix) {
    settings.beginGroup(prefix);
    settings.setValue(TEXT_TO_FIND, ui->textToFind->text());
    settings.setValue(TEXT_TO_REPLACE, ui->textToReplace->text());
    settings.setValue(NEXT_RADIO, ui->nextRadioButton->isChecked());
    settings.setValue(PRE_RADIO, ui->preRadioButton->isChecked());
    settings.setValue(CASE_CHECK, ui->caseCheckBox->isChecked());
    settings.setValue(WHOLE_CHECK, ui->wholeCheckBox->isChecked());
    settings.endGroup();
}

void FindReplaceForm::readSettings(QSettings &settings, const QString &prefix) {
    settings.beginGroup(prefix);
    ui->textToFind->setText(settings.value(TEXT_TO_FIND, "").toString());
    ui->textToReplace->setText(settings.value(TEXT_TO_REPLACE, "").toString());
    ui->nextRadioButton->setChecked(settings.value(NEXT_RADIO, true).toBool());
    ui->preRadioButton->setChecked(settings.value(PRE_RADIO, false).toBool());
    ui->caseCheckBox->setChecked(settings.value(CASE_CHECK, false).toBool());
    ui->wholeCheckBox->setChecked(settings.value(WHOLE_CHECK, false).toBool());
    settings.endGroup();
}
