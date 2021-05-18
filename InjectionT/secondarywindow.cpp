#include "secondarywindow.h"
#include "ui_secondarywindow.h"

SecondaryWindow::SecondaryWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondaryWindow)
{
    ui->setupUi(this);

    connect(ui->comboBoxGPS_Lat_2, SIGNAL(currentTextChanged(QString)), this, SLOT(textChangedGPSLat(QString)));
    connect(ui->comboBoxGPS_Lon_2, SIGNAL(currentTextChanged(QString)), this, SLOT(textChangedGPSLon(QString)));
    connect(ui->comboBoxGPS_Alt_2, SIGNAL(currentTextChanged(QString)), this, SLOT(textChangedGPSAlt(QString)));
}

SecondaryWindow::~SecondaryWindow()
{
    delete ui;
}

bool SecondaryWindow::check_gps_validity(QString lat, QString lon, QString alt, int isFloatAllowed){
    double lat_converted;
    double lon_converted;
    double alt_converted;

    switch(isFloatAllowed){
        case 0:
            lat_converted = lat.toInt();
            lon_converted = lon.toInt();
            alt_converted = alt.toInt();
            break;
        case 1:
            lat_converted = lat.toInt();
            lon_converted = lon.toDouble();
            alt_converted = alt.toInt();
            break;
        case 3:
            lat_converted = lat.toDouble();
            lon_converted = lon.toInt();
            alt_converted = alt.toInt();
            break;
        case 4:
            lat_converted = lat.toDouble();
            lon_converted = lon.toDouble();
            alt_converted = alt.toInt();
            break;
        case 5:
            lat_converted = lat.toInt();
            lon_converted = lon.toInt();
            alt_converted = alt.toDouble();
            break;
        case 6:
            lat_converted = lat.toInt();
            lon_converted = lon.toDouble();
            alt_converted = alt.toDouble();
            break;
        case 8:
            lat_converted = lat.toDouble();
            lon_converted = lon.toInt();
            alt_converted = alt.toDouble();
            break;
        default:
            //error
            break;
    }
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
    }
    return false;
}

//1 lon || 3 lat || 5 alt || 4 lon + lat || 6 lon + alt || 8 alt + lat
int SecondaryWindow::gps_which_are_float(bool lon, bool lat, bool alt){
    if(lon and !lat and !alt) return 1;
    else if(!lon and lat and !alt) return 3;
    else if(!lon and !lat and alt) return 5;
    else if(lon and lat and !alt) return 4;
    else if(lon and !lat and alt) return 6;
    else if(!lon and lat and alt) return 8;
    else return 0;
}

void SecondaryWindow::on_buttonRandomizeGPSInjection_clicked()
{
    QString min_value, max_value;
    double min_value_f, max_value_f, randomized_value;
    int min_value_i, max_value_i;
    if(ui->checkboxGPSLatInjection->isChecked() == true){
          if(ui->comboBoxGPS_Lat_2->currentText() == "Decimal"){
              min_value = ui->textEditGPSLatInjection_2->toPlainText();
              max_value = ui->textEditGPSLatInjection_4->toPlainText();
              min_value_f = min_value.toDouble();
              max_value_f = max_value.toDouble();
              if(min_value_f != 0.0 and max_value_f != 0.0 and max_value_f > min_value_f){
                  randomized_value = min_value_f + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(max_value_f-min_value_f)));
                  ui->textEditGPSLatInjection_3->setText(QString::number(randomized_value));
              }
          }else{
              min_value = ui->textEditGPSLatInjection_2->toPlainText();
              max_value = ui->textEditGPSLatInjection_4->toPlainText();
              min_value_i = min_value.toInt();
              max_value_i = max_value.toInt();
              if(max_value_i > min_value_i){
                  randomized_value = rand() % max_value_i;
                  if(randomized_value < min_value_i) randomized_value = randomized_value + min_value_i;
                  ui->textEditGPSLatInjection_3->setText(QString::number(randomized_value));
              }
          }
    }
    if(ui->checkboxGPSLonInjection->isChecked() == true){
          if(ui->comboBoxGPS_Lon_2->currentText() == "Decimal"){
              min_value = ui->textEditGPSLonInjection_4->toPlainText();
              max_value = ui->textEditGPSLonInjection_3->toPlainText();
              min_value_f = min_value.toDouble();
              max_value_f = max_value.toDouble();
              if(min_value_f != 0.0 and max_value_f != 0.0 and max_value_f > min_value_f){
                  randomized_value = min_value_f + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(max_value_f-min_value_f)));
                  ui->textEditGPSLonInjection_2->setText(QString::number(randomized_value));
              }
          }else{
              min_value = ui->textEditGPSLonInjection_4->toPlainText();
              max_value = ui->textEditGPSLonInjection_3->toPlainText();
              min_value_i = min_value.toInt();
              max_value_i = max_value.toInt();
              if(max_value_i > min_value_i){
                  randomized_value = rand() % max_value_i;
                  if(randomized_value < min_value_i) randomized_value = randomized_value + min_value_i;
                  ui->textEditGPSLonInjection_2->setText(QString::number(randomized_value));
              }
          }
    }
    if(ui->checkboxGPSAltInjection->isChecked() == true){
          if(ui->comboBoxGPS_Alt_2->currentText() == "Decimal"){
              min_value = ui->textEditGPSAltInjection_2->toPlainText();
              max_value = ui->textEditGPSAltInjection_4->toPlainText();
              min_value_f = min_value.toDouble();
              max_value_f = max_value.toDouble();
              if(min_value_f != 0.0 and max_value_f != 0.0 and max_value_f > min_value_f){
                  randomized_value = min_value_f + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(max_value_f-min_value_f)));
                  ui->textEditGPSAltInjection_3->setText(QString::number(randomized_value));
              }
          }else{
              min_value = ui->textEditGPSAltInjection_2->toPlainText();
              max_value = ui->textEditGPSAltInjection_4->toPlainText();
              min_value_i = min_value.toInt();
              max_value_i = max_value.toInt();
              if(max_value_i > min_value_i){
                  randomized_value = rand() % max_value_i;
                  if(randomized_value < min_value_i) randomized_value = randomized_value + min_value_i;
                  ui->textEditGPSAltInjection_3->setText(QString::number(randomized_value));
              }
          }
    }
}

