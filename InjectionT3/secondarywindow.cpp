#include "secondarywindow.h"
#include "ui_secondarywindow.h"

SecondaryWindow::SecondaryWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondaryWindow)
{
    ui->setupUi(this);

    connect(ui->doubleSpinBox_startInjection_2, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_startInjection_2_valueChanged(double)));
    connect(ui->doubleSpinBox_endInjection_2, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_endInjection_2_valueChanged(double)));
    connect(ui->doubleSpinBox_startInjection_3, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_startInjection_3_valueChanged(double)));
    connect(ui->doubleSpinBox_endInjection_3, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_endInjection_3_valueChanged(double)));
    connect(ui->doubleSpinBox_startInjection_4, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_startInjection_3_valueChanged(double)));
    connect(ui->doubleSpinBox_endInjection_4, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_endInjection_3_valueChanged(double)));
    connect(ui->doubleSpinBox_startInjection_5, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_startInjection_5_valueChanged(double)));
    connect(ui->doubleSpinBox_endInjection_5, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_endInjection_5_valueChanged(double)));
    connect(ui->doubleSpinBox_startInjection_6, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_startInjection_6_valueChanged(double)));
    connect(ui->doubleSpinBox_endInjection_6, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_endInjection_6_valueChanged(double)));
    connect(ui->doubleSpinBox_startInjection_7, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_startInjection_7_valueChanged(double)));
    connect(ui->doubleSpinBox_endInjection_7, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_endInjection_7_valueChanged(double)));
    connect(ui->doubleSpinBox_startInjection_8, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_startInjection_8_valueChanged(double)));
    connect(ui->doubleSpinBox_endInjection_8, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_endInjection_8_valueChanged(double)));
    connect(ui->doubleSpinBox_startInjection_9, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_startInjection_9_valueChanged(double)));
    connect(ui->doubleSpinBox_endInjection_9, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_endInjection_9_valueChanged(double)));
    connect(ui->doubleSpinBox_startInjection_10, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_startInjection_10_valueChanged(double)));
    connect(ui->doubleSpinBox_endInjection_10, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_endInjection_10_valueChanged(double)));
    connect(ui->doubleSpinBox_startInjection_11, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_startInjection_11_valueChanged(double)));
    connect(ui->doubleSpinBox_endInjection_11, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_endInjection_11_valueChanged(double)));
    connect(ui->doubleSpinBox_startInjection_12, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_startInjection_12_valueChanged(double)));
    connect(ui->doubleSpinBox_endInjection_12, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_endInjection_12_valueChanged(double)));
    connect(ui->doubleSpinBox_startInjection_13, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_startInjection_13_valueChanged(double)));
    connect(ui->doubleSpinBox_endInjection_13, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_endInjection_13_valueChanged(double)));
    connect(ui->doubleSpinBox_delayvalue, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_delayvalue_valueChanged(double)));
    connect(ui->doubleSpinBox_delayvalue_2, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_delayvalue_2_valueChanged(double)));
}

SecondaryWindow::~SecondaryWindow()
{
    delete ui;
}

//Auxiliary function to check if 3 Strings correspondent to 3 valid coordinate values
bool SecondaryWindow::check_gps_validity(QString lat, QString lon, QString alt){
    double lat_converted;
    double lon_converted;
    double alt_converted;

    lat_converted = lat.toDouble();
    lon_converted = lon.toDouble();
    alt_converted = alt.toDouble();

    int max_options = 0, correct_options = 0;

    if(!lat.isEmpty()){
        max_options++;
        if(lat_converted != 0) correct_options ++;
    }
    if(!lon.isEmpty()){
        max_options++;
        if(lon_converted != 0) correct_options ++;
    }
    if(!alt.isEmpty()){
        max_options++;
        if(alt_converted != 0) correct_options ++;
    }
    if (max_options == correct_options and max_options != 0){
        return true;
    }else return false;
}

//Process data from GPS Fixed Value - Latitude
void SecondaryWindow::on_textEditGPSLatInjection_5_textChanged()
{
    bool canShowButton;

    canShowButton = check_gps_validity(ui->textEditGPSLatInjection_5->toPlainText(),
                       ui->textEditGPSLonInjection_5->toPlainText(),
                       ui->textEditGPSAltInjection_5->toPlainText());

    if(canShowButton) {
        ui->buttonGPSInjectionContinue->setVisible(true);
        GPSLat = ui->textEditGPSLatInjection_5->toPlainText().toDouble();
    }
    else{
        ui->buttonGPSInjectionContinue->setVisible(false);
        GPSLat = std::numeric_limits<double>::quiet_NaN();
    }
}

//Process data from GPS Fixed Value - Longitude
void SecondaryWindow::on_textEditGPSLonInjection_5_textChanged()
{
    bool canShowButton;

    canShowButton = check_gps_validity(ui->textEditGPSLatInjection_5->toPlainText(),
                       ui->textEditGPSLonInjection_5->toPlainText(),
                       ui->textEditGPSAltInjection_5->toPlainText());
    if(canShowButton){
        ui->buttonGPSInjectionContinue->setVisible(true);
        GPSLon = ui->textEditGPSLonInjection_5->toPlainText().toDouble();
    }
    else{
        ui->buttonGPSInjectionContinue->setVisible(false);
        GPSLon = std::numeric_limits<double>::quiet_NaN();
    }
}

//Process data from GPS Fixed Value - Altitude
void SecondaryWindow::on_textEditGPSAltInjection_5_textChanged()
{
    bool canShowButton;

    canShowButton = check_gps_validity(ui->textEditGPSLatInjection_5->toPlainText(),
                       ui->textEditGPSLonInjection_5->toPlainText(),
                       ui->textEditGPSAltInjection_5->toPlainText());
    if(canShowButton){
        ui->buttonGPSInjectionContinue->setVisible(true);
        GPSAlt = ui->textEditGPSAltInjection_5->toPlainText().toDouble();
    }
    else{
        ui->buttonGPSInjectionContinue->setVisible(false);
        GPSAlt = std::numeric_limits<double>::quiet_NaN();
    }
}

//Currently Unused
void SecondaryWindow::textChangedGPSLat(QString text)
{
    bool canShowButton;

    canShowButton = check_gps_validity(ui->textEditGPSLatInjection_5->toPlainText(),
                       ui->textEditGPSLonInjection_5->toPlainText(),
                       ui->textEditGPSAltInjection_5->toPlainText());
    if(canShowButton) ui->buttonGPSInjectionContinue->setVisible(true);
    else ui->buttonGPSInjectionContinue->setVisible(false);
}

//Currently Unused
void SecondaryWindow::textChangedGPSLon(QString text)
{
    bool canShowButton;

    canShowButton = check_gps_validity(ui->textEditGPSLatInjection_5->toPlainText(),
                       ui->textEditGPSLonInjection_5->toPlainText(),
                       ui->textEditGPSAltInjection_5->toPlainText());
    if(canShowButton) ui->buttonGPSInjectionContinue->setVisible(true);
    else ui->buttonGPSInjectionContinue->setVisible(false);
}

//Currently Unused
void SecondaryWindow::textChangedGPSAlt(QString text)
{
    bool canShowButton;

    canShowButton = check_gps_validity(ui->textEditGPSLatInjection_5->toPlainText(),
                       ui->textEditGPSLonInjection_5->toPlainText(),
                       ui->textEditGPSAltInjection_5->toPlainText());
    if(canShowButton) ui->buttonGPSInjectionContinue->setVisible(true);
    else ui->buttonGPSInjectionContinue->setVisible(false);
}

