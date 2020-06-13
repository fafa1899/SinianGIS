#ifndef REGEDIT_H
#define REGEDIT_H

#include <QString>

/*******************************************************************
* 注册文件关联
*
*  className    // 自定义的类别
*  appPath      // 关联的程序目录
*  ext          // 关联的文件类型
*  extDes       // 该文件类型描述
*
*******************************************************************/
void registerFileRelation(const QString& className,const QString& appPath,const QString& ext,const QString& extDes);

#endif // REGEDIT_H
