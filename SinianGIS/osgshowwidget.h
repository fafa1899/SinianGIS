#ifndef OSGSHOWWIDGET_H
#define OSGSHOWWIDGET_H

#include <QWidget>
#include <QTimer>

#include <osgViewer/CompositeViewer>

class OSGShowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OSGShowWidget(QWidget *parent = nullptr);

    bool loadData(std::string path);

    //启动定时器绘制
    void onStartTimer();
    //关闭定时器绘制
    void onStopTimer();

protected:
    //virtual void paintEvent(QPaintEvent* e);
    virtual void timerEvent(QTimerEvent* );

    void addView();

    QTimer                     _timer;
    int _timerID;               //定时器ID
    osgViewer::CompositeViewer _viewer;
    osg::ref_ptr<osg::Node>    _scene;

signals:

public slots:
};

#endif // OSGSHOWWIDGET_H
