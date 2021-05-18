#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->spinBox_runnumber, SIGNAL(valueChanged(int)), this, SLOT(valueChangedRunNumber(int)));
    connect(ui->doubleSpinBox_allowedDeviation, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_allowedDeviation_valueChanged(double)));
    connect(ui->doubleSpinBox_startInjection, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_startInjection_valueChanged(double)));
    connect(ui->doubleSpinBox_endInjection, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_endInjection_valueChanged(double)));
    connect(ui->comboBox_droneModel, SIGNAL(currentTextChanged(QString)),
                this, SLOT(textChangedDrone(QString)));
    connect(PX4Process, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutputPX4P()));
    connect(MissionProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutputMissionP()));
    connect(PX4Process, SIGNAL(finished(int)), this, SLOT(PX4Processfinished()));
    connect(ui->comboBox_faultinjection_1, SIGNAL(currentTextChanged(QString)), this, SLOT(textChangedFaultInjection_1(QString)));
    connect(ui->comboBox_faultinjection_2, SIGNAL(currentTextChanged(QString)), this, SLOT(textChangedFaultInjection_2(QString)));
    connect(ui->comboBox_faultinjection_3, SIGNAL(currentTextChanged(QString)), this, SLOT(textChangedFaultInjection_3(QString)));
    //connect(newWindow, SIGNAL(destroyed(QObject*)), this, SLOT(WindowClosedReaction()));
    connect(newWindow, SIGNAL(windowClosed(double, double, double)), this, SLOT(WindowClosedReaction(double, double, double)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    if(!option_settings.contains("Mission plan path") or !option_settings.contains("PX4 system path")){
        QMessageBox::warning(this, "Error", "Define mission and system path in options before advancing.");
        return;
    }
    chosenSystem = "PX4";
    ui->stackedWidget->setCurrentIndex(2);
    ui->injectionContinueButton->setVisible(false);
}

void MainWindow::on_pushButton_TOF_goback_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_buttonMissionPlanChoice_clicked()
{
    QString filter = "Mission Plan (*.plan)";
    QString filename = QFileDialog::getOpenFileName(this, "Choose mission plan", option_settings.value("Mission plan path").toString(), filter); //c:// is placeholder, replace with option in options menu
    QFile file(filename);
    QFileInfo fi(file);
    //QMessageBox::information(this, "..", filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Error", "No file chosen");
        return;
    }
    missionPlan = fi.fileName();
    ui->buttonMissionPlanChoice->setText(missionPlan);
    missionChosen = 1;
    if(missionChosen && worldChosen) ui->buttonMissionDetailsContinue->setVisible(true);
}

void MainWindow::on_buttonMissionDetailsContinue_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->pushButton_setinputvalues->setVisible(false);
}


void MainWindow::textChangedDrone(QString text)
{
    if(text == "Iris") droneModel = "iris";
    if(text == "Solo") droneModel = "solo";
    if(text == "Typhoon H480") droneModel = "typhoon_480";
}

void MainWindow::on_buttonEnvironmentDetailsContinue_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->buttonMissionDetailsContinue->setVisible(false);
}

void MainWindow::on_actionPX4_System_path_triggered()
{
    QString filepath = QFileDialog::getExistingDirectory(this, "Choose PX4 System path:", "C://Users//Admin//Desktop", QFileDialog::ShowDirsOnly);
    if(filepath == ""){
        QMessageBox::warning(this, "Error", "No folder path chosen");
    }else{
        QMessageBox::information(this, "Chosen path", "Chosen path was: " + filepath);
        px4_system_path = filepath;
        option_settings.setValue("PX4 system path", filepath);
    }
}

void MainWindow::on_actionMissions_folder_path_triggered()
{
    QString filepath = QFileDialog::getExistingDirectory(this, "Choose PX4 System path:", "C://Users//Admin//Desktop", QFileDialog::ShowDirsOnly);
    if(filepath == ""){
        QMessageBox::warning(this, "Error", "No folder path chosen");
    }else{
        QMessageBox::information(this, "Chosen path", "Chosen path was: " + filepath);
        mission_plan_path = filepath;
        option_settings.setValue("Mission plan path", filepath);
    }
}