//Continue button from GPS Fixed Value. Validate data and send signal
void SecondaryWindow::on_buttonGPSInjectionContinue_clicked()
{
    if(((std::isnan(GPSLat)) && (std::isnan(GPSLon)) && (std::isnan(GPSAlt))) || ((GPSLat > 90) || (GPSLat < -90) || (GPSLon > 180) || (GPSLon < -180))){
        QMessageBox::warning(this, "Error", "No valid values given");
    }else if((std::isnan(startTime) || std::isnan(endTime) || startTime > endTime)){
        QMessageBox::warning(this, "Error", "No valid time interval given");
    }
    else{
        emit(windowClosedGPSFixed(GPSLat, GPSLon, GPSAlt, startTime, endTime));
        ui->doubleSpinBox_startInjection_2->setValue(0);
        ui->doubleSpinBox_endInjection_2->setValue(0);
        startTime = std::numeric_limits<double>::quiet_NaN();
        endTime = std::numeric_limits<double>::quiet_NaN();
        ui->textEditGPSLatInjection_5->clear();
        ui->textEditGPSLonInjection_5->clear();
        ui->textEditGPSAltInjection_5->clear();
        GPSLat = std::numeric_limits<double>::quiet_NaN();
        GPSLon = std::numeric_limits<double>::quiet_NaN();
        GPSAlt = std::numeric_limits<double>::quiet_NaN();
        this->close();
    }
}

//GPS Fixed Value Process Start Injection Time Value
void SecondaryWindow::on_doubleSpinBox_startInjection_2_valueChanged(double number)
{
    startTime = number;
}

//GPS Fixed Value Process End Injection Time Value
void SecondaryWindow::on_doubleSpinBox_endInjection_2_valueChanged(double number)
{
    endTime = number;
}

//Auxiliary function to change window based on signal sent from main window
void SecondaryWindow::changeWindowType(QString text){
    typeWindow = text;
    if(typeWindow == "GPS Fixed Value") ui->stackedWidget->setCurrentIndex(1);
    else if(typeWindow == "GPS Freeze Value") ui->stackedWidget->setCurrentIndex(0);
    else if(typeWindow == "GPS Delay Value") ui->stackedWidget->setCurrentIndex(2);
    else if(typeWindow == "GPS Random Value") ui->stackedWidget->setCurrentIndex(3);
    else if(typeWindow == "GPS Min|Max Value") ui->stackedWidget->setCurrentIndex(4);
    else if(typeWindow == "Random Longitude") ui->stackedWidget->setCurrentIndex(5);
    else if(typeWindow == "Random Latitude") ui->stackedWidget->setCurrentIndex(6);
    else if(typeWindow == "Random Position") ui->stackedWidget->setCurrentIndex(7);
    else if(typeWindow == "GPS Delay") ui->stackedWidget->setCurrentIndex(8);
    else if(typeWindow == "Force UAV Landing") ui->stackedWidget->setCurrentIndex(9);
    else if(typeWindow == "Hijack with a second UAV") ui->stackedWidget->setCurrentIndex(10);
    else if(typeWindow == "Hijack with attacker's position") ui->stackedWidget->setCurrentIndex(11);
}

//GPS Freeze Value Process Start Injection Time Value
void SecondaryWindow::on_doubleSpinBox_startInjection_3_valueChanged(double number)
{
    startTime = number;
}

//GPS Freeze Value Process End Injection Time Value
void SecondaryWindow::on_doubleSpinBox_endInjection_3_valueChanged(double number)
{
    endTime = number;
}

//GPS Delay Value Process Start Injection Time Value
void SecondaryWindow::on_doubleSpinBox_startInjection_4_valueChanged(double number)
{
    startTime = number;
}

//GPS Delay Value Process End Injection Time Value
void SecondaryWindow::on_doubleSpinBox_endInjection_4_valueChanged(double number)
{
    endTime = number;
}

//Continue button from GPS Freeze Value. Validate data and send signal
void SecondaryWindow::on_buttonGPSInjectionContinue_2_clicked()
{
    if(std::isnan(startTime) || std::isnan(endTime) || startTime > endTime){
        QMessageBox::warning(this, "Error", "No valid time interval given");
    }else{
        emit(windowClosedGPSFreeze(startTime, endTime));
        ui->doubleSpinBox_startInjection_3->setValue(0);
        ui->doubleSpinBox_endInjection_3->setValue(0);
        startTime = std::numeric_limits<double>::quiet_NaN();
        endTime = std::numeric_limits<double>::quiet_NaN();
        this->close();
    }
}

//Continue button from GPS Delay Value. Validate data and send signal
void SecondaryWindow::on_buttonGPSInjectionContinue_3_clicked()
{
    if(std::isnan(startTime) || std::isnan(endTime) || startTime > endTime){
        QMessageBox::warning(this, "Error", "No valid time interval given");
    }else if(std::isnan(delayValue)){
        QMessageBox::warning(this, "Error", "Delay invalid");
    }else{
        emit(windowClosedGPSDelay(delayValue, startTime, endTime));
        ui->doubleSpinBox_startInjection_4->setValue(0);
        ui->doubleSpinBox_endInjection_4->setValue(0);
        ui->doubleSpinBox_delayvalue->setValue(0);
        delayValue = std::numeric_limits<double>::quiet_NaN();
        startTime = std::numeric_limits<double>::quiet_NaN();
        endTime = std::numeric_limits<double>::quiet_NaN();
        this->close();
    }
}

//GPS Random Value Process Start Injection Time Value
void SecondaryWindow::on_doubleSpinBox_startInjection_5_valueChanged(double number)
{
    startTime = number;
}

//GPS Random Value Process End Injection Time Value
void SecondaryWindow::on_doubleSpinBox_endInjection_5_valueChanged(double number)
{
    endTime = number;
}

//Continue button from GPS Random Value. Validate data and send signal
void SecondaryWindow::on_buttonGPSInjectionContinue_4_clicked()
{
    if(std::isnan(startTime) || std::isnan(endTime) || startTime > endTime){
        QMessageBox::warning(this, "Error", "No valid time interval given");
    }else if(!buttonValidity){
        QMessageBox::warning(this, "Error", "GPS values given invalid");
    }else{
        emit(windowClosedGPSRandom(GPSLatMin, GPSLatMax, GPSLonMin, GPSLonMax, GPSAltMin, GPSAltMax, startTime, endTime));
        ui->textEditGPSLatInjection_3->clear();
        ui->textEditGPSLatInjection_6->clear();
        ui->textEditGPSLonInjection_7->clear();
        ui->textEditGPSLonInjection_6->clear();
        ui->textEditGPSAltInjection_3->clear();
        ui->textEditGPSAltInjection_6->clear();
        GPSLatMin = std::numeric_limits<double>::quiet_NaN();
        GPSLatMax = std::numeric_limits<double>::quiet_NaN();
        GPSLonMin = std::numeric_limits<double>::quiet_NaN();
        GPSLonMax = std::numeric_limits<double>::quiet_NaN();
        GPSAltMin = std::numeric_limits<double>::quiet_NaN();
        GPSAltMax = std::numeric_limits<double>::quiet_NaN();
        ui->doubleSpinBox_startInjection_5->setValue(0);
        ui->doubleSpinBox_endInjection_5->setValue(0);
        startTime = std::numeric_limits<double>::quiet_NaN();
        endTime = std::numeric_limits<double>::quiet_NaN();
        this->close();
    }
}

