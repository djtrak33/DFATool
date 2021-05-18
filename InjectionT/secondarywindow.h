#ifndef SECONDARYWINDOW_H
#define SECONDARYWINDOW_H

#include <QDialog>
#include <QMessageBox>
#include <cmath>
#include <QCloseEvent>
#include <QDebug>

namespace Ui {
class SecondaryWindow;
}

class SecondaryWindow : public QDialog
{
    Q_OBJECT

signals:
    void windowClosed(double GPSLatW, double GPSLonW, double GPSAltW);

public:
    explicit SecondaryWindow(QWidget *parent = 0);
    ~SecondaryWindow();

private slots:

    bool check_gps_validity(QString lat, QString lon, QString alt, int isFloatAllowed);

    int gps_which_are_float(bool lon, bool lat, bool alt);

    void on_buttonRandomizeGPSInjection_clicked();

    void on_textEditGPSLatInjection_3_textChanged();

    void on_textEditGPSLonInjection_2_textChanged();

    void on_textEditGPSAltInjection_3_textChanged();

    void on_checkboxGPSLatInjection_clicked();

    void on_checkboxGPSLonInjection_clicked();

    void on_checkboxGPSAltInjection_clicked();

    void textChangedGPSLat(QString text);

    void textChangedGPSLon(QString text);

    void textChangedGPSAlt(QString text);

    void on_buttonGPSInjectionContinue_clicked();

private:
    Ui::SecondaryWindow *ui;
    double GPSLon = std::numeric_limits<double>::quiet_NaN(), GPSLat = std::numeric_limits<double>::quiet_NaN(), GPSAlt = std::numeric_limits<double>::quiet_NaN();
};

#endif // SECONDARYWINDOW_H
