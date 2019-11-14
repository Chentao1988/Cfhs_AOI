#ifndef LINEEDIT_BUTTON_H
#define LINEEDIT_BUTTON_H
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QFileDialog>
class lineedit_button:public QWidget
{
    Q_OBJECT
public:
    enum FileType{FileImage, FileCcf};
    lineedit_button(QString text,QWidget *parent = nullptr,
                    const lineedit_button::FileType &type = FileImage);
    ~lineedit_button();
private:
    QLineEdit * lineedit;
    QLabel *label_text;
    FileType file_type;
public slots:
    void button_click();
    QString Get_Text();
    void Set_Text(QString text);
    QString Get_Path() const;
    void Set_Path(const QString &path);
};

#endif // LINEEDIT_BUTTON_H
