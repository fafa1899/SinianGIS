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

    std::string GetDataDir(){return dataDir;}

protected:
    std::string dataDir;

private slots:
    void on_pushButtonOK_clicked();

    void on_pushButtonCancel_clicked();

    void on_pBDir_clicked();

private:
    Ui::LoadPhotogrammetryDialog *ui;
};

#endif // LOADPHOTOGRAMMETRYDIALOG_H
