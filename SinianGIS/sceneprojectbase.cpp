#include "sceneprojectbase.h"

#include <iostream>
#include "pathref.hpp"

using namespace std;

SceneProjectBase::SceneProjectBase()
{
    appDir = PathRef::GetAppDir();
}

SceneProjectBase::~SceneProjectBase()
{

}
