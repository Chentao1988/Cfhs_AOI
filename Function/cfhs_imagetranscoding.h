#ifndef CFHS_IMAGETRANSCODING_H
#define CFHS_IMAGETRANSCODING_H

#include <QDialog>
#include <QMap>


class lineedit_button;
class QPushButton;

class Cfhs_ImageTranscoding : public QDialog
{
    Q_OBJECT

public:
    Cfhs_ImageTranscoding(QWidget *parent = nullptr, const int &stationNum = 4);
    ~Cfhs_ImageTranscoding();

    static bool isStaticImgExists(const int &stationNum = 4);

private:
    //图片转码
    //station_name 保存的工位名
    //image_path  需转码的图片路径
    //strInfo 执行日志
    bool setImageTranscode(const QString &station_name, const QString &image_path, QString &strInfo);
    bool isImagePathValid();
    std::string getStringFromQString(const QString &info);

private slots:
    void button_commit_clicked();
    void button_cancel_clicked();

private:
    lineedit_button *result_img;
    lineedit_button *analysis_img;
    //key 工位号
    //value 工位img
    QMap<int, lineedit_button*> map_station;
    QPushButton *button_commit;
    QPushButton *button_cancel;
};

#endif // CFHS_IMAGETRANSCODING_H
