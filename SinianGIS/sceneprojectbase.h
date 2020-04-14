#ifndef SCENEPROJECTBASE_H
#define SCENEPROJECTBASE_H

#include <string>

class SceneProjectBase
{
public:
    SceneProjectBase();
    virtual ~SceneProjectBase();

    virtual void create()=0;
    virtual void read(std::string path)=0;
    virtual void write(std::string path)=0;

protected:
    std::string projectFilePath;
    std::string appDir;
};

#endif // SCENEPROJECTBASE_H