void MainWindow::on_pushButton_SFGPSO_goback_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_SFGPSO_confirm_clicked()
{
    QFile runfile;
    QString runfile_name;
    QStringList PX4arguments, MissionArguments;
    auto world_str = QString("\nmake px4_sitl gazebo_%1__%2").arg(droneModel, worldName);
    QString gpsconfigfilename;
    QFile gpsconfig;

    if(option_settings.contains("PX4 system path")){
        gpsconfigfilename = option_settings.value("PX4 system path").toString() + "/build/px4_sitl_default/tmp/rootfs/VehicleGPSConfig.ini";
        gpsconfig.setFileName(gpsconfigfilename);
        if(gpsconfig.open(QIODevice::WriteOnly|QIODevice::Text)){
            qDebug("Create Success!");
            QTextStream out(&gpsconfig);
            out << "[GPS Values]\nGPS Latitude = " << "nan" << "\nGPS Longitude = " <<  "nan" << "\nGPS Altitude = " << "nan"
                << "\n[Mission Details]\nMission Deviation = " << "nan" << "\nStart Injection = " << "nan" << "\nEnd Injection = "
                << "nan";
            gpsconfig.close();
        }else{
            qDebug("Create Failed!");
        }
    }

    ui->stackedWidget->setCurrentIndex(6);
    ui->pushButton_campaignprogress_goback->setVisible(false);
    ui->pushButton_SFGPSO_generatedgraphs->setVisible(false);
    ui->label_2->setVisible(false);
    ui->pushButton_downloadfaulty->setVisible(false);

    PX4Process->setWorkingDirectory(option_settings.value("PX4 system path").toString());
    MissionProcess->setWorkingDirectory(option_settings.value("Mission plan path").toString());

    runfile_name = option_settings.value("PX4 system path").toString() + "/runme";
    runfile.setFileName(runfile_name);
    if(runfile.open(QIODevice::WriteOnly|QIODevice::Text)){
        qDebug("Create Success!");
        QTextStream out(&runfile);
        out << "#!/bin/bash" << world_str;
        runfile.close();
    }else{
        qDebug("Create Failed!");
    }

    qDebug() << world_str;
    PX4arguments << "runme";
    MissionArguments << "dronekit_oldscript.py" << "-path" << missionPlan << "-type" << "0";
    PX4Process->start("/bin/bash", PX4arguments);
    MissionProcess->start("python", MissionArguments);
}

void MainWindow::on_buttonWorldChoice_clicked()
{
    QString filter = "World (*.world)";
    QString filename = QFileDialog::getOpenFileName(this, "Choose mission plan", option_settings.value("PX4 system path").toString() + "/Tools/sitl_gazebo/worlds", filter);
    QFile file(filename);
    QFileInfo fi(file);

    if (!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Error", "No file chosen");
        return;
    }
    worldName = fi.baseName();
    ui->buttonWorldChoice->setText(worldName);
    worldChosen = 1;
    if(worldChosen && missionChosen) ui->buttonMissionDetailsContinue->setVisible(true);
}

//Dronekit output
void MainWindow::readOutputMissionP()
{
    QByteArray outputArray;
    QString convertedOutput;
    outputArray = MissionProcess->readAllStandardOutput();
    convertedOutput = outputArray.toStdString().c_str();
    qDebug() << convertedOutput;
    ui->textEditGPSRProgress->append(outputArray.toStdString().c_str());
    if(convertedOutput.contains("Drawn golden run plot") || convertedOutput.contains("Drawn faulty run plot")){
        MissionProcess->terminate();
    }
}

void MainWindow::on_pushButton_SFGPSP_haltrun_clicked()
{
    //qDebug() << "clicking haltrun";
    if(mustRunPause == 0){
        mustRunPause = 1;
        ui->textEditGPSRProgress->append("Pausing campaign");
    }
    else if(mustRunPause == 1){
        mustRunPause = 0;
        ui->textEditGPSRProgress->append("Resuming campaign");
        callNextRun();
    }
}

//Gazebo Output
void MainWindow::readOutputPX4P(){
    QByteArray outputArray;
    QString convertedOutput;
    outputArray = PX4Process->readAllStandardOutput();
    convertedOutput = outputArray.toStdString().c_str();
    qDebug() << outputArray.toStdString().c_str();
    if(convertedOutput.contains("Failsafe mode activated")){
        ui->textEditGPSRProgress->append("Anomaly detected, failsafe activated, issuing landing order");
    }
    if(convertedOutput.contains("closed logfile")){
        PX4Process->terminate();
    }
}