//Process data from GPS Random Value - Latitude Minimum
void SecondaryWindow::on_textEditGPSLatInjection_3_textChanged()
{
    bool gpsValidityMin, gpsValidityMax;

    gpsValidityMin = check_gps_validity(ui->textEditGPSLatInjection_3->toPlainText(),
                       ui->textEditGPSLonInjection_7->toPlainText(),
                       ui->textEditGPSAltInjection_3->toPlainText());

    gpsValidityMax = check_gps_validity(ui->textEditGPSLatInjection_6->toPlainText(),
                                        ui->textEditGPSLonInjection_6->toPlainText(),
                                        ui->textEditGPSAltInjection_6->toPlainText());

    if(gpsValidityMin && gpsValidityMax) buttonValidity = true; else buttonValidity = false;
    if(gpsValidityMin) GPSLatMin = ui->textEditGPSLatInjection_3->toPlainText().toDouble(); else GPSLatMin = std::numeric_limits<double>::quiet_NaN();
}

//Process data from GPS Random Value - Latitude Maximum
void SecondaryWindow::on_textEditGPSLatInjection_6_textChanged()
{
    bool gpsValidityMin, gpsValidityMax;

    gpsValidityMin = check_gps_validity(ui->textEditGPSLatInjection_3->toPlainText(),
                       ui->textEditGPSLonInjection_7->toPlainText(),
                       ui->textEditGPSAltInjection_3->toPlainText());

    gpsValidityMax = check_gps_validity(ui->textEditGPSLatInjection_6->toPlainText(),
                                        ui->textEditGPSLonInjection_6->toPlainText(),
                                        ui->textEditGPSAltInjection_6->toPlainText());

    if(gpsValidityMin && gpsValidityMax) buttonValidity = true; else buttonValidity = false;
    if(gpsValidityMax) GPSLatMax = ui->textEditGPSLatInjection_6->toPlainText().toDouble(); else GPSLatMax = std::numeric_limits<double>::quiet_NaN();
}

//Process data from GPS Random Value - Longitude Minimum
void SecondaryWindow::on_textEditGPSLonInjection_7_textChanged()
{
    bool gpsValidityMin, gpsValidityMax;

    gpsValidityMin = check_gps_validity(ui->textEditGPSLatInjection_3->toPlainText(),
                       ui->textEditGPSLonInjection_7->toPlainText(),
                       ui->textEditGPSAltInjection_3->toPlainText());

    gpsValidityMax = check_gps_validity(ui->textEditGPSLatInjection_6->toPlainText(),
                                        ui->textEditGPSLonInjection_6->toPlainText(),
                                        ui->textEditGPSAltInjection_6->toPlainText());

    if(gpsValidityMin && gpsValidityMax) buttonValidity = true; else buttonValidity = false;
    if(gpsValidityMin) GPSLonMin = ui->textEditGPSLonInjection_7->toPlainText().toDouble(); else GPSLonMin = std::numeric_limits<double>::quiet_NaN();
}

//Process data from GPS Random Value - Longitude Maximum
void SecondaryWindow::on_textEditGPSLonInjection_6_textChanged()
{
    bool gpsValidityMin, gpsValidityMax;

    gpsValidityMin = check_gps_validity(ui->textEditGPSLatInjection_3->toPlainText(),
                       ui->textEditGPSLonInjection_7->toPlainText(),
                       ui->textEditGPSAltInjection_3->toPlainText());

    gpsValidityMax = check_gps_validity(ui->textEditGPSLatInjection_6->toPlainText(),
                                        ui->textEditGPSLonInjection_6->toPlainText(),
                                        ui->textEditGPSAltInjection_6->toPlainText());

    if(gpsValidityMin && gpsValidityMax) buttonValidity = true; else buttonValidity = false;
    if(gpsValidityMax) GPSLonMax = ui->textEditGPSLonInjection_6->toPlainText().toDouble(); else GPSLonMax = std::numeric_limits<double>::quiet_NaN();
}

//Process data from GPS Random Value - Altitude Minimum
void SecondaryWindow::on_textEditGPSAltInjection_3_textChanged()
{
    bool gpsValidityMin, gpsValidityMax;

    gpsValidityMin = check_gps_validity(ui->textEditGPSLatInjection_3->toPlainText(),
                       ui->textEditGPSLonInjection_7->toPlainText(),
                       ui->textEditGPSAltInjection_3->toPlainText());

    gpsValidityMax = check_gps_validity(ui->textEditGPSLatInjection_6->toPlainText(),
                                        ui->textEditGPSLonInjection_6->toPlainText(),
                                        ui->textEditGPSAltInjection_6->toPlainText());

    if(gpsValidityMin && gpsValidityMax) buttonValidity = true; else buttonValidity = false;
    if(gpsValidityMin) GPSAltMin = ui->textEditGPSAltInjection_3->toPlainText().toDouble(); else GPSAltMin = std::numeric_limits<double>::quiet_NaN();
}

//Process data from GPS Random Value - Altitude Maximum
void SecondaryWindow::on_textEditGPSAltInjection_6_textChanged()
{
    bool gpsValidityMin, gpsValidityMax;

    gpsValidityMin = check_gps_validity(ui->textEditGPSLatInjection_3->toPlainText(),
                       ui->textEditGPSLonInjection_7->toPlainText(),
                       ui->textEditGPSAltInjection_3->toPlainText());

    gpsValidityMax = check_gps_validity(ui->textEditGPSLatInjection_6->toPlainText(),
                                        ui->textEditGPSLonInjection_6->toPlainText(),
                                        ui->textEditGPSAltInjection_6->toPlainText());

    if(gpsValidityMin && gpsValidityMax) buttonValidity = true; else buttonValidity = false;
    if(gpsValidityMax) GPSAltMax = ui->textEditGPSAltInjection_6->toPlainText().toDouble(); else GPSAltMax = std::numeric_limits<double>::quiet_NaN();
}

//Process data from GPS Min|Max Value - Latitude Checkbox
void SecondaryWindow::on_checkboxGPSLatInjection_2_clicked()
{
    if(ui->checkboxGPSLatInjection_2->isChecked()){
        GPSLat = 1;
    }else GPSLat = 0;
    if(ui->checkboxGPSLatInjection_2->isChecked() || ui->checkboxGPSLonInjection_2->isChecked() || ui->checkboxGPSAltInjection_2->isChecked()){
        buttonValidity = true;
    }else buttonValidity = false;
}

//GPS Min|Max Value Process Start Injection Time Value
void SecondaryWindow::on_doubleSpinBox_startInjection_6_valueChanged(double number)
{
    startTime = number;
}

//GPS Min|Max Value Process End Injection Time Value
void SecondaryWindow::on_doubleSpinBox_endInjection_6_valueChanged(double number)
{
    endTime = number;
}