void SecondaryWindow::on_textEditGPSLatInjection_3_textChanged()
{
    bool canShowButton;

    bool lat, lon, alt;
    if(ui->comboBoxGPS_Lat_2->currentText() == "Decimal") lat = true; else lat = false;
    if(ui->comboBoxGPS_Lon_2->currentText() == "Decimal") lon = true; else lon = false;
    if(ui->comboBoxGPS_Alt_2->currentText() == "Decimal") alt = true; else alt = false;

    canShowButton = check_gps_validity(ui->textEditGPSLatInjection_3->toPlainText(),
                       ui->textEditGPSLonInjection_2->toPlainText(),
                       ui->textEditGPSAltInjection_3->toPlainText(), gps_which_are_float(lon,
                                                                                       lat,
                                                                                       alt));
    if(canShowButton) {
        ui->buttonGPSInjectionContinue->setVisible(true);
        GPSLat = ui->textEditGPSLatInjection_3->toPlainText().toDouble();
    }
    else{
        ui->buttonGPSInjectionContinue->setVisible(false);
        GPSLat = std::numeric_limits<double>::quiet_NaN();
    }
}

void SecondaryWindow::on_textEditGPSLonInjection_2_textChanged()
{
    bool canShowButton;
    bool lat, lon, alt;

    if(ui->comboBoxGPS_Lat_2->currentText() == "Decimal") lat = true; else lat = false;
    if(ui->comboBoxGPS_Lon_2->currentText() == "Decimal") lon = true; else lon = false;
    if(ui->comboBoxGPS_Alt_2->currentText() == "Decimal") alt = true; else alt = false;

    canShowButton = check_gps_validity(ui->textEditGPSLatInjection_3->toPlainText(),
                       ui->textEditGPSLonInjection_2->toPlainText(),
                       ui->textEditGPSAltInjection_3->toPlainText(), gps_which_are_float(lon,
                                                                                       lat,
                                                                                       alt));
    if(canShowButton){
        ui->buttonGPSInjectionContinue->setVisible(true);
        GPSLon = ui->textEditGPSLonInjection_2->toPlainText().toDouble();
    }
    else{
        ui->buttonGPSInjectionContinue->setVisible(false);
        GPSLon = std::numeric_limits<double>::quiet_NaN();
    }
}

void SecondaryWindow::on_textEditGPSAltInjection_3_textChanged()
{
    bool canShowButton;
    bool lat, lon, alt;

    if(ui->comboBoxGPS_Lat_2->currentText() == "Decimal") lat = true; else lat = false;
    if(ui->comboBoxGPS_Lon_2->currentText() == "Decimal") lon = true; else lon = false;
    if(ui->comboBoxGPS_Alt_2->currentText() == "Decimal") alt = true; else alt = false;

    canShowButton = check_gps_validity(ui->textEditGPSLatInjection_3->toPlainText(),
                       ui->textEditGPSLonInjection_2->toPlainText(),
                       ui->textEditGPSAltInjection_3->toPlainText(), gps_which_are_float(lon,
                                                                                       lat,
                                                                                       alt));
    if(canShowButton){
        ui->buttonGPSInjectionContinue->setVisible(true);
        GPSAlt = ui->textEditGPSAltInjection_3->toPlainText().toDouble();
    }
    else{
        ui->buttonGPSInjectionContinue->setVisible(false);
        GPSAlt = std::numeric_limits<double>::quiet_NaN();
    }
}

