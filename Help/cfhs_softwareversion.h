#ifndef CFHS_SOFTWAREVERSION_H
#define CFHS_SOFTWAREVERSION_H

#include <QDialog>
#include <QPointer>

class QLabel;
class QPushButton;
class QLineEdit;

class Cfhs_SoftwareVersion : public QDialog
{
    Q_OBJECT

public:
    Cfhs_SoftwareVersion(QWidget *parent = nullptr);
    ~Cfhs_SoftwareVersion();

private:
    void init();
    void setWindowStyle();

private slots:
    void on_commitButton_clicked();
    void on_addLockTimeButton_clicked();
    void on_companyButton_clicked();

private:
    QPointer<QLabel> m_logoLabel;
    QPointer<QLabel> m_versionLabel;
    QPointer<QLineEdit> m_versionLineEdit;
    QPointer<QLabel> m_releaseTimeLabel;
    QPointer<QLabel> m_lockLabel;
    QPointer<QLineEdit> m_lockLineEdit;
    QPointer<QPushButton> m_addLockTimeButton;
    QPointer<QLabel> m_contactManLabel;
    QPointer<QLineEdit> m_contactManLineEdit;
    QPointer<QLabel> m_contactPhoneLabel;
    QPointer<QLineEdit> m_contactPhoneLineEdit;
    QPointer<QLabel> m_companyLabel;
    QPointer<QPushButton> m_companyButton;
    QPointer<QPushButton> m_commitButton;
};

#endif // CFHS_SOFTWAREVERSION_H
