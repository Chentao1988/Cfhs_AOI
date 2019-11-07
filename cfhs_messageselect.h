#ifndef CFHS_MESSAGESELECT_H
#define CFHS_MESSAGESELECT_H

#include <QDialog>
#include <QPointer>

class QLabel;
class Cfhs_ComboBox;
class QPushButton;


//自定义弹出框，主要用于从列表中选择数据
class Cfhs_MessageSelect : public QDialog
{
    Q_OBJECT

public:
    //name  项目名称
    //list  项目的取值列表
    //parent  父窗口
    Cfhs_MessageSelect(const QString& name, const QStringList& list, QWidget *parent = nullptr);
    ~Cfhs_MessageSelect();
    QString getMessage() const;
    int getCurrentIndex();
    void setCurrentIndex(const int &index);
    void setCurrentText(const QString &text);

private slots:
    void onCommitButton_clicked();
    void onCancelButton_clicked();

private:
    QPointer<QLabel> m_nameLabel;
    QPointer<Cfhs_ComboBox> m_msgCombo;
    QPointer<QPushButton> m_commitButton;
    QPointer<QPushButton> m_cancelButton;
    QString m_strMessage;  //返回结果
};

#endif // CFHS_MESSAGESELECT_H