void SecondaryWindow::on_checkboxGPSLatInjection_clicked()
{
    if(ui->checkboxGPSLatInjection->isChecked()){
        ui->textEditGPSLatInjection_2->setVisible(true);
        ui->textEditGPSLatInjection_4->setVisible(true);
        ui->label_7->setVisible(true);
        ui->label_8->setVisible(true);
    }else{
        ui->textEditGPSLatInjection_2->setVisible(false);
        ui->textEditGPSLatInjection_4->setVisible(false);
    }
    if(!ui->checkboxGPSLatInjection->isChecked() and !ui->checkboxGPSLonInjection->isChecked() and !ui->checkboxGPSAltInjection->isChecked()){
        ui->label_7->setVisible(false);
        ui->label_8->setVisible(false);
    }
}

void SecondaryWindow::on_checkboxGPSLonInjection_clicked()
{
    if(ui->checkboxGPSLonInjection->isChecked()){
        ui->textEditGPSLonInjection_3->setVisible(true);
        ui->textEditGPSLonInjection_4->setVisible(true);
        ui->label_7->setVisible(true);
        ui->label_8->setVisible(true);
    }else{
        ui->textEditGPSLonInjection_3->setVisible(false);
        ui->textEditGPSLonInjection_4->setVisible(false);
    }
    if(!ui->checkboxGPSLatInjection->isChecked() and !ui->checkboxGPSLonInjection->isChecked() and !ui->checkboxGPSAltInjection->isChecked()){
        ui->label_7->setVisible(false);
        ui->label_8->setVisible(false);
    }
}

void SecondaryWindow::on_checkboxGPSAltInjection_clicked()
{
    if(ui->checkboxGPSAltInjection->isChecked()){
        ui->textEditGPSAltInjection_2->setVisible(true);
        ui->textEditGPSAltInjection_4->setVisible(true);
        ui->label_7->setVisible(true);
        ui->label_8->setVisible(true);
    }else{
        ui->textEditGPSAltInjection_2->setVisible(false);
        ui->textEditGPSAltInjection_4->setVisible(false);
    }
    if(!ui->checkboxGPSLatInjection->isChecked() and !ui->checkboxGPSLonInjection->isChecked() and !ui->checkboxGPSAltInjection->isChecked()){
        ui->label_7->setVisible(false);
        ui->label_8->setVisible(false);
    }
}

void SecondaryWindow::textChangedGPSLat(QString text)
{
    bool canShowButton;
    bool lat, lon, alt;
    if(text == "Decimal") lat = true; else lat = false;
    if(ui->comboBoxGPS_Lon_2->currentText() == "Decimal") lon = true; else lon = false;
    if(ui->comboBoxGPS_Alt_2->currentText() == "Decimal") alt = true; else alt = false;

    canShowButton = check_gps_validity(ui->textEditGPSLatInjection_3->toPlainText(),
                       ui->textEditGPSLonInjection_2->toPlainText(),
                       ui->textEditGPSAltInjection_3->toPlainText(), gps_which_are_float(lon, lat, alt));
    if(canShowButton) ui->buttonGPSInjectionContinue->setVisible(true);
    else ui->buttonGPSInjectionContinue->setVisible(false);
}

void SecondaryWindow::textChangedGPSLon(QString text)
{
    bool canShowButton;
    bool lat, lon, alt;
    if(ui->comboBoxGPS_Lat_2->currentText() == "Decimal") lat = true; else lat = false;
    if(text == "Decimal") lon = true; else lon = false;
    if(ui->comboBoxGPS_Alt_2->currentText() == "Decimal") alt = true; else alt = false;

    canShowButton = check_gps_validity(ui->textEditGPSLatInjection_3->toPlainText(),
                       ui->textEditGPSLonInjection_2->toPlainText(),
                       ui->textEditGPSAltInjection_3->toPlainText(), gps_which_are_float(lon, lat, alt));
    if(canShowButton) ui->buttonGPSInjectionContinue->setVisible(true);
    else ui->buttonGPSInjectionContinue->setVisible(false);
}

void SecondaryWindow::textChangedGPSAlt(QString text)
{
    bool canShowButton;
    bool lat, lon, alt;
    if(ui->comboBoxGPS_Lat_2->currentText() == "Decimal") lat = true; else lat = false;
    if(ui->comboBoxGPS_Lon_2->currentText() == "Decimal") lon = true; else lon = false;
    if(text == "Decimal") alt = true; else alt = false;

    canShowButton = check_gps_validity(ui->textEditGPSLatInjection_3->toPlainText(),
                       ui->textEditGPSLonInjection_2->toPlainText(),
                       ui->textEditGPSAltInjection_3->toPlainText(), gps_which_are_float(lon, lat, alt));
    if(canShowButton) ui->buttonGPSInjectionContinue->setVisible(true);
    else ui->buttonGPSInjectionContinue->setVisible(false);
}

void SecondaryWindow::on_buttonGPSInjectionContinue_clicked()
{
    if(std::isnan(GPSLat) && std::isnan(GPSLon) && std::isnan(GPSAlt)){
        QMessageBox::warning(this, "Error", "No valid values given");
    }else{
        emit(windowClosed(GPSLat, GPSLon, GPSAlt));
        this->close();
    }
}
