#ifndef CFHS_ALGORITHMTABLE_H
#define CFHS_ALGORITHMTABLE_H

#include <QTableWidget>
#include <QMap>

//该类主要用于设置算法参数表
class Cfhs_AlgorithmTable : public QTableWidget
{
public:
    Cfhs_AlgorithmTable(QWidget *parent = nullptr);
    ~Cfhs_AlgorithmTable();
    //设置参数列表
    //listName  参数名列表
    bool setParaList(const QStringList &listName);
    //key  参数名
    //value 参数说明
    bool setParaMap(const QMap<int, QStringList> &map);
    //添加一行
    //paraName  参数名
    bool addOnePara(const QString& paraName);
    //添加一行
    //paraName  参数名
    //paraInfo  参数说明
    bool addOnePara(const QString& paraName, const QString& paraInfo);
    //删除一行
    //paraName  参数名
    bool delOnePara(const QString& paraName);
    //获取参数表保存的值
    //key  参数索引，从1开始
    //value  参数值，固定int
    bool getParaMap(QMap<int, QString> &map);
    //清空表格
    void tableInit();

private:
    QMap<int, QString> m_mapPara;
};

#endif // CFHS_ALGORITHMTABLE_H