//Continue button from GPS Min|Max Value. Validate data and send signal
void SecondaryWindow::on_buttonGPSInjectionContinue_5_clicked()
{
    if(buttonValidity){
        QMessageBox::warning(this, "Error", "No value chosen");
    }else if(std::isnan(startTime) || std::isnan(endTime) || startTime > endTime) QMessageBox::warning(this, "Error", "No valid time interval given");
    else{
        emit(windowClosedGPSMinMax(GPSLat, GPSLon, GPSAlt, startTime, endTime));
        ui->doubleSpinBox_startInjection_6->setValue(0);
        ui->doubleSpinBox_endInjection_6->setValue(0);
        startTime = std::numeric_limits<double>::quiet_NaN();
        endTime = std::numeric_limits<double>::quiet_NaN();
        ui->checkboxGPSLatInjection_2->setChecked(false);
        ui->checkboxGPSLonInjection_2->setChecked(false);
        ui->checkboxGPSAltInjection_2->setChecked(false);
        GPSLat = std::numeric_limits<double>::quiet_NaN();
        GPSLon = std::numeric_limits<double>::quiet_NaN();
        GPSAlt = std::numeric_limits<double>::quiet_NaN();
        this->close();
    }
}

//Process data from GPS Min|Max Value - Longitude Checkbox
void SecondaryWindow::on_checkboxGPSLonInjection_2_clicked()
{
    if(ui->checkboxGPSLonInjection_2->isChecked()){
        GPSLon = 1;
    }else GPSLon = 0;
    if(ui->checkboxGPSLatInjection_2->isChecked() || ui->checkboxGPSLonInjection_2->isChecked() || ui->checkboxGPSAltInjection_2->isChecked()){
        buttonValidity = true;
    }else buttonValidity = false;
}

//Process data from GPS Min|Max Value - Altitude Checkbox
void SecondaryWindow::on_checkboxGPSAltInjection_2_clicked()
{
    if(ui->checkboxGPSAltInjection_2->isChecked()){
        GPSAlt = 1;
    }else GPSAlt = 0;
    if(ui->checkboxGPSLatInjection_2->isChecked() || ui->checkboxGPSLonInjection_2->isChecked() || ui->checkboxGPSAltInjection_2->isChecked()){
        buttonValidity = true;
    }else buttonValidity = false;
}

//GPS Fixed Value Cancel Button - Clear field and close secondary window
void SecondaryWindow::on_buttonGPSInjectionCancel_clicked()
{
    ui->doubleSpinBox_startInjection_2->setValue(0);
    ui->doubleSpinBox_endInjection_2->setValue(0);
    startTime = std::numeric_limits<double>::quiet_NaN();
    endTime = std::numeric_limits<double>::quiet_NaN();
    ui->textEditGPSLatInjection_5->clear();
    ui->textEditGPSLonInjection_5->clear();
    ui->textEditGPSAltInjection_5->clear();
    GPSLat = std::numeric_limits<double>::quiet_NaN();
    GPSLon = std::numeric_limits<double>::quiet_NaN();
    GPSAlt = std::numeric_limits<double>::quiet_NaN();
    this->close();
}

//GPS Fixed Value Cancel Button - Clear field and close secondary window
void SecondaryWindow::on_buttonGPSInjectionCancel_2_clicked()
{
    ui->doubleSpinBox_startInjection_4->setValue(0);
    ui->doubleSpinBox_endInjection_4->setValue(0);
    ui->doubleSpinBox_delayvalue->setValue(0);
    delayValue = std::numeric_limits<double>::quiet_NaN();
    startTime = std::numeric_limits<double>::quiet_NaN();
    endTime = std::numeric_limits<double>::quiet_NaN();
    this->close();
}

//GPS Delay Value Cancel Button - Clear field and close secondary window
void SecondaryWindow::on_buttonGPSInjectionCancel_3_clicked()
{
    ui->textEditGPSLatInjection_3->clear();
    ui->textEditGPSLatInjection_6->clear();
    ui->textEditGPSLonInjection_7->clear();
    ui->textEditGPSLonInjection_6->clear();
    ui->textEditGPSAltInjection_3->clear();
    ui->textEditGPSAltInjection_6->clear();
    GPSLatMin = std::numeric_limits<double>::quiet_NaN();
    GPSLatMax = std::numeric_limits<double>::quiet_NaN();
    GPSLonMin = std::numeric_limits<double>::quiet_NaN();
    GPSLonMax = std::numeric_limits<double>::quiet_NaN();
    GPSAltMin = std::numeric_limits<double>::quiet_NaN();
    GPSAltMax = std::numeric_limits<double>::quiet_NaN();
    ui->doubleSpinBox_startInjection_5->setValue(0);
    ui->doubleSpinBox_endInjection_5->setValue(0);
    startTime = std::numeric_limits<double>::quiet_NaN();
    endTime = std::numeric_limits<double>::quiet_NaN();
    this->close();
}

//GPS Min|Max Value Cancel Button - Clear field and close secondary window
void SecondaryWindow::on_buttonGPSInjectionCancel_4_clicked()
{
    ui->doubleSpinBox_startInjection_6->setValue(0);
    ui->doubleSpinBox_endInjection_6->setValue(0);
    startTime = std::numeric_limits<double>::quiet_NaN();
    endTime = std::numeric_limits<double>::quiet_NaN();
    ui->checkboxGPSLatInjection_2->setChecked(false);
    ui->checkboxGPSLonInjection_2->setChecked(false);
    ui->checkboxGPSAltInjection_2->setChecked(false);
    GPSLat = std::numeric_limits<double>::quiet_NaN();
    GPSLon = std::numeric_limits<double>::quiet_NaN();
    GPSAlt = std::numeric_limits<double>::quiet_NaN();
    this->close();
}

//GPS Freeze Value Cancel Button - Clear field and close secondary window
void SecondaryWindow::on_buttonGPSInjectionCancel_5_clicked()
{
    ui->doubleSpinBox_startInjection_3->setValue(0);
    ui->doubleSpinBox_endInjection_3->setValue(0);
    startTime = std::numeric_limits<double>::quiet_NaN();
    endTime = std::numeric_limits<double>::quiet_NaN();
    this->close();
}

//GPS Delay Value - Process delay value
void SecondaryWindow::on_doubleSpinBox_delayvalue_valueChanged(double number)
{
    delayValue = number;
}

//Security Attack Hijack with attacker position continue button. Validate data and send signal
void SecondaryWindow::on_buttonGPSInjectionContinue_6_clicked()
{
    if(((std::isnan(GPSLat)) && (std::isnan(GPSLon)) && (std::isnan(GPSAlt))) || ((GPSLat > 90) || (GPSLat < -90) || (GPSLon > 180) || (GPSLon < -180))){
        QMessageBox::warning(this, "Error", "No valid values given");
    }else if((std::isnan(startTime) || std::isnan(endTime) || startTime > endTime)){
        QMessageBox::warning(this, "Error", "No valid time interval given");
    }
    else{
        emit(windowClosedSecurityPosition(GPSLat, GPSLon, GPSAlt, startTime, endTime));
        ui->doubleSpinBox_startInjection_7->setValue(0);
        ui->doubleSpinBox_endInjection_7->setValue(0);
        startTime = std::numeric_limits<double>::quiet_NaN();
        endTime = std::numeric_limits<double>::quiet_NaN();
        ui->textEditGPSLatInjection_7->clear();
        ui->textEditGPSLonInjection_8->clear();
        ui->textEditGPSAltInjection_7->clear();
        GPSLat = std::numeric_limits<double>::quiet_NaN();
        GPSLon = std::numeric_limits<double>::quiet_NaN();
        GPSAlt = std::numeric_limits<double>::quiet_NaN();
        this->close();
    }
}

