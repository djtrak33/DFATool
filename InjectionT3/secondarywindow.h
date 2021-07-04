#ifndef SECONDARYWINDOW_H
#define SECONDARYWINDOW_H

#include <QDialog>
#include <QMessageBox>
#include <cmath>
#include <QCloseEvent>
#include <QDebug>
#include <QMainWindow>

namespace Ui {
class SecondaryWindow;
}

class SecondaryWindow : public QDialog
{
    Q_OBJECT

signals:
    void windowClosedGPSFixed(double GPSLatW, double GPSLonW, double GPSAltW, double startTimeW, double endTimeW);
    void windowClosedGPSFreeze(double startTimeW, double endTimeW);
    void windowClosedGPSDelay(double delayvalueW, double startTimeW, double endTimeW);
    void windowClosedGPSRandom(double GPSLatWMin, double GPSLatWMax, double GPSLonWMin, double GPSLonWMax, double GPSAltWMin, double GPSAltWMax, double startTimeW, double endTimeW);
    void windowClosedGPSMinMax(double GPSLatW, double GPSLonW, double GPSAltW, double startTimeW, double endTimeW);
    void windowClosedSecurityPosition(double GPSLatW, double GPSLonW, double GPSAltW, double startTimeW, double endTimeW);
    void windowClosedSecurityUAVPosition(double GPSLatW, double GPSLonW, double GPSAltW, double startTimeW, double endTimeW);
    void windowClosedSecurityAttackRandom(double GPSLatWMin, double GPSLatWMax, double GPSLonWMin, double GPSLonWMax, double GPSAltWMin, double GPSAltWMax, double startTimeW, double endTimeW);
    void windowClosedSecurityAttackDelay(double delayvalueW, double startTimeW, double endTimeW);
    void windowClosedSecurityAttackRandomLat(double GPSLatWMin, double GPSLatWMax, double GPSLonWMin, double GPSLonWMax, double GPSAltWMin, double GPSAltWMax, double startTimeW, double endTimeW);
    void windowClosedSecurityAttackRandomLon(double GPSLatWMin, double GPSLatWMax, double GPSLonWMin, double GPSLonWMax, double GPSAltWMin, double GPSAltWMax, double startTimeW, double endTimeW);
    void windowClosedSecurityForceLand(double GPSLatW, double GPSLonW, double GPSAltW, double startTimeW, double endTimeW);

public:
    explicit SecondaryWindow(QWidget *parent = 0);
    ~SecondaryWindow();

private slots:

    bool check_gps_validity(QString lat, QString lon, QString alt);

    void on_textEditGPSLatInjection_5_textChanged();

    void on_textEditGPSLonInjection_5_textChanged();

    void on_textEditGPSAltInjection_5_textChanged();

    void textChangedGPSLat(QString text);

    void textChangedGPSLon(QString text);

    void textChangedGPSAlt(QString text);

    void on_buttonGPSInjectionContinue_clicked();

    void on_doubleSpinBox_startInjection_2_valueChanged(double arg1);

    void on_doubleSpinBox_endInjection_2_valueChanged(double arg1);

    void changeWindowType(QString text);

    void on_doubleSpinBox_startInjection_3_valueChanged(double arg1);

    void on_doubleSpinBox_endInjection_3_valueChanged(double arg1);

    void on_doubleSpinBox_startInjection_4_valueChanged(double arg1);

    void on_doubleSpinBox_endInjection_4_valueChanged(double arg1);

    void on_buttonGPSInjectionContinue_2_clicked();

    void on_buttonGPSInjectionContinue_3_clicked();

    void on_doubleSpinBox_startInjection_5_valueChanged(double arg1);

    void on_doubleSpinBox_endInjection_5_valueChanged(double arg1);

    void on_buttonGPSInjectionContinue_4_clicked();

    void on_textEditGPSLatInjection_3_textChanged();

    void on_textEditGPSLatInjection_6_textChanged();

    void on_textEditGPSLonInjection_7_textChanged();

    void on_textEditGPSLonInjection_6_textChanged();

    void on_textEditGPSAltInjection_3_textChanged();

    void on_textEditGPSAltInjection_6_textChanged();

    void on_checkboxGPSLatInjection_2_clicked();

    void on_doubleSpinBox_startInjection_6_valueChanged(double arg1);

    void on_doubleSpinBox_endInjection_6_valueChanged(double arg1);

    void on_buttonGPSInjectionContinue_5_clicked();

    void on_checkboxGPSLonInjection_2_clicked();

    void on_checkboxGPSAltInjection_2_clicked();

