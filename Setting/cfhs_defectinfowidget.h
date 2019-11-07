#ifndef CFHS_DEFECTINFOWIDGET_H
#define CFHS_DEFECTINFOWIDGET_H

#include <QWidget>
#include <QPointer>
#include <QMap>
#include "Setting/cfhs_defectinfotable.h"


class QLabel;
class QLineEdit;
class QPushButton;

class Cfhs_DefectInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit Cfhs_DefectInfoWidget(const QStringList &listFeather, QWidget *parent = nullptr);
    ~Cfhs_DefectInfoWidget();
    QMap<QString, QString> getDefectInfoMap();
    void setDefectInfoMap(const QMap<QString, QString> &map);
    void setFeatherList(const QStringList &list);

signals:
    void sig_deleteOneRow(const QString &name);
    void sig_editOneRow(const QString &oldName, const QString &newName);
    void sig_addOneRow(const QString &name);

private slots:
    void onCommitButton_clicked();

private:
    QPointer<QLabel> m_defectTitleLabel;
    QPointer<QLineEdit> m_defectNameLineEdit;
    QPointer<QPushButton> m_commitButton;
    QPointer<Cfhs_DefectInfoTable> m_infoTable;
};

#endif // CFHS_DEFECTINFOWIDGET_H