//Security Attack Hijack with attacker position latitude value processing
void SecondaryWindow::on_textEditGPSLatInjection_7_textChanged()
{
    bool canShowButton;

    canShowButton = check_gps_validity(ui->textEditGPSLatInjection_7->toPlainText(),
                       ui->textEditGPSLonInjection_8->toPlainText(),
                       ui->textEditGPSAltInjection_7->toPlainText());

    if(canShowButton) {
        ui->buttonGPSInjectionContinue_6->setVisible(true);
        GPSLat = ui->textEditGPSLatInjection_7->toPlainText().toDouble();
    }
    else{
        ui->buttonGPSInjectionContinue_6->setVisible(false);
        GPSLat = std::numeric_limits<double>::quiet_NaN();
    }
}

//Security Attack Hijack with attacker position cancel button. Clear data and close window
void SecondaryWindow::on_buttonGPSInjectionCancel_6_clicked()
{
    ui->doubleSpinBox_startInjection_7->setValue(0);
    ui->doubleSpinBox_endInjection_7->setValue(0);
    startTime = std::numeric_limits<double>::quiet_NaN();
    endTime = std::numeric_limits<double>::quiet_NaN();
    ui->textEditGPSLatInjection_7->clear();
    ui->textEditGPSLonInjection_8->clear();
    ui->textEditGPSAltInjection_7->clear();
    GPSLat = std::numeric_limits<double>::quiet_NaN();
    GPSLon = std::numeric_limits<double>::quiet_NaN();
    GPSAlt = std::numeric_limits<double>::quiet_NaN();
    this->close();
}

//Security Attack Hijack with attacker position end injection value processing
void SecondaryWindow::on_doubleSpinBox_endInjection_7_valueChanged(double number)
{
    endTime = number;
}

//Security Attack Hijack with attacker position start injection value processing
void SecondaryWindow::on_doubleSpinBox_startInjection_7_valueChanged(double number)
{
    startTime = number;
}

//Security Attack Hijack with attacker position longitude value processing
void SecondaryWindow::on_textEditGPSLonInjection_8_textChanged()
{
    bool canShowButton;

    canShowButton = check_gps_validity(ui->textEditGPSLatInjection_7->toPlainText(),
                       ui->textEditGPSLonInjection_8->toPlainText(),
                       ui->textEditGPSAltInjection_7->toPlainText());

    if(canShowButton){
        ui->buttonGPSInjectionContinue_6->setVisible(true);
        GPSLon = ui->textEditGPSLonInjection_8->toPlainText().toDouble();
    }
    else{
        ui->buttonGPSInjectionContinue_6->setVisible(false);
        GPSLon = std::numeric_limits<double>::quiet_NaN();
    }
}

//Security Attack Hijack with attacker position altitude value processing
void SecondaryWindow::on_textEditGPSAltInjection_7_textChanged()
{
    bool canShowButton;

    canShowButton = check_gps_validity(ui->textEditGPSLatInjection_7->toPlainText(),
                       ui->textEditGPSLonInjection_8->toPlainText(),
                       ui->textEditGPSAltInjection_7->toPlainText());

    if(canShowButton){
        ui->buttonGPSInjectionContinue_6->setVisible(true);
        GPSAlt = ui->textEditGPSAltInjection_7->toPlainText().toDouble();
    }
    else{
        ui->buttonGPSInjectionContinue_6->setVisible(false);
        GPSAlt = std::numeric_limits<double>::quiet_NaN();
    }
}

//Security Attack Hijack with uav position start injection value processing
void SecondaryWindow::on_doubleSpinBox_startInjection_8_valueChanged(double number)
{
    startTime = number;
}

//Security Attack Hijack with uav position end injection value processing
void SecondaryWindow::on_doubleSpinBox_endInjection_8_valueChanged(double number)
{
    endTime = number;
}

//Security Attack Hijack with uav position latitude value processing
void SecondaryWindow::on_textEditGPSLatInjection_8_textChanged()
{
    bool canShowButton;

    canShowButton = check_gps_validity(ui->textEditGPSLatInjection_8->toPlainText(),
                       ui->textEditGPSLonInjection_9->toPlainText(),
                       ui->textEditGPSAltInjection_8->toPlainText());

    if(canShowButton) {
        ui->buttonGPSInjectionContinue_7->setVisible(true);
        GPSLat = ui->textEditGPSLatInjection_8->toPlainText().toDouble();
    }
    else{
        ui->buttonGPSInjectionContinue_7->setVisible(false);
        GPSLat = std::numeric_limits<double>::quiet_NaN();
    }
}

//Security Attack Hijack with uav position longitude value processing
void SecondaryWindow::on_textEditGPSLonInjection_9_textChanged()
{
    bool canShowButton;

    canShowButton = check_gps_validity(ui->textEditGPSLatInjection_8->toPlainText(),
                       ui->textEditGPSLonInjection_9->toPlainText(),
                       ui->textEditGPSAltInjection_8->toPlainText());

    if(canShowButton){
        ui->buttonGPSInjectionContinue_7->setVisible(true);
        GPSLon = ui->textEditGPSLonInjection_9->toPlainText().toDouble();
    }
    else{
        ui->buttonGPSInjectionContinue_7->setVisible(false);
        GPSLon = std::numeric_limits<double>::quiet_NaN();
    }
}

//Security Attack Hijack with uav position altitude value processing
void SecondaryWindow::on_textEditGPSAltInjection_8_textChanged()
{
    bool canShowButton;

    canShowButton = check_gps_validity(ui->textEditGPSLatInjection_8->toPlainText(),
                       ui->textEditGPSLonInjection_9->toPlainText(),
                       ui->textEditGPSAltInjection_8->toPlainText());

    if(canShowButton){
        ui->buttonGPSInjectionContinue_7->setVisible(true);
        GPSAlt = ui->textEditGPSAltInjection_8->toPlainText().toDouble();
    }
    else{
        ui->buttonGPSInjectionContinue_7->setVisible(false);
        GPSAlt = std::numeric_limits<double>::quiet_NaN();
    }
}

//Security Attack Hijack with uav position cancel button. Clear data and close window
void SecondaryWindow::on_buttonGPSInjectionCancel_7_clicked()
{
    ui->doubleSpinBox_startInjection_8->setValue(0);
    ui->doubleSpinBox_endInjection_8->setValue(0);
    startTime = std::numeric_limits<double>::quiet_NaN();
    endTime = std::numeric_limits<double>::quiet_NaN();
    ui->textEditGPSLatInjection_8->clear();
    ui->textEditGPSLonInjection_9->clear();
    ui->textEditGPSAltInjection_8->clear();
    GPSLat = std::numeric_limits<double>::quiet_NaN();
    GPSLon = std::numeric_limits<double>::quiet_NaN();
    GPSAlt = std::numeric_limits<double>::quiet_NaN();
    this->close();
}