    void on_buttonGPSInjectionCancel_clicked();

    void on_buttonGPSInjectionCancel_2_clicked();

    void on_buttonGPSInjectionCancel_3_clicked();

    void on_buttonGPSInjectionCancel_4_clicked();

    void on_buttonGPSInjectionCancel_5_clicked();

    void on_doubleSpinBox_delayvalue_valueChanged(double arg1);

    void on_buttonGPSInjectionContinue_6_clicked();

    void on_textEditGPSLatInjection_7_textChanged();

    void on_buttonGPSInjectionCancel_6_clicked();

    void on_doubleSpinBox_endInjection_7_valueChanged(double arg1);

    void on_doubleSpinBox_startInjection_7_valueChanged(double arg1);

    void on_textEditGPSLonInjection_8_textChanged();

    void on_textEditGPSAltInjection_7_textChanged();

    void on_doubleSpinBox_startInjection_8_valueChanged(double arg1);

    void on_doubleSpinBox_endInjection_8_valueChanged(double arg1);

    void on_textEditGPSLatInjection_8_textChanged();

    void on_textEditGPSLonInjection_9_textChanged();

    void on_textEditGPSAltInjection_8_textChanged();

    void on_buttonGPSInjectionCancel_7_clicked();

    void on_buttonGPSInjectionContinue_7_clicked();

    void on_doubleSpinBox_startInjection_9_valueChanged(double arg1);

    void on_doubleSpinBox_endInjection_9_valueChanged(double arg1);

    void on_buttonGPSInjectionCancel_8_clicked();

    void on_buttonGPSInjectionContinue_8_clicked();

    void on_textEditGPSLatInjection_4_textChanged();

    void on_textEditGPSLatInjection_9_textChanged();

    void on_textEditGPSLonInjection_11_textChanged();

    void on_textEditGPSLonInjection_10_textChanged();

    void on_textEditGPSAltInjection_4_textChanged();

    void on_textEditGPSAltInjection_9_textChanged();

    void on_doubleSpinBox_startInjection_10_valueChanged(double arg1);

    void on_doubleSpinBox_endInjection_10_valueChanged(double arg1);

    void on_doubleSpinBox_delayvalue_2_valueChanged(double arg1);

    void on_buttonGPSInjectionCancel_9_clicked();

    void on_buttonGPSInjectionContinue_9_clicked();

    void on_doubleSpinBox_startInjection_11_valueChanged(double arg1);

    void on_doubleSpinBox_endInjection_11_valueChanged(double arg1);

    void on_buttonGPSInjectionCancel_10_clicked();

    void on_buttonGPSInjectionContinue_10_clicked();

    void on_textEditGPSLatInjection_10_textChanged();

    void on_textEditGPSLatInjection_11_textChanged();

    void on_doubleSpinBox_startInjection_12_valueChanged(double arg1);

    void on_doubleSpinBox_endInjection_12_valueChanged(double arg1);

    void on_buttonGPSInjectionCancel_11_clicked();

    void on_buttonGPSInjectionContinue_11_clicked();

    void on_textEditGPSLonInjection_12_textChanged();

    void on_textEditGPSLonInjection_13_textChanged();

    void on_doubleSpinBox_startInjection_13_valueChanged(double arg1);

    void on_doubleSpinBox_endInjection_13_valueChanged(double arg1);

    void on_buttonGPSInjectionCancel_12_clicked();

    void on_buttonGPSInjectionContinue_12_clicked();

    void on_textEditGPSAltInjection_10_textChanged();

private:
    Ui::SecondaryWindow *ui;

    QString typeWindow;
    bool buttonValidity = false;

    double startTime = std::numeric_limits<double>::quiet_NaN(), endTime = std::numeric_limits<double>::quiet_NaN();
    double GPSLon = std::numeric_limits<double>::quiet_NaN(), GPSLat = std::numeric_limits<double>::quiet_NaN(), GPSAlt = std::numeric_limits<double>::quiet_NaN();

    double GPSLonMin = std::numeric_limits<double>::quiet_NaN(), GPSLatMin = std::numeric_limits<double>::quiet_NaN(), GPSAltMin = std::numeric_limits<double>::quiet_NaN();
    double GPSLonMax = std::numeric_limits<double>::quiet_NaN(), GPSLatMax = std::numeric_limits<double>::quiet_NaN(), GPSAltMax = std::numeric_limits<double>::quiet_NaN();
    double delayValue = std::numeric_limits<double>::quiet_NaN();
};

#endif // SECONDARYWINDOW_H
