#ifndef OSG3DOBJECTSHOWWIDGET_H
#define OSG3DOBJECTSHOWWIDGET_H

#include "osgshowwidget.h"

class OSG3DObjectShowWidget : public OSGShowWidget
{
    Q_OBJECT
public:
    explicit OSG3DObjectShowWidget(QWidget *parent = nullptr);
    virtual ~OSG3DObjectShowWidget();

    void load3DObject(std::string filePath);

signals:

protected:
    osg::ref_ptr<osg::Group> root;

public slots:
};

#endif // OSG3DOBJECTSHOWWIDGET_H
