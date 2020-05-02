#include "qdockwidgetex.h"
#include <QDebug>

void QDockWidgetEx::closeEvent(QCloseEvent *event)
{
    signalClose(this);
}

