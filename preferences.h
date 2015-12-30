#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>
#include <QSettings>

#define THEME "THEME"

namespace Ui {
class preferences;
}

class preferences : public QDialog
{
    Q_OBJECT

public:
    explicit preferences(QWidget *parent = 0);
    virtual void writeSettings(QSettings &settings, const QString &prefix = "Preference");
    virtual void readSettings(QSettings &settings, const QString &prefix = "Preference");
    ~preferences();

// protected:
    // void changeEvent(QEvent *e);

public slots:
    void switch2gen();
    void switch2edit();
    void switch2theme();
    void switch2css();
    void change_theme();

private:
    Ui::preferences *ui;
    void set_env(char env_id, bool env);

};

#endif // PREFERENCES_H
