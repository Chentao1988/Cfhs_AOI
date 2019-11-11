#ifndef CFHS_FILTERNGDIALOG_H
#define CFHS_FILTERNGDIALOG_H

#include <QDialog>
#include <QPointer>


class Cfhs_DefectInfoTable;
class QPushButton;

class Cfhs_FilterNgDialog : public QDialog
{
    Q_OBJECT

public:
    Cfhs_FilterNgDialog(const QStringList &featureList, QWidget *parent = nullptr);
    ~Cfhs_FilterNgDialog();
    bool setNgFilterInfo(const QString &info);
    QString getNgFilterInfo() const;

private slots:
    void on_commitButton_clicked();
    void on_cancelButton_clicked();

private:
    QPointer<Cfhs_DefectInfoTable> m_ngTable;
    QPointer<QPushButton> m_commitButton;
    QPointer<QPushButton> m_cancelButton;
    QString m_strNgInfo;
};

#endif // CFHS_FILTERNGDIALOG_H
