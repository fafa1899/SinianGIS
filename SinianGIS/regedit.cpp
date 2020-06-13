#include "regedit.h"

#include <QSettings>

void registerFileRelation(const QString& className,const QString& appPath,const QString& ext,const QString& extDes)
{
    QString baseUrl("HKEY_CURRENT_USER\\Software\\Classes");    // 要添加的顶层目录
    QSettings settingClasses(baseUrl,QSettings::NativeFormat);

    // 在...Classes\类别下创建一个新的类别，并设置该类别打开文件时的调用参数
    settingClasses.setValue("/" + className + "/Shell/Open/Command/.","\"" + appPath + "\" \"%1\"");

    // 文件类型描述
    settingClasses.setValue("/" + className + "/.",extDes);

    // 设置该类别的默认图标默认图标
    settingClasses.setValue("/" + className + "/DefaultIcon/.",appPath + ",0");

    // 关联ext 和 类别
    settingClasses.setValue("/" + ext + "/OpenWithProgIds/" + className,"");

    // 立即保存该修改
    settingClasses.sync();
}
