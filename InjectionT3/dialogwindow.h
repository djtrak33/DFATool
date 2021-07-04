#ifndef DIALOGWINDOW_H
#define DIALOGWINDOW_H

#include <QWidget>
#include <QMessageBox>
#include <cmath>
#include <QCloseEvent>
#include <QDebug>

namespace Ui {
class DialogWindow;
}

class DialogWindow : public QWidget
{
    Q_OBJECT

signals:
    void windowClosed();

public:
    explicit DialogWindow(QWidget *parent = nullptr, double *GPSLatPA = nullptr, double *GPSLonPA = nullptr, double *GPSAltPA = nullptr);
    ~DialogWindow();

private slots:

    bool check_gps_validity(QString lat, QString lon, QString alt, int isFloatAllowed);

    int gps_which_are_float(bool lon, bool lat, bool alt);

    void on_buttonRandomizeGPSInjection_clicked();

    void on_textEditGPSLatInjection_textChanged();

    void on_textEditGPSLonInjection_textChanged();

    void on_textEditGPSAltInjection_textChanged();

    void on_checkboxGPSLatInjection_clicked();

    void on_checkboxGPSLonInjection_clicked();

    void on_checkboxGPSAltInjection_clicked();

    void textChangedGPSLat(QString text);

    void textChangedGPSLon(QString text);

    void textChangedGPSAlt(QString text);

    void on_buttonGPSInjectionContinue_clicked();

private:
    Ui::DialogWindow *ui;
    double GPSLon = std::numeric_limits<double>::quiet_NaN(), GPSLat = std::numeric_limits<double>::quiet_NaN(), GPSAlt = std::numeric_limits<double>::quiet_NaN();
    double *GPSLonPS, *GPSLatPS, *GPSAltPS;
};

#endif // DIALOGWINDOW_H
