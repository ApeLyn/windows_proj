#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>

namespace Ui {
class preferences;
}

class preferences : public QDialog
{
    Q_OBJECT

public:
    explicit preferences(QWidget *parent = 0);
    ~preferences();

public slots:
    void switch2gen();
    void switch2edit();
    void switch2theme();
    void switch2css();

private:
    Ui::preferences *ui;
    void set_env(char env_id, bool env);

};

#endif // PREFERENCES_H
