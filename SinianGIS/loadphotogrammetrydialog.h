#ifndef LOADPHOTOGRAMMETRYDIALOG_H
#define LOADPHOTOGRAMMETRYDIALOG_H

#include <QDialog>

namespace Ui {
class LoadPhotogrammetryDialog;
}

class LoadPhotogrammetryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadPhotogrammetryDialog(QWidget *parent = nullptr);
    ~LoadPhotogrammetryDialog();

private:
    Ui::LoadPhotogrammetryDialog *ui;
};

#endif // LOADPHOTOGRAMMETRYDIALOG_H