void MainWindow::on_pushButton_Title_Start_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_Title_Upload_clicked()
{
    QString filter = "Fault Campaign (*.fc)";
    QString filename = QFileDialog::getOpenFileName(this, "Choose fault campaign file", "Fault Campaigns", filter);
    QFile file(filename);

    if (!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Error", "No file chosen");
        return;
    }

    QTextStream in(&file);
    while(!in.atEnd()){
        QString line = in.readLine();
        qDebug() << line;
        if(line.contains("System")) chosenSystem = line.section(' ', -1);
        if(line.contains("Type of Failure")) chosenInjectionChoice = line.section(' ', -1);
        if(chosenInjectionChoice.compare("GPS") == 0){
            if(line.contains("GPS Latitude")){
                GPSLat = line.section(' ', -1).toDouble();
            }
            if(line.contains("GPS Longitude")){
                GPSLon = line.section(' ', -1).toDouble();
            }
            if(line.contains("GPS Altitude")){
                GPSAlt = line.section(' ', -1).toDouble();
            }
        }
        if(line.contains("Mission Deviation")) missionDeviation = line.section(' ', -1).toDouble();
        if(line.contains("Start Injection")) startInjectionTime = line.section(' ', -1).toDouble();
        if(line.contains("End Injection")) endInjectionTime = line.section(' ', -1).toDouble();
        if(line.contains("Mission Plan")) missionPlan = line.section(' ', -1);
        if(line.contains("World chosen")) worldName = line.section(' ', -1);
        if(line.contains("Drone Model")) droneModel = line.section(' ', -1);
    }
    ui->stackedWidget->setCurrentIndex(5);
    ui->pushButton_SFGPSO_savecampaign->setVisible(false);
    ui->textEditOverview->append("Working on a " + chosenSystem + " system");
    ui->textEditOverview->append("Type of failure is " + chosenInjectionChoice);
    if(chosenInjectionChoice.compare("GPS") == 0){
        ui->textEditOverview->append(QString("GPS Latitude value is: %1").arg(GPSLat));
        ui->textEditOverview->append(QString("GPS Longitude value is: %1").arg(GPSLon));
        ui->textEditOverview->append(QString("GPS Altitude value is: %1").arg(GPSAlt));
    }
    ui->textEditOverview->append("Start injection time is: " + QString::number(startInjectionTime));
    ui->textEditOverview->append("End injection time is: " + QString::number(endInjectionTime));
    ui->textEditOverview->append("Mission deviation from golden run is: " + QString::number(missionDeviation));
    ui->textEditOverview->append("Mission plan is: " + missionPlan);
    ui->textEditOverview->append("World chosen is: " + worldName);
    ui->textEditOverview->append("Drone model is: " + droneModel);
}

void MainWindow::on_pushButton_SFGPSMD_goback_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_pushButton_SFGPSED_goback_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_SFGPSO_savecampaign_clicked()
{
    QString campaignFilename;
    QFile campaignFile;
    QDir campaignFolder;

    if(!QDir("Fault Campaigns").exists()){
        QDir().mkdir("Fault Campaigns");
        campaignFolder.setPath("Fault Campaigns");
    }else{
        campaignFolder.setPath("Fault Campaigns");
    }

    campaignFolder.setFilter(QDir::NoDotAndDotDot | QDir::Files);

    campaignFilename = "Fault Campaigns/" + QString("Fault_Campaign_%1.fc").arg(campaignFolder.count());
    campaignFile.setFileName(campaignFilename);
    if(campaignFile.open(QIODevice::WriteOnly|QIODevice::Text)){
        qDebug("Campaign file create success!");
        QTextStream out(&campaignFile);
        out << "System = " << chosenSystem << "\n";
        if(chosenInjectionChoice.compare("gps") == 0){
            out << "Type of Failure = GPS\n";
            out << QString("[GPS Values]\nGPS Latitude = ") << QString::number(GPSLat) << QString("\nGPS Longitude = ") << QString::number(GPSLon) << QString("\nGPS Altitude = ") << QString::number(GPSAlt) << "\n";
        }
        out << "[Mission Details]\nMission Deviation = " << QString::number(missionDeviation) << "\nStart Injection = " << QString::number(startInjectionTime) << "\nEnd Injection = " << QString::number(endInjectionTime);
        out << "\nMission Plan = " << missionPlan;
        out << "\n[Environmental Details]\nWorld chosen = " << worldName << "\nDrone model = " << droneModel;
        campaignFile.close();
    }else{
        qDebug("Campaign file create failed!");
    }
}

