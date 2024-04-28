#ifndef NEWREMINDDIALOG_H
#define NEWREMINDDIALOG_H

#include <QDialog>
#include <QTimer>

#include <remind.h>

namespace Ui {
class NewRemindDialog;
}

class NewRemindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewRemindDialog(QWidget *parent = nullptr);
    ~NewRemindDialog();

    Remind getRemindData();

protected slots:
    void updateDuration();
    void updateDateTime();
    void sync();

private:
    Ui::NewRemindDialog *ui;
    bool inDurationUpdate;
    bool inDateTimeUpdate;
};

#endif // NEWREMINDDIALOG_H