//Security Attack Hijack with uav position continue button. Validate data and send signal
void SecondaryWindow::on_buttonGPSInjectionContinue_7_clicked()
{
    if(((std::isnan(GPSLat)) && (std::isnan(GPSLon)) && (std::isnan(GPSAlt))) || ((GPSLat > 90) || (GPSLat < -90) || (GPSLon > 180) || (GPSLon < -180))){
        QMessageBox::warning(this, "Error", "No valid values given");
    }else if((std::isnan(startTime) || std::isnan(endTime) || startTime > endTime)){
        QMessageBox::warning(this, "Error", "No valid time interval given");
    }
    else{
        emit(windowClosedSecurityUAVPosition(GPSLat, GPSLon, GPSAlt, startTime, endTime));
        ui->doubleSpinBox_startInjection_8->setValue(0);
        ui->doubleSpinBox_endInjection_8->setValue(0);
        startTime = std::numeric_limits<double>::quiet_NaN();
        endTime = std::numeric_limits<double>::quiet_NaN();
        ui->textEditGPSLatInjection_8->clear();
        ui->textEditGPSLonInjection_9->clear();
        ui->textEditGPSAltInjection_8->clear();
        GPSLat = std::numeric_limits<double>::quiet_NaN();
        GPSLon = std::numeric_limits<double>::quiet_NaN();
        GPSAlt = std::numeric_limits<double>::quiet_NaN();
        this->close();
    }
}

//Security attack random position start injection value processing
void SecondaryWindow::on_doubleSpinBox_startInjection_9_valueChanged(double number)
{
    startTime = number;
}

//Security attack random position end injection value processing
void SecondaryWindow::on_doubleSpinBox_endInjection_9_valueChanged(double number)
{
    endTime = number;
}

//Security Attack random position cancel button. Clear data and close window
void SecondaryWindow::on_buttonGPSInjectionCancel_8_clicked()
{
    ui->textEditGPSLatInjection_4->clear();
    ui->textEditGPSLatInjection_9->clear();
    ui->textEditGPSLonInjection_11->clear();
    ui->textEditGPSLonInjection_10->clear();
    ui->textEditGPSAltInjection_4->clear();
    ui->textEditGPSAltInjection_9->clear();
    GPSLatMin = std::numeric_limits<double>::quiet_NaN();
    GPSLatMax = std::numeric_limits<double>::quiet_NaN();
    GPSLonMin = std::numeric_limits<double>::quiet_NaN();
    GPSLonMax = std::numeric_limits<double>::quiet_NaN();
    GPSAltMin = std::numeric_limits<double>::quiet_NaN();
    GPSAltMax = std::numeric_limits<double>::quiet_NaN();
    ui->doubleSpinBox_startInjection_9->setValue(0);
    ui->doubleSpinBox_endInjection_9->setValue(0);
    startTime = std::numeric_limits<double>::quiet_NaN();
    endTime = std::numeric_limits<double>::quiet_NaN();
    this->close();
}

//Security Attack random position continue button. Validate data and send signal
void SecondaryWindow::on_buttonGPSInjectionContinue_8_clicked()
{
    if(std::isnan(startTime) || std::isnan(endTime) || startTime > endTime){
        QMessageBox::warning(this, "Error", "No valid time interval given");
    }else if(!buttonValidity){
        QMessageBox::warning(this, "Error", "GPS values given invalid");
    }else{
        emit(windowClosedSecurityAttackRandom(GPSLatMin, GPSLatMax, GPSLonMin, GPSLonMax, GPSAltMin, GPSAltMax, startTime, endTime));
        ui->textEditGPSLatInjection_4->clear();
        ui->textEditGPSLatInjection_9->clear();
        ui->textEditGPSLonInjection_11->clear();
        ui->textEditGPSLonInjection_10->clear();
        ui->textEditGPSAltInjection_4->clear();
        ui->textEditGPSAltInjection_9->clear();
        GPSLatMin = std::numeric_limits<double>::quiet_NaN();
        GPSLatMax = std::numeric_limits<double>::quiet_NaN();
        GPSLonMin = std::numeric_limits<double>::quiet_NaN();
        GPSLonMax = std::numeric_limits<double>::quiet_NaN();
        GPSAltMin = std::numeric_limits<double>::quiet_NaN();
        GPSAltMax = std::numeric_limits<double>::quiet_NaN();
        ui->doubleSpinBox_startInjection_9->setValue(0);
        ui->doubleSpinBox_endInjection_9->setValue(0);
        startTime = std::numeric_limits<double>::quiet_NaN();
        endTime = std::numeric_limits<double>::quiet_NaN();
        this->close();
    }
}

//Security Attack Random position validate latitude minimum value
void SecondaryWindow::on_textEditGPSLatInjection_4_textChanged()
{
    bool gpsValidityMin, gpsValidityMax;

    gpsValidityMin = check_gps_validity(ui->textEditGPSLatInjection_4->toPlainText(),
                       ui->textEditGPSLonInjection_11->toPlainText(),
                       ui->textEditGPSAltInjection_4->toPlainText());

    gpsValidityMax = check_gps_validity(ui->textEditGPSLatInjection_9->toPlainText(),
                                        ui->textEditGPSLonInjection_10->toPlainText(),
                                        ui->textEditGPSAltInjection_9->toPlainText());

    if(gpsValidityMin && gpsValidityMax) buttonValidity = true; else buttonValidity = false;
    if(gpsValidityMin) GPSLatMin = ui->textEditGPSLatInjection_4->toPlainText().toDouble(); else GPSLatMin = std::numeric_limits<double>::quiet_NaN();
}

//Security Attack Random position validate latitude maximum value
void SecondaryWindow::on_textEditGPSLatInjection_9_textChanged()
{
    bool gpsValidityMin, gpsValidityMax;

    gpsValidityMin = check_gps_validity(ui->textEditGPSLatInjection_4->toPlainText(),
                       ui->textEditGPSLonInjection_11->toPlainText(),
                       ui->textEditGPSAltInjection_4->toPlainText());

    gpsValidityMax = check_gps_validity(ui->textEditGPSLatInjection_9->toPlainText(),
                                        ui->textEditGPSLonInjection_10->toPlainText(),
                                        ui->textEditGPSAltInjection_9->toPlainText());

    if(gpsValidityMin && gpsValidityMax) buttonValidity = true; else buttonValidity = false;
    if(gpsValidityMin) GPSLatMax = ui->textEditGPSLatInjection_9->toPlainText().toDouble(); else GPSLatMax = std::numeric_limits<double>::quiet_NaN();
}

//Security Attack Random position validate longitude minimum value
void SecondaryWindow::on_textEditGPSLonInjection_11_textChanged()
{
    bool gpsValidityMin, gpsValidityMax;

    gpsValidityMin = check_gps_validity(ui->textEditGPSLatInjection_4->toPlainText(),
                       ui->textEditGPSLonInjection_11->toPlainText(),
                       ui->textEditGPSAltInjection_4->toPlainText());

    gpsValidityMax = check_gps_validity(ui->textEditGPSLatInjection_9->toPlainText(),
                                        ui->textEditGPSLonInjection_10->toPlainText(),
                                        ui->textEditGPSAltInjection_9->toPlainText());

    if(gpsValidityMin && gpsValidityMax) buttonValidity = true; else buttonValidity = false;
    if(gpsValidityMin) GPSLonMin = ui->textEditGPSLonInjection_11->toPlainText().toDouble(); else GPSLonMin = std::numeric_limits<double>::quiet_NaN();
}