void MainWindow::valueChangedRunNumber(int number){
    numberRuns = number;
}

void MainWindow::PX4Processfinished(){

    MissionProcess->waitForFinished();
    MissionProcess->terminate();

    if(mustRunPause == 0) callNextRun();

}

void MainWindow::callNextRun(){
    QStringList PX4arguments, MissionArguments;
    QString gpsconfigfilename;
    QFile gpsconfig;

    currentRun++;

    if(currentRun == 1){
        if(option_settings.contains("PX4 system path")){
            gpsconfigfilename = option_settings.value("PX4 system path").toString() + "/build/px4_sitl_default/tmp/rootfs/VehicleGPSConfig.ini";
            gpsconfig.setFileName(gpsconfigfilename);
            if(gpsconfig.open(QIODevice::WriteOnly|QIODevice::Text)){
                qDebug("Create Success!");
                QTextStream out(&gpsconfig);
                out << QString("[GPS Values]\nGPS Latitude = ") << QString::number(GPSLat) << QString("\nGPS Longitude = ") << QString::number(GPSLon) << QString("\nGPS Altitude = ") << QString::number(GPSAlt)
                    << "\n[Mission Details]\nMission Deviation = " << QString::number(missionDeviation) << "\nStart Injection = " << QString::number(startInjectionTime) << "\nEnd Injection = "
                    << QString::number(endInjectionTime);
                gpsconfig.close();
            }else{
                qDebug("Create Failed!");
            }
        }
    }

    if(currentRun <= numberRuns){
        ui->textEditGPSRProgress->append("Starting Run: " + QString::number(currentRun));
        PX4Process->setWorkingDirectory(option_settings.value("PX4 system path").toString());
        MissionProcess->setWorkingDirectory(option_settings.value("Mission plan path").toString());
        PX4arguments << "runme";
        MissionArguments << "dronekit_oldscript.py" << "-path" << missionPlan << "-type" << "1";
        PX4Process->start("/bin/bash", PX4arguments);
        MissionProcess->start("python", MissionArguments);
    }else{
        ui->textEditGPSRProgress->append("Failure campaign over");
        ui->pushButton_abortCampaign->setVisible(false);
        ui->pushButton_SFGPSP_haltrun->setVisible(false);
        ui->pushButton_campaignprogress_goback->setVisible(true);
        ui->pushButton_SFGPSO_generatedgraphs->setVisible(true);
        ui->label_2->setVisible(true);
        ui->pushButton_downloadfaulty->setVisible(true);
    }
}

void MainWindow::textChangedFaultInjection_1(QString text)
{
    if(text == "Software Failures"){
        ui->comboBox_faultinjection_2->clear();
        ui->comboBox_faultinjection_3->clear();
        ui->comboBox_faultinjection_2->addItem("GPS Failure");
        ui->comboBox_faultinjection_2->setCurrentIndex(-1);
    }
    if(text == "Communication Failures"){
        ui->comboBox_faultinjection_2->clear();
        ui->comboBox_faultinjection_3->clear();
    }
    if(text == "AI Failures"){
        ui->comboBox_faultinjection_2->clear();
        ui->comboBox_faultinjection_3->clear();
    }
    if(text == "Security Attack"){
        ui->comboBox_faultinjection_2->clear();
        ui->comboBox_faultinjection_3->clear();
    }
}

void MainWindow::textChangedFaultInjection_2(QString text)
{
    if(text == "GPS Failure"){
       chosenInjectionChoice = "gps";
       ui->comboBox_faultinjection_3->clear();
       ui->comboBox_faultinjection_3->addItem("Freeze Value");
       ui->comboBox_faultinjection_3->addItem("Fixed Value");
       ui->comboBox_faultinjection_3->setCurrentIndex(-1);
    }
}

void MainWindow::MainWindow::textChangedFaultInjection_3(QString text)
{
    if(text == "Fixed Value"){
        ui->pushButton_setinputvalues->setVisible(true);
    }
}


void MainWindow::on_pushButton_setinputvalues_clicked()
{
    if(ui->comboBox_faultinjection_3->currentText() == "Fixed Value"){
        newWindow->setAttribute(Qt::WA_DeleteOnClose);
        newWindow->show();
    }
    ui->injectionContinueButton->setVisible(true);
}

