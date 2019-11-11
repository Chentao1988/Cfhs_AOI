#ifndef CFHS_BASE_H
#define CFHS_BASE_H


#include <QString>
#include <QPointer>
#include <QCoreApplication>
#include <QTime>


#pragma execution_character_set("utf-8")

/***********************************
 * 登录账号权限：
 * AdminPermission 管理员权限
 * OperatorPermission 操作员权限：只能执行加载方案，软件执行开始操作
 ************************************/
enum PermissionEnum{AdminPermission=1, OperatorPermission=0};

/**********************************
 * 登录账号 struct
 * name   用户名   QString
 * password 密码   QString
 * Permission  权限  PermissionEnum
 ***********************************/
typedef struct _UserType{
    QString m_name;
    QString m_password;
    PermissionEnum m_Permission;
}UserType;

//界面语言
enum LanguageEnum{English=0, SimplifiedChinese, TraditionalChinese};


#endif // CFHS_BASE_H