//Security Attack Random position validate longitude maximum value
void SecondaryWindow::on_textEditGPSLonInjection_10_textChanged()
{
    bool gpsValidityMin, gpsValidityMax;

    gpsValidityMin = check_gps_validity(ui->textEditGPSLatInjection_4->toPlainText(),
                       ui->textEditGPSLonInjection_11->toPlainText(),
                       ui->textEditGPSAltInjection_4->toPlainText());

    gpsValidityMax = check_gps_validity(ui->textEditGPSLatInjection_9->toPlainText(),
                                        ui->textEditGPSLonInjection_10->toPlainText(),
                                        ui->textEditGPSAltInjection_9->toPlainText());

    if(gpsValidityMin && gpsValidityMax) buttonValidity = true; else buttonValidity = false;
    if(gpsValidityMin) GPSLonMax = ui->textEditGPSLonInjection_10->toPlainText().toDouble(); else GPSLonMax = std::numeric_limits<double>::quiet_NaN();
}

//Security Attack Random position validate altitude minimum value
void SecondaryWindow::on_textEditGPSAltInjection_4_textChanged()
{
    bool gpsValidityMin, gpsValidityMax;

    gpsValidityMin = check_gps_validity(ui->textEditGPSLatInjection_4->toPlainText(),
                       ui->textEditGPSLonInjection_11->toPlainText(),
                       ui->textEditGPSAltInjection_4->toPlainText());

    gpsValidityMax = check_gps_validity(ui->textEditGPSLatInjection_9->toPlainText(),
                                        ui->textEditGPSLonInjection_10->toPlainText(),
                                        ui->textEditGPSAltInjection_9->toPlainText());

    if(gpsValidityMin && gpsValidityMax) buttonValidity = true; else buttonValidity = false;
    if(gpsValidityMin) GPSAltMin = ui->textEditGPSAltInjection_4->toPlainText().toDouble(); else GPSAltMin = std::numeric_limits<double>::quiet_NaN();
}

//Security Attack Random position validate altitude maximum value
void SecondaryWindow::on_textEditGPSAltInjection_9_textChanged()
{
    bool gpsValidityMin, gpsValidityMax;

    gpsValidityMin = check_gps_validity(ui->textEditGPSLatInjection_4->toPlainText(),
                       ui->textEditGPSLonInjection_11->toPlainText(),
                       ui->textEditGPSAltInjection_4->toPlainText());

    gpsValidityMax = check_gps_validity(ui->textEditGPSLatInjection_9->toPlainText(),
                                        ui->textEditGPSLonInjection_10->toPlainText(),
                                        ui->textEditGPSAltInjection_9->toPlainText());

    if(gpsValidityMin && gpsValidityMax) buttonValidity = true; else buttonValidity = false;
    if(gpsValidityMin) GPSAltMax = ui->textEditGPSAltInjection_9->toPlainText().toDouble(); else GPSAltMax = std::numeric_limits<double>::quiet_NaN();
}

//Security attack GPS Delay validate start injection value
void SecondaryWindow::on_doubleSpinBox_startInjection_10_valueChanged(double number)
{
    startTime = number;
}

//Security attack GPS Delay validate end injection value
void SecondaryWindow::on_doubleSpinBox_endInjection_10_valueChanged(double number)
{
    endTime = number;
}

//Security attack GPS Delay validate delay value
void SecondaryWindow::on_doubleSpinBox_delayvalue_2_valueChanged(double number)
{
    delayValue = number;
}

//Security attack GPS Delay cancel button. Clear data and close secondary window
void SecondaryWindow::on_buttonGPSInjectionCancel_9_clicked()
{
    ui->doubleSpinBox_startInjection_10->setValue(0);
    ui->doubleSpinBox_endInjection_10->setValue(0);
    ui->doubleSpinBox_delayvalue_2->setValue(0);
    startTime = std::numeric_limits<double>::quiet_NaN();
    endTime = std::numeric_limits<double>::quiet_NaN();
    delayValue = std::numeric_limits<double>::quiet_NaN();
    this->close();
}

//Security attack GPS Delay continue button. Validate values and send signal to main window
void SecondaryWindow::on_buttonGPSInjectionContinue_9_clicked()
{
    if(std::isnan(startTime) || std::isnan(endTime) || startTime > endTime){
        QMessageBox::warning(this, "Error", "No valid time interval given");
    }else if(std::isnan(delayValue)){
        QMessageBox::warning(this, "Error", "Delay invalid");
    }else{
        emit(windowClosedSecurityAttackDelay(delayValue, startTime, endTime));
        ui->doubleSpinBox_startInjection_10->setValue(0);
        ui->doubleSpinBox_endInjection_10->setValue(0);
        ui->doubleSpinBox_delayvalue_2->setValue(0);
        delayValue = std::numeric_limits<double>::quiet_NaN();
        startTime = std::numeric_limits<double>::quiet_NaN();
        endTime = std::numeric_limits<double>::quiet_NaN();
        this->close();
    }
}

//Security attack random latitude start injection value processing
void SecondaryWindow::on_doubleSpinBox_startInjection_11_valueChanged(double number)
{
    startTime = number;
}

//Security attack random latitude end injection value processing
void SecondaryWindow::on_doubleSpinBox_endInjection_11_valueChanged(double number)
{
    endTime = number;
}

//Security attack random latitude cancel button. Clear data and close secondary window
void SecondaryWindow::on_buttonGPSInjectionCancel_10_clicked()
{
    ui->textEditGPSLatInjection_10->clear();
    ui->textEditGPSLatInjection_11->clear();
    GPSLatMin = std::numeric_limits<double>::quiet_NaN();
    GPSLatMax = std::numeric_limits<double>::quiet_NaN();
    ui->doubleSpinBox_startInjection_11->setValue(0);
    ui->doubleSpinBox_endInjection_11->setValue(0);
    startTime = std::numeric_limits<double>::quiet_NaN();
    endTime = std::numeric_limits<double>::quiet_NaN();
    this->close();
}

//security attack random latitude continue button. Validate values and send signal to main window
void SecondaryWindow::on_buttonGPSInjectionContinue_10_clicked()
{
    if(std::isnan(startTime) || std::isnan(endTime) || startTime > endTime){
        QMessageBox::warning(this, "Error", "No valid time interval given");
    }else if(!buttonValidity){
        QMessageBox::warning(this, "Error", "GPS values given invalid");
    }else{
        emit(windowClosedSecurityAttackRandomLat(GPSLatMin, GPSLatMax, GPSLonMin, GPSLonMax, GPSAltMin, GPSAltMax, startTime, endTime));
        ui->textEditGPSLatInjection_10->clear();
        ui->textEditGPSLatInjection_11->clear();
        GPSLatMin = std::numeric_limits<double>::quiet_NaN();
        GPSLatMax = std::numeric_limits<double>::quiet_NaN();
        ui->doubleSpinBox_startInjection_11->setValue(0);
        ui->doubleSpinBox_endInjection_11->setValue(0);
        startTime = std::numeric_limits<double>::quiet_NaN();
        endTime = std::numeric_limits<double>::quiet_NaN();
        this->close();
    }
}

//security attack random latitude validate latitude minimum value
void SecondaryWindow::on_textEditGPSLatInjection_10_textChanged()
{
    bool gpsValidityMin, gpsValidityMax;

    gpsValidityMin = check_gps_validity(ui->textEditGPSLatInjection_10->toPlainText(),
                       "",
                       "");

    gpsValidityMax = check_gps_validity(ui->textEditGPSLatInjection_11->toPlainText(),
                                        "",
                                        "");

    if(gpsValidityMin && gpsValidityMax) buttonValidity = true; else buttonValidity = false;
    if(gpsValidityMin) GPSLatMin = ui->textEditGPSLatInjection_10->toPlainText().toDouble(); else GPSLatMin = std::numeric_limits<double>::quiet_NaN();
}

