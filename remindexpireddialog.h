#ifndef REMINDEXPIREDDIALOG_H
#define REMINDEXPIREDDIALOG_H

#include <QDialog>
#include <remind.h>

namespace Ui {
class RemindExpiredDialog;
}

class RemindExpiredDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemindExpiredDialog(QWidget *parent = nullptr, Remind r = Remind());
    ~RemindExpiredDialog();

private:
    Ui::RemindExpiredDialog *ui;
};

#endif // REMINDEXPIREDDIALOG_H
