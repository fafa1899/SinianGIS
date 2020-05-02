#ifndef QDOCKWIDGETEX_H
#define QDOCKWIDGETEX_H

#include <QDockWidget>

class QDockWidgetEx : public QDockWidget
{
    Q_OBJECT
public:
    explicit QDockWidgetEx(const QString &title, QWidget *parent = nullptr,
                           Qt::WindowFlags flags = Qt::WindowFlags())
        : QDockWidget(title, parent, flags)
    {
    }

    explicit QDockWidgetEx(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags())
        : QDockWidget(parent, flags)
    {
    }

signals:
    void signalClose(QDockWidgetEx *dock);

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // QDOCKWIDGETEX_H
