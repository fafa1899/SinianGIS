#ifndef MAPDOWNLOADDIALOG_H
#define MAPDOWNLOADDIALOG_H

#include <QDialog>

namespace Ui {
class MapDownloadDialog;
}

class MapDownloadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MapDownloadDialog(QWidget *parent = nullptr);
    ~MapDownloadDialog();

protected:
    void SaveIniConfig();

private slots:
    void on_pBOK_clicked();

    void on_pBCancel_clicked();

    void on_pBOutDir_clicked();

private:
    Ui::MapDownloadDialog *ui;
};

#endif // MAPDOWNLOADDIALOG_H