void MainWindow::on_injectionContinueButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
    ui->textEditOverview->append("Working on a PX4 architecture");
    //if(!std::isnan(GPSLat)) ui->textEditOverview->append(QString("Injecting GPS latitude as: %1").arg(GPSLat));
    //if(!std::isnan(GPSLon)) ui->textEditOverview->append(QString("Injecting GPS longitude as: %1").arg(GPSLon));
    //if(!std::isnan(GPSAlt)) ui->textEditOverview->append(QString("Injecting GPS altitude as: %1").arg(GPSAlt));
    for(int i = 0; i < failuresCampaign->size(); ++i){
        ui->textEditOverview->append(failuresCampaign->at(i));
    }
    ui->textEditOverview->append("Using mission plan " + missionPlan + " with a deviation of " + QString::number(missionDeviation) + " seconds");
    ui->textEditOverview->append("Using a " + droneModel + " drone");
}

void MainWindow::WindowClosedReaction(double GPSLatR, double GPSLonR, double GPSAltR){
    GPSLat = GPSLatR;
    GPSLon = GPSLonR;
    GPSAlt = GPSAltR;
    if(std::isnan(GPSLat) && (std::isnan(GPSLon)) && (!std::isnan(GPSAlt)))  failuresCampaign->append(QString("GPS Fixed Value: GPS Altitude - %1").arg(GPSAlt));
    else if(std::isnan(GPSLat) && (!std::isnan(GPSLon)) && (std::isnan(GPSAlt)))     failuresCampaign->append(QString("GPS Fixed Value: GPS Longitude - %1").arg(GPSLon));
    else if(!std::isnan(GPSLat) && (std::isnan(GPSLon)) && (std::isnan(GPSAlt)))     failuresCampaign->append(QString("GPS Fixed Value: GPS Latitude - %1").arg(GPSLat));
    else if(!std::isnan(GPSLat) && (!std::isnan(GPSLon)) && (std::isnan(GPSAlt)))     failuresCampaign->append(QString("GPS Fixed Value: GPS Latitude - %1 | GPS Longitude - %2").arg(GPSLat).arg(GPSLon));
    else if(!std::isnan(GPSLat) && (std::isnan(GPSLon)) && (!std::isnan(GPSAlt)))     failuresCampaign->append(QString("GPS Fixed Value: GPS Latitude - %1 | GPS Altitude - %2").arg(GPSLat).arg(GPSAlt));
    else if(std::isnan(GPSLat) && (!std::isnan(GPSLon)) && (!std::isnan(GPSAlt)))     failuresCampaign->append(QString("GPS Fixed Value: GPS Longitude - %1 | GPS Altitude - %2").arg(GPSLon).arg(GPSAlt));

}

void MainWindow::on_pushButton_results_goback_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_SFGPSO_generatedgraphs_clicked()
{
    QPixmap goldenrun(option_settings.value("Mission plan path").toString() + "/drone_golden_run.png");
    QPixmap faultyrun(option_settings.value("Mission plan path").toString() + "/drone_faulty_run.png");
    ui->stackedWidget->setCurrentIndex(7);
    ui->label_goldenrunimage->setPixmap(goldenrun.scaled(ui->label_goldenrunimage->width(), ui->label_goldenrunimage->height(),Qt::IgnoreAspectRatio));
    ui->label_faultyrunimage->setPixmap(faultyrun.scaled(ui->label_faultyrunimage->width(), ui->label_faultyrunimage->height(),Qt::IgnoreAspectRatio));
}

void MainWindow::on_pushButton_definedcampaign_clicked()
{
    QTextEdit *campaignShower = new QTextEdit();
    campaignShower->setWindowModality(Qt::ApplicationModal);
    campaignShower->setWindowTitle("Active Failures");
    campaignShower->setFixedSize(500, 300);
    for(int i = 0; i < failuresCampaign->size(); ++i){
        campaignShower->append(failuresCampaign->at(i));
    }
    campaignShower->show();
}

void MainWindow::on_doubleSpinBox_allowedDeviation_valueChanged(double number)
{
    missionDeviation = number;
}

void MainWindow::on_doubleSpinBox_startInjection_valueChanged(double number)
{
    startInjectionTime = number;
}

void MainWindow::on_doubleSpinBox_endInjection_valueChanged(double number)
{
    endInjectionTime = number;
}

void MainWindow::on_pushButton_campaignprogress_goback_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_abortCampaign_clicked()
{
    currentRun = numberRuns + 1;
    MissionProcess->terminate();
    PX4Process->terminate();
    ui->textEditGPSRProgress->append("Aborting campaign");
}
