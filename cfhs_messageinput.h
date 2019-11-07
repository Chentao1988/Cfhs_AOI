#ifndef CFHS_MESSAGEINPUT_H
#define CFHS_MESSAGEINPUT_H

#include <QDialog>
#include <QPointer>

class QPushButton;
class QLabel;
class QLineEdit;


//自定义弹出窗口，主要用于输入数据
class Cfhs_MessageInput : public QDialog
{
    Q_OBJECT
public:
    Cfhs_MessageInput(const QString& name,
                      QWidget *parent = nullptr,
                      const QStringList &listAlready = QStringList());
    ~Cfhs_MessageInput();
    QString getMessage() const;

private slots:
    void onCommitButton_clicked();
    void onCancelButton_clicked();

private:
    QString m_strMessage;
    QPointer<QLabel> m_nameLabel;
    QPointer<QLineEdit> m_msgLineEdit;
    QPointer<QPushButton> m_commitButton;
    QPointer<QPushButton> m_cancelButton;
    QStringList m_listAlready; //已有的数据
};

#endif // CFHS_MESSAGEINPUT_H