//security attack random latitude validate latitude maximum value
void SecondaryWindow::on_textEditGPSLatInjection_11_textChanged()
{
    bool gpsValidityMin, gpsValidityMax;

    gpsValidityMin = check_gps_validity(ui->textEditGPSLatInjection_10->toPlainText(),
                       "",
                       "");

    gpsValidityMax = check_gps_validity(ui->textEditGPSLatInjection_11->toPlainText(),
                                        "",
                                        "");

    if(gpsValidityMin && gpsValidityMax) buttonValidity = true; else buttonValidity = false;
    if(gpsValidityMin) GPSLatMax = ui->textEditGPSLatInjection_11->toPlainText().toDouble(); else GPSLatMax = std::numeric_limits<double>::quiet_NaN();
}

//Security attack random longitude start injection value processing
void SecondaryWindow::on_doubleSpinBox_startInjection_12_valueChanged(double number)
{
    startTime = number;
}

//Security attack random longitude end injection value processing
void SecondaryWindow::on_doubleSpinBox_endInjection_12_valueChanged(double number)
{
   endTime = number;
}

//Security attack random longitude cancel button. Clear data and close secondary window
void SecondaryWindow::on_buttonGPSInjectionCancel_11_clicked()
{
    ui->textEditGPSLonInjection_12->clear();
    ui->textEditGPSLonInjection_13->clear();
    GPSLonMin = std::numeric_limits<double>::quiet_NaN();
    GPSLonMax = std::numeric_limits<double>::quiet_NaN();
    ui->doubleSpinBox_startInjection_12->setValue(0);
    ui->doubleSpinBox_endInjection_12->setValue(0);
    startTime = std::numeric_limits<double>::quiet_NaN();
    endTime = std::numeric_limits<double>::quiet_NaN();
    this->close();
}

//security attack random longitude continue button. Validate values and send signal to main window
void SecondaryWindow::on_buttonGPSInjectionContinue_11_clicked()
{
    if(std::isnan(startTime) || std::isnan(endTime) || startTime > endTime){
        QMessageBox::warning(this, "Error", "No valid time interval given");
    }else if(!buttonValidity){
        QMessageBox::warning(this, "Error", "GPS values given invalid");
    }else{
        emit(windowClosedSecurityAttackRandomLon(GPSLatMin, GPSLatMax, GPSLonMin, GPSLonMax, GPSAltMin, GPSAltMax, startTime, endTime));
        ui->textEditGPSLonInjection_12->clear();
        ui->textEditGPSLonInjection_13->clear();
        GPSLonMin = std::numeric_limits<double>::quiet_NaN();
        GPSLonMax = std::numeric_limits<double>::quiet_NaN();
        ui->doubleSpinBox_startInjection_12->setValue(0);
        ui->doubleSpinBox_endInjection_12->setValue(0);
        startTime = std::numeric_limits<double>::quiet_NaN();
        endTime = std::numeric_limits<double>::quiet_NaN();
        this->close();
    }
}

//security attack random longitude validate longitude minimum value
void SecondaryWindow::on_textEditGPSLonInjection_12_textChanged()
{
    bool gpsValidityMin, gpsValidityMax;

    gpsValidityMin = check_gps_validity("",
                       ui->textEditGPSLonInjection_12->toPlainText(),
                       "");

    gpsValidityMax = check_gps_validity("",
                                        ui->textEditGPSLonInjection_13->toPlainText(),
                                        "");

    if(gpsValidityMin && gpsValidityMax) buttonValidity = true; else buttonValidity = false;
    if(gpsValidityMin) GPSLonMin = ui->textEditGPSLonInjection_12->toPlainText().toDouble(); else GPSLonMin = std::numeric_limits<double>::quiet_NaN();
}

//security attack random longitude validate longitude maximum value
void SecondaryWindow::on_textEditGPSLonInjection_13_textChanged()
{
    bool gpsValidityMin, gpsValidityMax;

    gpsValidityMin = check_gps_validity("",
                       ui->textEditGPSLonInjection_12->toPlainText(),
                       "");

    gpsValidityMax = check_gps_validity("",
                                        ui->textEditGPSLonInjection_13->toPlainText(),
                                        "");

    if(gpsValidityMin && gpsValidityMax) buttonValidity = true; else buttonValidity = false;
    if(gpsValidityMin) GPSLonMax = ui->textEditGPSLonInjection_13->toPlainText().toDouble(); else GPSLonMax = std::numeric_limits<double>::quiet_NaN();
}

//security attack force uav landing start injection value processing
void SecondaryWindow::on_doubleSpinBox_startInjection_13_valueChanged(double number)
{
    startTime = number;
}

//security attack force uav landing end injection value processing
void SecondaryWindow::on_doubleSpinBox_endInjection_13_valueChanged(double number)
{
    endTime = number;
}

//security attack force uav landing cancel button. Clear data and close secondary window
void SecondaryWindow::on_buttonGPSInjectionCancel_12_clicked()
{
    ui->doubleSpinBox_startInjection_13->setValue(0);
    ui->doubleSpinBox_endInjection_13->setValue(0);
    startTime = std::numeric_limits<double>::quiet_NaN();
    endTime = std::numeric_limits<double>::quiet_NaN();
    ui->textEditGPSAltInjection_10->clear();
    GPSAlt = std::numeric_limits<double>::quiet_NaN();
    this->close();
}

//security attack force uav landing continue button. Validate values and send signal
void SecondaryWindow::on_buttonGPSInjectionContinue_12_clicked()
{
    if(std::isnan(GPSAlt)){
        QMessageBox::warning(this, "Error", "No valid values given");
    }else if((std::isnan(startTime) || std::isnan(endTime) || startTime > endTime)){
        QMessageBox::warning(this, "Error", "No valid time interval given");
    }
    else{
        emit(windowClosedSecurityForceLand(GPSLat, GPSLon, GPSAlt, startTime, endTime));
        ui->doubleSpinBox_startInjection_13->setValue(0);
        ui->doubleSpinBox_endInjection_13->setValue(0);
        startTime = std::numeric_limits<double>::quiet_NaN();
        endTime = std::numeric_limits<double>::quiet_NaN();
        ui->textEditGPSAltInjection_10->clear();
        GPSLat = std::numeric_limits<double>::quiet_NaN();
        GPSLon = std::numeric_limits<double>::quiet_NaN();
        GPSAlt = std::numeric_limits<double>::quiet_NaN();
        this->close();
    }
}

//security attack force uav landing validate altitude value
void SecondaryWindow::on_textEditGPSAltInjection_10_textChanged()
{
    bool canShowButton;

    canShowButton = check_gps_validity("",
                       "",
                       ui->textEditGPSAltInjection_10->toPlainText());

    if(canShowButton){
        ui->buttonGPSInjectionContinue_12->setVisible(true);
        GPSAlt = ui->textEditGPSAltInjection_10->toPlainText().toDouble();
    }
    else{
        ui->buttonGPSInjectionContinue_12->setVisible(false);
        GPSAlt = std::numeric_limits<double>::quiet_NaN();
    }
}
