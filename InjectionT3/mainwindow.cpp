#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    validRuns.fill("", 999);
    connect(ui->spinBox_runnumber, SIGNAL(valueChanged(int)), this, SLOT(valueChangedRunNumber(int)));
    connect(ui->doubleSpinBox_allowedDeviation, SIGNAL(valueChanged(double)), this, SLOT(on_doubleSpinBox_allowedDeviation_valueChanged(double)));
    connect(ui->comboBox_droneModel, SIGNAL(currentTextChanged(QString)),
                this, SLOT(textChangedDrone(QString)));
    connect(PX4Process, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutputPX4P()));
    connect(MissionProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutputMissionP()));
    connect(PX4Process, SIGNAL(finished(int)), this, SLOT(PX4Processfinished()));
    connect(ui->comboBox_faultinjection_1, SIGNAL(currentTextChanged(QString)), this, SLOT(textChangedFaultInjection_1(QString)));
    connect(ui->comboBox_faultinjection_2, SIGNAL(currentTextChanged(QString)), this, SLOT(textChangedFaultInjection_2(QString)));
    connect(ui->comboBox_faultinjection_3, SIGNAL(currentTextChanged(QString)), this, SLOT(textChangedFaultInjection_3(QString)));
    connect(newWindow, SIGNAL(windowClosedGPSFixed(double, double, double, double, double)), this, SLOT(WindowClosedReactionGPSFixed(double, double, double, double, double)));
    connect(newWindow, SIGNAL(windowClosedGPSFreeze(double,double)), this, SLOT(WindowClosedReactionGPSFreeze(double,double)));
    connect(newWindow, SIGNAL(windowClosedGPSDelay(double, double,double)), this, SLOT(WindowClosedReactionGPSDelay(double,double,double)));
    connect(newWindow, SIGNAL(windowClosedGPSRandom(double,double,double,double,double,double,double,double)), this, SLOT(WindowClosedReactionGPSRandom(double,double,double,double,double,double,double,double)));
    connect(newWindow, SIGNAL(windowClosedGPSMinMax(double,double,double,double,double)), this, SLOT(WindowClosedReactionGPSMinMax(double,double,double,double,double)));
    connect(newWindow, SIGNAL(windowClosedSecurityPosition(double, double, double, double, double)), this, SLOT(WindowClosedReactionSecurityPosition(double, double, double, double, double)));
    connect(newWindow, SIGNAL(windowClosedSecurityUAVPosition(double, double, double, double, double)), this, SLOT(WindowClosedReactionSecurityUAVPosition(double, double, double, double, double)));
    connect(newWindow, SIGNAL(windowClosedSecurityAttackRandom(double,double,double,double,double,double,double,double)), this, SLOT(WindowClosedSecurityAttackRandomReaction(double,double,double,double,double,double,double,double)));
    connect(newWindow, SIGNAL(windowClosedSecurityAttackRandomLat(double,double,double,double,double,double,double,double)), this, SLOT(WindowClosedSecurityAttackRandomLatReaction(double,double,double,double,double,double,double,double)));
    connect(newWindow, SIGNAL(windowClosedSecurityAttackRandomLon(double,double,double,double,double,double,double,double)), this, SLOT(WindowClosedSecurityAttackRandomLonReaction(double,double,double,double,double,double,double,double)));
    connect(newWindow, SIGNAL(windowClosedSecurityAttackDelay(double, double,double)), this, SLOT(WindowClosedReactionSecurityAttackDelay(double,double,double)));
    connect(newWindow, SIGNAL(windowClosedSecurityForceLand(double, double, double, double, double)), this, SLOT(WindowClosedReactionSecurityForceLand(double, double, double, double, double)));
    connect(this, SIGNAL(emitWindowType(QString)), newWindow, SLOT(changeWindowType(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//PX4 Button Clicked on Screen 2
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

//Configure injection campaign go back button
void MainWindow::on_pushButton_TOF_goback_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

//Button to choose and confirm a plan file
void MainWindow::on_buttonMissionPlanChoice_clicked()
{
    QString filter = "Mission Plan (*.plan)";
    QString filename = QFileDialog::getOpenFileName(this, "Choose mission plan", option_settings.value("Mission plan path").toString(), filter); //c:// is placeholder, replace with option in options menu
    QFile file(filename);
    QFileInfo fi(file);

    if (!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Error", "No file chosen");
        return;
    }
    missionPlan = fi.fileName();
    ui->buttonMissionPlanChoice->setText(missionPlan);
    missionChosen = 1;
    if(missionChosen && worldChosen) ui->buttonMissionDetailsContinue->setVisible(true);
}

//continue button on mission and environment screen
void MainWindow::on_buttonMissionDetailsContinue_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->pushButton_setinputvalues->setVisible(false);
}

//validate UAV choice on uav selection screen
void MainWindow::textChangedDrone(QString text)
{
    if(text == "Iris") droneModel = "iris";
    if(text == "Solo") droneModel = "solo";
    if(text == "Typhoon H480") droneModel = "typhoon_480";
}

//continue button on UAV selection screen
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

    //read world file for initial drone coordinates
    //QString home_lon_value = QString("export PX4_HOME_LON=1.4009819178705\n");
    //QString home_lat_value = QString("\nexport PX4_HOME_LAT=42.3332322593086\n");
    //QString home_alt_value = QString("export PX4_HOME_ALT=0.0");
    QString faultOptionsfilename;
    QFile faultOptions;

    faultOptionsfilename = option_settings.value("PX4 system path").toString() + "/build/px4_sitl_default/tmp/rootfs/VehicleFaultConfig.ini";
    faultOptions.setFileName(faultOptionsfilename);
    faultOptions.remove();

    //Advances page and alters buttons visibility
    ui->stackedWidget->setCurrentIndex(6);
    ui->pushButton_campaignprogress_goback->setVisible(false);
    ui->pushButton_SFGPSO_generatedgraphs->setVisible(false);
    ui->pushButton_downloadfaulty->setVisible(false);

    //Prepares dronekit and PX4 folders and run files and runs them
    PX4Process->setWorkingDirectory(option_settings.value("PX4 system path").toString());
    MissionProcess->setWorkingDirectory(option_settings.value("Mission plan path").toString());

    runfile_name = option_settings.value("PX4 system path").toString() + "/runme";
    runfile.setFileName(runfile_name);
    if(runfile.open(QIODevice::WriteOnly|QIODevice::Text)){
        qDebug("PX4 runfile Create Success!");
        QTextStream out(&runfile);
        //out << "#!/bin/bash" << home_lat_value << home_lon_value << home_alt_value << world_str;
        out << "#!/bin/bash" << world_str;
        runfile.close();
    }else{
        qDebug("PX4 runfile Create Failed!");
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
        if(!QDir("Campaign Results").exists()){
            QDir().mkdir("Campaign Results");
        }
        if(currentRun == 0){
            //ir buscar o ultimo logfile, criar pasta da campanha e por o logfile na pasta
            QString logfilepath;
            logfilepath = option_settings.value("PX4 system path").toString() + "/build/px4_sitl_default/tmp/rootfs/log";
            QDirIterator it(logfilepath, QDir::AllEntries | QDir::Hidden | QDir::System, QDirIterator::Subdirectories);
            QDateTime tempDate = QDateTime();
            QString tempFilePath, tempFileName;
            while(it.hasNext()){
                QFile f(it.next());
                QFileInfo fileInfo(f);
                if(tempDate.isNull()){
                    tempDate = fileInfo.lastModified();
                    tempFilePath = fileInfo.absoluteFilePath();
                    tempFileName = fileInfo.fileName();
                }
                else if((fileInfo.lastModified() >= tempDate)){
                    tempDate = fileInfo.lastModified();
                    tempFilePath = fileInfo.absoluteFilePath();
                    tempFileName = fileInfo.fileName();
                }
            }
            int i = 0;
            while(QDir(QString("Campaign Results/Campaign Number %1").arg(i)).exists()){
                i++;
            }
            QDir().mkdir(QString("Campaign Results/Campaign Number %1").arg(i));
            currentCampaignFolder = QString("Campaign Results/Campaign Number %1").arg(i);
            QFile::copy(tempFilePath, QString("Campaign Results/" + QString("Campaign Number %1").arg(i) + "/Reference Run.ulg"));
        }else if(currentRun > 0){
            completedRuns++;
        }
    }else if(convertedOutput.contains("Takeoff detected")){
        canAbort = 1;
        if(currentRun == 0){
            runTimer.start();
        }else if(currentRun > 0){
            runTimer.start();
        }
    }else if(convertedOutput.contains("Landing detected")){
        canAbort = 0;
        if(currentRun == 0){
            referenceTime = runTimer.elapsed()/100;
        }else if(currentRun > 0){
            validRuns.insert(currentRun-1, "valid");
        }
    }else{
        if(currentRun > 0){
            if((runTimer.elapsed()/100 > referenceTime + missionDeviation) && canAbort){
                validRuns.insert(currentRun-1, "invalid");
                PX4Process->terminate();
            }
        }
    }
}

void MainWindow::on_pushButton_Title_Start_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_Title_Upload_clicked()
{
    QString tempComponent;
    QVector<QString> tempVector;
    bool ignore_readLine = false;
    QString line;

    QString filter = "Fault Campaign (*.fc)";
    QString filename = QFileDialog::getOpenFileName(this, "Choose fault campaign file", "Fault Campaigns", filter);
    QFile file(filename);

    if (!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Error", "No file chosen");
        return;
    }

    QTextStream in(&file);
    while(!in.atEnd()){
        if(ignore_readLine) ignore_readLine = false;
        else if(!ignore_readLine) line = in.readLine();
        qDebug() << line;

        if(line.contains("System")) chosenSystem = line.section(' ', -1);
        if(line.contains("Mission Plan")) missionPlan = line.section(' ', -1);
        if(line.contains("World chosen")) worldName = line.section(' ', -1);
        if(line.contains("Type of Failure")) chosenInjectionChoice = line.section(' ', -1);
        if(line.contains("Drone model")) droneModel = line.section(' ', -1);
        if(line.contains("Mission Deviation")) missionDeviation = line.section(' ', -1).toDouble();
        if(line.contains("Component")){
            tempComponent = line.section(' ', -1);
            line = in.readLine();
            while(!line.contains("Component") || !in.atEnd()){
                tempVector.append(line.section(' ', 0));
                tempVector.append(line.section(' ', -1));
                line = in.readLine();
            }
            faultsArray[tempComponent] = tempVector;
            ignore_readLine = 1;
        }
    }

    //Show Fault Campaign File data in overview screen
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
    QString campaignFilename, tempComponent, tempComponentValue;
    QFile campaignFile;
    QDir campaignFolder;
    QVector<QString> tempVector;
    QHashIterator<QString, QVector<QString>> hashIterator(faultsArray);

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
        out << "System = " << chosenSystem;
        out << "\nMission Plan = " << missionPlan;
        out << "\nWorld chosen = " << worldName << "\nDrone model = " << droneModel;
        out << "\nMission Deviation = " << QString::number(missionDeviation) << "\n";
        while(hashIterator.hasNext()){
            hashIterator.next();
            tempComponent = hashIterator.key();
            out << QString("Component = ") << tempComponent << "\n";
            tempVector = hashIterator.value();
            tempComponentValue = tempVector[0];
            out << QString("Value type = ") << tempComponentValue << "\n";
            for(int i = 1; i < tempVector.size(); i+=2){
                if(tempVector.at(i) == "") break;
                else out << tempVector.at(i) << " = " << tempVector.at(i+1) << "\n";
            }
        }
        campaignFile.close();
    }else{
        qDebug("Campaign file create failed!");
    }
}

void MainWindow::valueChangedRunNumber(int number){
    numberRuns = number;
}

void MainWindow::PX4Processfinished(){
//changed
    MissionProcess->terminate();
    MissionProcess->waitForFinished();


    if(mustAbort == 0){
        while(mustRunPause == 1){} //wait
        callNextRun();
    }else{
        //end
        ui->textEditGPSRProgress->append("Failure campaign over");
        ui->pushButton_abortCampaign->setVisible(false);
        ui->pushButton_SFGPSP_haltrun->setVisible(false);
        ui->pushButton_campaignprogress_goback->setVisible(true);
        ui->pushButton_SFGPSO_generatedgraphs->setVisible(true);
        ui->pushButton_downloadfaulty->setVisible(true);
    }
}

void MainWindow::callNextRun(){
    QStringList PX4arguments, MissionArguments;
    QString faultOptionsFilename;
    QFile faultOptions;
    QString tempComponent, tempComponentValue;
    QVector<QString> tempVector;
    QHashIterator<QString, QVector<QString>> hashIterator(faultsArray);
    double randomizedValue;

    currentRun++;

    //might have to change in accordance with random aspect
    faultOptionsFilename = option_settings.value("PX4 system path").toString() + "/build/px4_sitl_default/tmp/rootfs/VehicleFaultConfig.ini";
    faultOptions.setFileName(faultOptionsFilename);
    if(faultOptions.open(QIODevice::WriteOnly|QIODevice::Text)){
        QTextStream outF(&faultOptions);
        outF << QString("Mission Deviation = ") << QString::number(missionDeviation) << "\n";
        while(hashIterator.hasNext()){
            hashIterator.next();
            tempComponent = hashIterator.key();
            outF << QString("Component = ") << tempComponent << "\n";
            if(tempComponent == "GPS Failure"){
                tempVector = hashIterator.value();
                tempComponentValue = tempVector[0];
                if(tempComponentValue == "Random Value"){
                    outF << QString("Value type = Fixed Value") << "\n";
                    //randomize latitude
                    if(!std::isnan(tempVector[2].toDouble()) && !std::isnan(tempVector[4].toDouble())){
                        randomizedValue = tempVector[2].toDouble() + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(tempVector[4].toDouble()-tempVector[2].toDouble())));
                        outF << QString("GPS Lat = %1\n").arg(randomizedValue);
                    }else outF << QString("GPS Lat = nan\n");
                    //randomize longitude
                    if(!std::isnan(tempVector[6].toDouble()) && !std::isnan(tempVector[8].toDouble())){
                        randomizedValue = tempVector[6].toDouble() + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(tempVector[8].toDouble()-tempVector[6].toDouble())));
                        outF << QString("GPS Lon = %1\n").arg(randomizedValue);
                    }else outF << QString("GPS Lon = nan\n");
                    //randomize altitude
                    if(!std::isnan(tempVector[10].toDouble()) && !std::isnan(tempVector[12].toDouble())){
                        randomizedValue = tempVector[10].toDouble() + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(tempVector[12].toDouble()-tempVector[10].toDouble())));
                        outF << QString("GPS Alt = %1\n").arg(randomizedValue);
                    }else outF << QString("GPS Alt = nan\n");
                    outF << tempVector.at(13) << " = " << tempVector.at(14) << "\n";
                    outF << tempVector.at(15) << " = " << tempVector.at(16) << "\n";                    
                }
                if(tempComponentValue == "Min|Max Value"){
                    outF << QString("Value type = Fixed Value") << "\n";
                    if(tempVector[2].toDouble() == 1){
                        randomizedValue = rand() % 2;
                        if(randomizedValue == 1) outF << QString("GPS Lat = %1\n").arg(std::numeric_limits<double>::max());
                        else outF << QString("GPS Lat = %1\n").arg(std::numeric_limits<double>::min());
                    }else outF << QString("GPS Lat = nan\n");
                    if(tempVector[4].toDouble() == 1){
                        randomizedValue = rand() % 2;
                        if(randomizedValue == 1) outF << QString("GPS Lon = %1\n").arg(std::numeric_limits<double>::max());
                        else outF << QString("GPS Lon = %1\n").arg(std::numeric_limits<double>::min());
                    }else outF << QString("GPS Lon = nan\n");
                    if(tempVector[6].toDouble() == 1){
                        randomizedValue = rand() % 2;
                        if(randomizedValue == 1) outF << QString("GPS Alt = %1\n").arg(std::numeric_limits<double>::max());
                        else outF << QString("GPS Alt = %1\n").arg(std::numeric_limits<double>::min());
                    }else outF << QString("GPS Alt = nan\n");
                    outF << tempVector.at(7) << " = " << tempVector.at(8) << "\n";
                    outF << tempVector.at(9) << " = " << tempVector.at(10) << "\n";
                }
                else{
                    outF << QString("Value type = ") << tempComponentValue << "\n";
                    for(int i = 1; i < tempVector.size(); i+=2){
                        if(tempVector.at(i) == "") break;
                        else outF << tempVector.at(i) << " = " << tempVector.at(i+1) << "\n";
                    }
                }
            }else if(tempComponent == "GPS Attack"){
                tempVector = hashIterator.value();
                tempComponentValue = tempVector[0];
                if(tempComponentValue == "Random Position"){
                    outF << QString("Value type = Fixed Value") << "\n";
                    //randomize latitude
                    if(!std::isnan(tempVector[2].toDouble()) && !std::isnan(tempVector[4].toDouble())){
                        randomizedValue = tempVector[2].toDouble() + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(tempVector[4].toDouble()-tempVector[2].toDouble())));
                        outF << QString("GPS Lat = %1\n").arg(randomizedValue);
                    }else outF << QString("GPS Lat = nan\n");
                    //randomize longitude
                    if(!std::isnan(tempVector[6].toDouble()) && !std::isnan(tempVector[8].toDouble())){
                        randomizedValue = tempVector[6].toDouble() + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(tempVector[8].toDouble()-tempVector[6].toDouble())));
                        outF << QString("GPS Lon = %1\n").arg(randomizedValue);
                    }else outF << QString("GPS Lon = nan\n");
                    //randomize altitude
                    if(!std::isnan(tempVector[10].toDouble()) && !std::isnan(tempVector[12].toDouble())){
                        randomizedValue = tempVector[10].toDouble() + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(tempVector[12].toDouble()-tempVector[10].toDouble())));
                        outF << QString("GPS Alt = %1\n").arg(randomizedValue);
                    }else outF << QString("GPS Alt = nan\n");
                    outF << tempVector.at(13) << " = " << tempVector.at(14) << "\n";
                    outF << tempVector.at(15) << " = " << tempVector.at(16) << "\n";
                }else if(tempComponentValue == "Hijack with attacker's position"){
                    outF << QString("Value type = Fixed Value") << "\n";
                    for(int i = 1; i < tempVector.size(); i+=2){
                        if(tempVector.at(i) == "") break;
                        else outF << tempVector.at(i) << " = " << tempVector.at(i+1) << "\n";
                    }
                }else if(tempComponentValue == "Hijack with a second UAV"){
                    outF << QString("Value type = Fixed Value") << "\n";
                    for(int i = 1; i < tempVector.size(); i+=2){
                        if(tempVector.at(i) == "") break;
                        else outF << tempVector.at(i) << " = " << tempVector.at(i+1) << "\n";
                    }
                }else if(tempComponentValue == "GPS Delay"){
                    outF << QString("Value type = Delay Value") << "\n";
                    for(int i = 1; i < tempVector.size(); i+=2){
                        if(tempVector.at(i) == "") break;
                        else outF << tempVector.at(i) << " = " << tempVector.at(i+1) << "\n";
                    }
                }else if(tempComponentValue == "Random Latitude"){
                    outF << QString("Value type = Fixed Value") << "\n";
                    //randomize latitude
                    if(!std::isnan(tempVector[2].toDouble()) && !std::isnan(tempVector[4].toDouble())){
                        randomizedValue = tempVector[2].toDouble() + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(tempVector[4].toDouble()-tempVector[2].toDouble())));
                        outF << QString("GPS Lat = %1\n").arg(randomizedValue);
                    }else outF << QString("GPS Lat = nan\n");
                    //randomize longitude
                    if(!std::isnan(tempVector[6].toDouble()) && !std::isnan(tempVector[8].toDouble())){
                        randomizedValue = tempVector[6].toDouble() + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(tempVector[8].toDouble()-tempVector[6].toDouble())));
                        outF << QString("GPS Lon = %1\n").arg(randomizedValue);
                    }else outF << QString("GPS Lon = nan\n");
                    //randomize altitude
                    if(!std::isnan(tempVector[10].toDouble()) && !std::isnan(tempVector[12].toDouble())){
                        randomizedValue = tempVector[10].toDouble() + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(tempVector[12].toDouble()-tempVector[10].toDouble())));
                        outF << QString("GPS Alt = %1\n").arg(randomizedValue);
                    }else outF << QString("GPS Alt = nan\n");
                    outF << tempVector.at(13) << " = " << tempVector.at(14) << "\n";
                    outF << tempVector.at(15) << " = " << tempVector.at(16) << "\n";
                }else if(tempComponentValue == "Random Longitude"){
                    outF << QString("Value type = Fixed Value") << "\n";
                    //randomize latitude
                    if(!std::isnan(tempVector[2].toDouble()) && !std::isnan(tempVector[4].toDouble())){
                        randomizedValue = tempVector[2].toDouble() + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(tempVector[4].toDouble()-tempVector[2].toDouble())));
                        outF << QString("GPS Lat = %1\n").arg(randomizedValue);
                    }else outF << QString("GPS Lat = nan\n");
                    //randomize longitude
                    if(!std::isnan(tempVector[6].toDouble()) && !std::isnan(tempVector[8].toDouble())){
                        randomizedValue = tempVector[6].toDouble() + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(tempVector[8].toDouble()-tempVector[6].toDouble())));
                        outF << QString("GPS Lon = %1\n").arg(randomizedValue);
                    }else outF << QString("GPS Lon = nan\n");
                    //randomize altitude
                    if(!std::isnan(tempVector[10].toDouble()) && !std::isnan(tempVector[12].toDouble())){
                        randomizedValue = tempVector[10].toDouble() + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(tempVector[12].toDouble()-tempVector[10].toDouble())));
                        outF << QString("GPS Alt = %1\n").arg(randomizedValue);
                    }else outF << QString("GPS Alt = nan\n");
                    outF << tempVector.at(13) << " = " << tempVector.at(14) << "\n";
                    outF << tempVector.at(15) << " = " << tempVector.at(16) << "\n";
                }else if(tempComponentValue == "Force UAV Landing"){
                    outF << QString("Value type = Fixed Value") << "\n";
                    for(int i = 1; i < tempVector.size(); i+=2){
                        if(tempVector.at(i) == "") break;
                        else outF << tempVector.at(i) << " = " << tempVector.at(i+1) << "\n";
                    }
                }
            }
        }
        faultOptions.close();
    }else{
        qDebug("Fault options Create Failed!");
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
        mustAbort = 1;
        ui->textEditGPSRProgress->append("Failure campaign over");
        ui->pushButton_abortCampaign->setVisible(false);
        ui->pushButton_SFGPSP_haltrun->setVisible(false);
        ui->pushButton_campaignprogress_goback->setVisible(true);
        ui->pushButton_SFGPSO_generatedgraphs->setVisible(true);
        ui->pushButton_downloadfaulty->setVisible(true);
    }
}

void MainWindow::textChangedFaultInjection_1(QString text)
{
    if(text == "Software Failures"){
        ui->comboBox_faultinjection_2->clear();
        ui->comboBox_faultinjection_3->clear();
        ui->pushButton_setinputvalues->setVisible(false);
        ui->comboBox_faultinjection_2->addItem("--------");
        ui->comboBox_faultinjection_2->addItem("GPS Failure");
        ui->comboBox_faultinjection_2->setCurrentIndex(0);
    }
    if(text == "Communication Failures"){
        ui->comboBox_faultinjection_2->clear();
        ui->comboBox_faultinjection_3->clear();
        ui->pushButton_setinputvalues->setVisible(false);
    }
    if(text == "AI Failures"){
        ui->comboBox_faultinjection_2->clear();
        ui->comboBox_faultinjection_3->clear();
        ui->pushButton_setinputvalues->setVisible(false);
    }
    if(text == "Security Attack"){
        ui->comboBox_faultinjection_2->clear();
        ui->comboBox_faultinjection_3->clear();
        ui->pushButton_setinputvalues->setVisible(false);
        ui->comboBox_faultinjection_2->addItem("--------");
        ui->comboBox_faultinjection_2->addItem("GPS Attack");
        ui->comboBox_faultinjection_2->setCurrentIndex(0);
    }else if(text == "---------------"){
        ui->comboBox_faultinjection_2->clear();
        ui->comboBox_faultinjection_3->clear();
        ui->pushButton_setinputvalues->setVisible(false);
    }
}

void MainWindow::textChangedFaultInjection_2(QString text)
{
    if(text == "GPS Failure"){
       chosenInjectionChoice = "gps";
       ui->comboBox_faultinjection_3->clear();
       ui->pushButton_setinputvalues->setVisible(false);
       ui->comboBox_faultinjection_3->addItem("--------");
       ui->comboBox_faultinjection_3->addItem("Freeze Value");
       ui->comboBox_faultinjection_3->addItem("Fixed Value");
       ui->comboBox_faultinjection_3->addItem("Delay Value");
       ui->comboBox_faultinjection_3->addItem("Random Value");
       ui->comboBox_faultinjection_3->addItem("Min|Max Value");
       ui->comboBox_faultinjection_3->setCurrentIndex(0);
    }
    if(text == "GPS Attack"){
        chosenInjectionChoice = "gps attack";
        ui->comboBox_faultinjection_3->clear();
        ui->pushButton_setinputvalues->setVisible(false);
        ui->comboBox_faultinjection_3->addItem("--------");
        ui->comboBox_faultinjection_3->addItem("Random Longitude");
        ui->comboBox_faultinjection_3->addItem("Random Latitude");
        ui->comboBox_faultinjection_3->addItem("Random Position");
        ui->comboBox_faultinjection_3->addItem("GPS Delay");
        ui->comboBox_faultinjection_3->addItem("Force UAV Landing");
        ui->comboBox_faultinjection_3->addItem("Hijack with a second UAV");
        ui->comboBox_faultinjection_3->addItem("Hijack with attacker's position");
        ui->comboBox_faultinjection_3->setCurrentIndex(0);
    }else if(text == "--------"){
        ui->comboBox_faultinjection_3->clear();
        ui->pushButton_setinputvalues->setVisible(false);
    }
}

void MainWindow::textChangedFaultInjection_3(QString text)
{
    if(text == "Fixed Value"){
        ui->pushButton_setinputvalues->setVisible(true);
        typeWindow = QString("GPS Fixed Value");
        emit(emitWindowType(typeWindow));
    }else if(text == "Freeze Value"){
        ui->pushButton_setinputvalues->setVisible(true);
        typeWindow = QString("GPS Freeze Value");
        emit(emitWindowType(typeWindow));
    }else if(text == "Delay Value"){
        ui->pushButton_setinputvalues->setVisible(true);
        typeWindow = QString("GPS Delay Value");
        emit(emitWindowType(typeWindow));
    }else if(text == "Random Value"){
        ui->pushButton_setinputvalues->setVisible(true);
        typeWindow = QString("GPS Random Value");
        emit(emitWindowType(typeWindow));
    }else if(text == "Min|Max Value"){
        ui->pushButton_setinputvalues->setVisible(true);
        typeWindow = QString("GPS Min|Max Value");
        emit(emitWindowType(typeWindow));
    }else if(text == "Random Longitude"){
        ui->pushButton_setinputvalues->setVisible(true);
        typeWindow = QString("Random Longitude");
        emit(emitWindowType(typeWindow));
    }else if(text == "Random Latitude"){
        ui->pushButton_setinputvalues->setVisible(true);
        typeWindow = QString("Random Latitude");
        emit(emitWindowType(typeWindow));
    }else if(text == "Random Position"){
        ui->pushButton_setinputvalues->setVisible(true);
        typeWindow = QString("Random Position");
        emit(emitWindowType(typeWindow));
    }else if(text == "GPS Delay"){
        ui->pushButton_setinputvalues->setVisible(true);
        typeWindow = QString("GPS Delay");
        emit(emitWindowType(typeWindow));
    }else if(text == "Force UAV Landing"){
        ui->pushButton_setinputvalues->setVisible(true);
        typeWindow = QString("Force UAV Landing");
        emit(emitWindowType(typeWindow));
    }else if(text == "Hijack with a second UAV"){
        ui->pushButton_setinputvalues->setVisible(true);
        typeWindow = QString("Hijack with a second UAV");
        emit(emitWindowType(typeWindow));
    }else if(text == "Hijack with attacker's position"){
        ui->pushButton_setinputvalues->setVisible(true);
        typeWindow = QString("Hijack with attacker's position");
        emit(emitWindowType(typeWindow));
    }else if(text == "--------"){
        ui->pushButton_setinputvalues->setVisible(false);
    }
}


void MainWindow::on_pushButton_setinputvalues_clicked()
{
    if(ui->comboBox_faultinjection_3->currentIndex() != 0){
        newWindow->show();        
    }
}

void MainWindow::on_injectionContinueButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
    ui->textEditOverview->append("Working on a PX4 architecture");
    for(int i = 0; i < failuresCampaign->size(); ++i){
        ui->textEditOverview->append(failuresCampaign->at(i));
    }
    ui->textEditOverview->append("Using mission plan " + missionPlan + " with a deviation of " + QString::number(missionDeviation) + " seconds");
    ui->textEditOverview->append("Using a " + droneModel + " drone");
}

void MainWindow::WindowClosedReactionGPSFixed(double GPSLatR, double GPSLonR, double GPSAltR, double startTimeR, double endTimeR){
    GPSLat = GPSLatR;
    GPSLon = GPSLonR;
    GPSAlt = GPSAltR;

    if(std::isnan(GPSLat) && (std::isnan(GPSLon)) && (!std::isnan(GPSAlt)))  failuresCampaign->append(QString("GPS Fixed Value: GPS Altitude - %1\n").arg(GPSAlt));
    else if(std::isnan(GPSLat) && (!std::isnan(GPSLon)) && (std::isnan(GPSAlt)))     failuresCampaign->append(QString("GPS Fixed Value: GPS Longitude - %1\n").arg(GPSLon));
    else if(!std::isnan(GPSLat) && (std::isnan(GPSLon)) && (std::isnan(GPSAlt)))     failuresCampaign->append(QString("GPS Fixed Value: GPS Latitude - %1\n").arg(GPSLat));
    else if(!std::isnan(GPSLat) && (!std::isnan(GPSLon)) && (std::isnan(GPSAlt)))     failuresCampaign->append(QString("GPS Fixed Value: GPS Latitude - %1 | GPS Longitude - %2\n").arg(GPSLat).arg(GPSLon));
    else if(!std::isnan(GPSLat) && (std::isnan(GPSLon)) && (!std::isnan(GPSAlt)))     failuresCampaign->append(QString("GPS Fixed Value: GPS Latitude - %1 | GPS Altitude - %2\n").arg(GPSLat).arg(GPSAlt));
    else if(std::isnan(GPSLat) && (!std::isnan(GPSLon)) && (!std::isnan(GPSAlt)))     failuresCampaign->append(QString("GPS Fixed Value: GPS Longitude - %1 | GPS Altitude - %2\n").arg(GPSLon).arg(GPSAlt));
    else if(!std::isnan(GPSLat) && (!std::isnan(GPSLon)) && (!std::isnan(GPSAlt)))    failuresCampaign->append(QString("GPS Fixed Value: GPS Latitude - %1 | GPS Longitude - %2 | GPS Altitude - %3\n").arg(GPSLat).arg(GPSLon).arg(GPSAlt));

    QString faultComponent = ui->comboBox_faultinjection_2->currentText();
    QVector<QString> correspondentVector(11, "");
    correspondentVector[0] = ui->comboBox_faultinjection_3->currentText();
    correspondentVector[1] = "GPS Lat"; correspondentVector[2] = QString::number(GPSLatR);
    correspondentVector[3] = "GPS Lon"; correspondentVector[4] = QString::number(GPSLonR);
    correspondentVector[5] = "GPS Alt"; correspondentVector[6] = QString::number(GPSAltR);
    correspondentVector[7] = "Start Time"; correspondentVector[8] = QString::number(startTimeR);
    correspondentVector[9] = "End Time"; correspondentVector[10] = QString::number(endTimeR);
    faultsArray[faultComponent] = correspondentVector;
    qDebug() << faultsArray[faultComponent];
    ui->injectionContinueButton->setVisible(true);
}

void MainWindow::WindowClosedReactionSecurityForceLand(double GPSLatR, double GPSLonR, double GPSAltR, double startTimeR, double endTimeR){
    GPSLat = GPSLatR;
    GPSLon = GPSLonR;
    GPSAlt = GPSAltR;

    if(std::isnan(GPSLat) && (std::isnan(GPSLon)) && (!std::isnan(GPSAlt)))  failuresCampaign->append(QString("Security Attack Force Landing: GPS Altitude - %1\n").arg(GPSAlt));
    else if(std::isnan(GPSLat) && (!std::isnan(GPSLon)) && (std::isnan(GPSAlt)))     failuresCampaign->append(QString("Security Attack Force Landing: GPS Longitude - %1\n").arg(GPSLon));
    else if(!std::isnan(GPSLat) && (std::isnan(GPSLon)) && (std::isnan(GPSAlt)))     failuresCampaign->append(QString("Security Attack Force Landing: GPS Latitude - %1\n").arg(GPSLat));
    else if(!std::isnan(GPSLat) && (!std::isnan(GPSLon)) && (std::isnan(GPSAlt)))     failuresCampaign->append(QString("Security Attack Force Landing: GPS Latitude - %1 | GPS Longitude - %2\n").arg(GPSLat).arg(GPSLon));
    else if(!std::isnan(GPSLat) && (std::isnan(GPSLon)) && (!std::isnan(GPSAlt)))     failuresCampaign->append(QString("Security Attack Force Landing: GPS Latitude - %1 | GPS Altitude - %2\n").arg(GPSLat).arg(GPSAlt));
    else if(std::isnan(GPSLat) && (!std::isnan(GPSLon)) && (!std::isnan(GPSAlt)))     failuresCampaign->append(QString("Security Attack Force Landing: GPS Longitude - %1 | GPS Altitude - %2\n").arg(GPSLon).arg(GPSAlt));
    else if(!std::isnan(GPSLat) && (!std::isnan(GPSLon)) && (!std::isnan(GPSAlt)))    failuresCampaign->append(QString("Security Attack Force Landing: GPS Latitude - %1 | GPS Longitude - %2 | GPS Altitude - %3\n").arg(GPSLat).arg(GPSLon).arg(GPSAlt));

    QString faultComponent = ui->comboBox_faultinjection_2->currentText();
    QVector<QString> correspondentVector(11, "");
    correspondentVector[0] = ui->comboBox_faultinjection_3->currentText();
    correspondentVector[1] = "GPS Lat"; correspondentVector[2] = QString::number(GPSLatR);
    correspondentVector[3] = "GPS Lon"; correspondentVector[4] = QString::number(GPSLonR);
    correspondentVector[5] = "GPS Alt"; correspondentVector[6] = QString::number(GPSAltR);
    correspondentVector[7] = "Start Time"; correspondentVector[8] = QString::number(startTimeR);
    correspondentVector[9] = "End Time"; correspondentVector[10] = QString::number(endTimeR);
    faultsArray[faultComponent] = correspondentVector;
    qDebug() << faultsArray[faultComponent];
    ui->injectionContinueButton->setVisible(true);
}

void MainWindow::WindowClosedReactionSecurityPosition(double GPSLatR, double GPSLonR, double GPSAltR, double startTimeR, double endTimeR){
    GPSLat = GPSLatR;
    GPSLon = GPSLonR;
    GPSAlt = GPSAltR;

    if(std::isnan(GPSLat) && (std::isnan(GPSLon)) && (!std::isnan(GPSAlt)))  failuresCampaign->append(QString("Security Attack Hacker Position: GPS Altitude - %1\n").arg(GPSAlt));
    else if(std::isnan(GPSLat) && (!std::isnan(GPSLon)) && (std::isnan(GPSAlt)))     failuresCampaign->append(QString("Security Attack Hacker Position: GPS Longitude - %1\n").arg(GPSLon));
    else if(!std::isnan(GPSLat) && (std::isnan(GPSLon)) && (std::isnan(GPSAlt)))     failuresCampaign->append(QString("Security Attack Hacker Position: GPS Latitude - %1\n").arg(GPSLat));
    else if(!std::isnan(GPSLat) && (!std::isnan(GPSLon)) && (std::isnan(GPSAlt)))     failuresCampaign->append(QString("Security Attack Hacker Position: GPS Latitude - %1 | GPS Longitude - %2\n").arg(GPSLat).arg(GPSLon));
    else if(!std::isnan(GPSLat) && (std::isnan(GPSLon)) && (!std::isnan(GPSAlt)))     failuresCampaign->append(QString("Security Attack Hacker Position: GPS Latitude - %1 | GPS Altitude - %2\n").arg(GPSLat).arg(GPSAlt));
    else if(std::isnan(GPSLat) && (!std::isnan(GPSLon)) && (!std::isnan(GPSAlt)))     failuresCampaign->append(QString("Security Attack Hacker Position: GPS Longitude - %1 | GPS Altitude - %2\n").arg(GPSLon).arg(GPSAlt));
    else if(!std::isnan(GPSLat) && (!std::isnan(GPSLon)) && (!std::isnan(GPSAlt)))    failuresCampaign->append(QString("Security Attack Hacker Position: GPS Latitude - %1 | GPS Longitude - %2 | GPS Altitude - %3\n").arg(GPSLat).arg(GPSLon).arg(GPSAlt));

    QString faultComponent = ui->comboBox_faultinjection_2->currentText();
    QVector<QString> correspondentVector(11, "");
    correspondentVector[0] = ui->comboBox_faultinjection_3->currentText();
    correspondentVector[1] = "GPS Lat"; correspondentVector[2] = QString::number(GPSLatR);
    correspondentVector[3] = "GPS Lon"; correspondentVector[4] = QString::number(GPSLonR);
    correspondentVector[5] = "GPS Alt"; correspondentVector[6] = QString::number(GPSAltR);
    correspondentVector[7] = "Start Time"; correspondentVector[8] = QString::number(startTimeR);
    correspondentVector[9] = "End Time"; correspondentVector[10] = QString::number(endTimeR);
    faultsArray[faultComponent] = correspondentVector;
    qDebug() << faultsArray[faultComponent];
    ui->injectionContinueButton->setVisible(true);
}

void MainWindow::WindowClosedReactionSecurityUAVPosition(double GPSLatR, double GPSLonR, double GPSAltR, double startTimeR, double endTimeR){
    GPSLat = GPSLatR;
    GPSLon = GPSLonR;
    GPSAlt = GPSAltR;

    if(std::isnan(GPSLat) && (std::isnan(GPSLon)) && (!std::isnan(GPSAlt)))  failuresCampaign->append(QString("Security Attack UAV Position: GPS Altitude - %1\n").arg(GPSAlt));
    else if(std::isnan(GPSLat) && (!std::isnan(GPSLon)) && (std::isnan(GPSAlt)))     failuresCampaign->append(QString("Security Attack UAV Position: GPS Longitude - %1\n").arg(GPSLon));
    else if(!std::isnan(GPSLat) && (std::isnan(GPSLon)) && (std::isnan(GPSAlt)))     failuresCampaign->append(QString("Security Attack UAV Position: GPS Latitude - %1\n").arg(GPSLat));
    else if(!std::isnan(GPSLat) && (!std::isnan(GPSLon)) && (std::isnan(GPSAlt)))     failuresCampaign->append(QString("Security Attack UAV Position: GPS Latitude - %1 | GPS Longitude - %2\n").arg(GPSLat).arg(GPSLon));
    else if(!std::isnan(GPSLat) && (std::isnan(GPSLon)) && (!std::isnan(GPSAlt)))     failuresCampaign->append(QString("Security Attack UAV Position: GPS Latitude - %1 | GPS Altitude - %2\n").arg(GPSLat).arg(GPSAlt));
    else if(std::isnan(GPSLat) && (!std::isnan(GPSLon)) && (!std::isnan(GPSAlt)))     failuresCampaign->append(QString("Security Attack UAV Position: GPS Longitude - %1 | GPS Altitude - %2\n").arg(GPSLon).arg(GPSAlt));
    else if(!std::isnan(GPSLat) && (!std::isnan(GPSLon)) && (!std::isnan(GPSAlt)))    failuresCampaign->append(QString("Security Attack UAV Position: GPS Latitude - %1 | GPS Longitude - %2 | GPS Altitude - %3\n").arg(GPSLat).arg(GPSLon).arg(GPSAlt));

    QString faultComponent = ui->comboBox_faultinjection_2->currentText();
    QVector<QString> correspondentVector(11, "");
    correspondentVector[0] = ui->comboBox_faultinjection_3->currentText();
    correspondentVector[1] = "GPS Lat"; correspondentVector[2] = QString::number(GPSLatR);
    correspondentVector[3] = "GPS Lon"; correspondentVector[4] = QString::number(GPSLonR);
    correspondentVector[5] = "GPS Alt"; correspondentVector[6] = QString::number(GPSAltR);
    correspondentVector[7] = "Start Time"; correspondentVector[8] = QString::number(startTimeR);
    correspondentVector[9] = "End Time"; correspondentVector[10] = QString::number(endTimeR);
    faultsArray[faultComponent] = correspondentVector;
    qDebug() << faultsArray[faultComponent];
    ui->injectionContinueButton->setVisible(true);
}

void MainWindow::WindowClosedReactionGPSFreeze(double startTime, double endTime){
    failuresCampaign->append(QString("GPS Freeze Value: Start Time - %1 | End Time - %2\n").arg(startTime).arg(endTime));
    QString faultComponent = ui->comboBox_faultinjection_2->currentText();
    QVector<QString> correspondentVector(5, "");
    correspondentVector[0] = ui->comboBox_faultinjection_3->currentText();
    correspondentVector[1] = "Start Time"; correspondentVector[2] = QString::number(startTime);
    correspondentVector[3] = "End Time"; correspondentVector[4] = QString::number(endTime);
    faultsArray[faultComponent] = correspondentVector;
    qDebug() << faultsArray[faultComponent];
    ui->injectionContinueButton->setVisible(true);
}

void MainWindow::WindowClosedReactionGPSDelay(double delay, double startTime, double endTime){
    failuresCampaign->append(QString("GPS Delay Value: Delay - %1 | Start Time - %2 | End Time - %3\n").arg(delay).arg(startTime).arg(endTime));
    QString faultComponent = ui->comboBox_faultinjection_2->currentText();
    QVector<QString> correspondentVector(7, "");
    correspondentVector[0] = ui->comboBox_faultinjection_3->currentText();
    correspondentVector[1] = "Delay Value"; correspondentVector[2] = QString::number(delay);
    correspondentVector[3] = "Start Time"; correspondentVector[4] = QString::number(startTime);
    correspondentVector[5] = "End Time"; correspondentVector[6] = QString::number(endTime);
    faultsArray[faultComponent] = correspondentVector;
    qDebug() << faultsArray[faultComponent];
    ui->injectionContinueButton->setVisible(true);
}

void MainWindow::WindowClosedReactionSecurityAttackDelay(double delay, double startTime, double endTime){
    failuresCampaign->append(QString("Security Attack GPS Delay: Delay - %1 | Start Time - %2 | End Time - %3\n").arg(delay).arg(startTime).arg(endTime));
    QString faultComponent = ui->comboBox_faultinjection_2->currentText();
    QVector<QString> correspondentVector(7, "");
    correspondentVector[0] = ui->comboBox_faultinjection_3->currentText();
    correspondentVector[1] = "Delay Value"; correspondentVector[2] = QString::number(delay);
    correspondentVector[3] = "Start Time"; correspondentVector[4] = QString::number(startTime);
    correspondentVector[5] = "End Time"; correspondentVector[6] = QString::number(endTime);
    faultsArray[faultComponent] = correspondentVector;
    qDebug() << faultsArray[faultComponent];
    ui->injectionContinueButton->setVisible(true);
}

void MainWindow::WindowClosedReactionGPSRandom(double GPSLatMin, double GPSLatMax, double GPSLonMin, double GPSLonMax, double GPSAltMin, double GPSAltMax, double startTime, double endTime){
    if(!std::isnan(GPSLatMin) && !std::isnan(GPSLatMax) && !std::isnan(GPSLonMin) && !std::isnan(GPSLonMax) && !std::isnan(GPSAltMin) && !std::isnan(GPSAltMax)) failuresCampaign->append(QString("GPS Random Value: GPS Latitude Interval - [%1,%2] | GPS Longitude Interval - [%3,%4] | GPS Altitude Interval - [%5,%6] | Start Time - %7 | End Time - %8\n").arg(GPSLatMin).arg(GPSLatMax).arg(GPSLonMin).arg(GPSLonMax).arg(GPSAltMin).arg(GPSAltMax).arg(startTime).arg(endTime));
    else if(!std::isnan(GPSLatMin) && !std::isnan(GPSLatMax) && (std::isnan(GPSLonMin) || std::isnan(GPSLonMax)) && (std::isnan(GPSAltMin) || std::isnan(GPSAltMax))) failuresCampaign->append(QString("GPS Random Value: GPS Latitude Interval - [%1,%2] | Start Time - %3 | End Time - %4\n").arg(GPSLatMin).arg(GPSLatMax).arg(startTime).arg(endTime));
    else if((std::isnan(GPSLatMin) || std::isnan(GPSLatMax)) && !std::isnan(GPSLonMin) && !std::isnan(GPSLonMax) && (std::isnan(GPSAltMin) || std::isnan(GPSAltMax))) failuresCampaign->append(QString("GPS Random Value: GPS Longitude Interval - [%1,%2] | Start Time - %3 | End Time - %4\n").arg(GPSLonMin).arg(GPSLonMax).arg(startTime).arg(endTime));
    else if((std::isnan(GPSLatMin) || std::isnan(GPSLatMax)) && (std::isnan(GPSLonMin) || std::isnan(GPSLonMax)) && !std::isnan(GPSAltMin) && !std::isnan(GPSAltMax)) failuresCampaign->append(QString("GPS Random Value: GPS Altitude Interval - [%1,%2] | Start Time - %3 | End Time - %4\n").arg(GPSAltMin).arg(GPSAltMax).arg(startTime).arg(endTime));
    else if(!std::isnan(GPSLatMin) && !std::isnan(GPSLatMax) && !std::isnan(GPSLonMin) && !std::isnan(GPSLonMax) && (std::isnan(GPSAltMin) || std::isnan(GPSAltMax))) failuresCampaign->append(QString("GPS Random Value: GPS Latitude Interval - [%1,%2] | GPS Longitude Interval - [%3,%4] | Start Time - %5 | End Time - %6\n").arg(GPSLatMin).arg(GPSLatMax).arg(GPSLonMin).arg(GPSLonMax).arg(startTime).arg(endTime));
    else if(!std::isnan(GPSLatMin) && !std::isnan(GPSLatMax) && (std::isnan(GPSLonMin) || std::isnan(GPSLonMax)) && !std::isnan(GPSAltMin) && !std::isnan(GPSAltMax)) failuresCampaign->append(QString("GPS Random Value: GPS Latitude Interval - [%1,%2] | GPS Altitude Interval - [%3,%4] | Start Time - %5 | End Time - %6\n").arg(GPSLatMin).arg(GPSLatMax).arg(GPSAltMin).arg(GPSAltMax).arg(startTime).arg(endTime));
    else if((std::isnan(GPSLatMin) || std::isnan(GPSLatMax)) && !std::isnan(GPSLonMin) && !std::isnan(GPSLonMax) && !std::isnan(GPSAltMin) && !std::isnan(GPSAltMax)) failuresCampaign->append(QString("GPS Random Value: GPS Longitude Interval - [%1,%2] | GPS Altitude Interval - [%3,%4] | Start Time - %5 | End Time - %6\n").arg(GPSLonMin).arg(GPSLonMax).arg(GPSAltMin).arg(GPSAltMax).arg(startTime).arg(endTime));

    QString faultComponent = ui->comboBox_faultinjection_2->currentText();
    QVector<QString> correspondentVector(17, "");
    correspondentVector[0] = ui->comboBox_faultinjection_3->currentText();
    correspondentVector[1] = "GPS Lat Min"; correspondentVector[2] = QString::number(GPSLatMin);
    correspondentVector[3] = "GPS Lat Max"; correspondentVector[4] = QString::number(GPSLatMax);
    correspondentVector[5] = "GPS Lon Min"; correspondentVector[6] = QString::number(GPSLonMin);
    correspondentVector[7] = "GPS Lon Max"; correspondentVector[8] = QString::number(GPSLonMax);
    correspondentVector[9] = "GPS Alt Min"; correspondentVector[10] = QString::number(GPSAltMin);
    correspondentVector[11] = "GPS Alt Max"; correspondentVector[12] = QString::number(GPSAltMax);
    correspondentVector[13] = "Start Time"; correspondentVector[14] = QString::number(startTime);
    correspondentVector[15] = "End Time"; correspondentVector[16] = QString::number(endTime);
    faultsArray[faultComponent] = correspondentVector;
    qDebug() << faultsArray[faultComponent];
    ui->injectionContinueButton->setVisible(true);
}

void MainWindow::WindowClosedSecurityAttackRandomReaction(double GPSLatMin, double GPSLatMax, double GPSLonMin, double GPSLonMax, double GPSAltMin, double GPSAltMax, double startTime, double endTime){
    if(!std::isnan(GPSLatMin) && !std::isnan(GPSLatMax) && !std::isnan(GPSLonMin) && !std::isnan(GPSLonMax) && !std::isnan(GPSAltMin) && !std::isnan(GPSAltMax)) failuresCampaign->append(QString("Security Attack Random Position: GPS Latitude Interval - [%1,%2] | GPS Longitude Interval - [%3,%4] | GPS Altitude Interval - [%5,%6] | Start Time - %7 | End Time - %8\n").arg(GPSLatMin).arg(GPSLatMax).arg(GPSLonMin).arg(GPSLonMax).arg(GPSAltMin).arg(GPSAltMax).arg(startTime).arg(endTime));
    else if(!std::isnan(GPSLatMin) && !std::isnan(GPSLatMax) && (std::isnan(GPSLonMin) || std::isnan(GPSLonMax)) && (std::isnan(GPSAltMin) || std::isnan(GPSAltMax))) failuresCampaign->append(QString("Security Attack Random Position: GPS Latitude Interval - [%1,%2] | Start Time - %3 | End Time - %4\n").arg(GPSLatMin).arg(GPSLatMax).arg(startTime).arg(endTime));
    else if((std::isnan(GPSLatMin) || std::isnan(GPSLatMax)) && !std::isnan(GPSLonMin) && !std::isnan(GPSLonMax) && (std::isnan(GPSAltMin) || std::isnan(GPSAltMax))) failuresCampaign->append(QString("Security Attack Random Position: GPS Longitude Interval - [%1,%2] | Start Time - %3 | End Time - %4\n").arg(GPSLonMin).arg(GPSLonMax).arg(startTime).arg(endTime));
    else if((std::isnan(GPSLatMin) || std::isnan(GPSLatMax)) && (std::isnan(GPSLonMin) || std::isnan(GPSLonMax)) && !std::isnan(GPSAltMin) && !std::isnan(GPSAltMax)) failuresCampaign->append(QString("Security Attack Random Position: GPS Altitude Interval - [%1,%2] | Start Time - %3 | End Time - %4\n").arg(GPSAltMin).arg(GPSAltMax).arg(startTime).arg(endTime));
    else if(!std::isnan(GPSLatMin) && !std::isnan(GPSLatMax) && !std::isnan(GPSLonMin) && !std::isnan(GPSLonMax) && (std::isnan(GPSAltMin) || std::isnan(GPSAltMax))) failuresCampaign->append(QString("Security Attack Random Position: GPS Latitude Interval - [%1,%2] | GPS Longitude Interval - [%3,%4] | Start Time - %5 | End Time - %6\n").arg(GPSLatMin).arg(GPSLatMax).arg(GPSLonMin).arg(GPSLonMax).arg(startTime).arg(endTime));
    else if(!std::isnan(GPSLatMin) && !std::isnan(GPSLatMax) && (std::isnan(GPSLonMin) || std::isnan(GPSLonMax)) && !std::isnan(GPSAltMin) && !std::isnan(GPSAltMax)) failuresCampaign->append(QString("Security Attack Random Position: GPS Latitude Interval - [%1,%2] | GPS Altitude Interval - [%3,%4] | Start Time - %5 | End Time - %6\n").arg(GPSLatMin).arg(GPSLatMax).arg(GPSAltMin).arg(GPSAltMax).arg(startTime).arg(endTime));
    else if((std::isnan(GPSLatMin) || std::isnan(GPSLatMax)) && !std::isnan(GPSLonMin) && !std::isnan(GPSLonMax) && !std::isnan(GPSAltMin) && !std::isnan(GPSAltMax)) failuresCampaign->append(QString("Security Attack Random Position: GPS Longitude Interval - [%1,%2] | GPS Altitude Interval - [%3,%4] | Start Time - %5 | End Time - %6\n").arg(GPSLonMin).arg(GPSLonMax).arg(GPSAltMin).arg(GPSAltMax).arg(startTime).arg(endTime));

    QString faultComponent = ui->comboBox_faultinjection_2->currentText();
    QVector<QString> correspondentVector(17, "");
    correspondentVector[0] = ui->comboBox_faultinjection_3->currentText();
    correspondentVector[1] = "GPS Lat Min"; correspondentVector[2] = QString::number(GPSLatMin);
    correspondentVector[3] = "GPS Lat Max"; correspondentVector[4] = QString::number(GPSLatMax);
    correspondentVector[5] = "GPS Lon Min"; correspondentVector[6] = QString::number(GPSLonMin);
    correspondentVector[7] = "GPS Lon Max"; correspondentVector[8] = QString::number(GPSLonMax);
    correspondentVector[9] = "GPS Alt Min"; correspondentVector[10] = QString::number(GPSAltMin);
    correspondentVector[11] = "GPS Alt Max"; correspondentVector[12] = QString::number(GPSAltMax);
    correspondentVector[13] = "Start Time"; correspondentVector[14] = QString::number(startTime);
    correspondentVector[15] = "End Time"; correspondentVector[16] = QString::number(endTime);
    faultsArray[faultComponent] = correspondentVector;
    qDebug() << faultsArray[faultComponent];
    ui->injectionContinueButton->setVisible(true);
}

void MainWindow::WindowClosedSecurityAttackRandomLatReaction(double GPSLatMin, double GPSLatMax, double GPSLonMin, double GPSLonMax, double GPSAltMin, double GPSAltMax, double startTime, double endTime){
    if(!std::isnan(GPSLatMin) && !std::isnan(GPSLatMax) && !std::isnan(GPSLonMin) && !std::isnan(GPSLonMax) && !std::isnan(GPSAltMin) && !std::isnan(GPSAltMax)) failuresCampaign->append(QString("Security Attack Random Latitude: GPS Latitude Interval - [%1,%2] | GPS Longitude Interval - [%3,%4] | GPS Altitude Interval - [%5,%6] | Start Time - %7 | End Time - %8\n").arg(GPSLatMin).arg(GPSLatMax).arg(GPSLonMin).arg(GPSLonMax).arg(GPSAltMin).arg(GPSAltMax).arg(startTime).arg(endTime));
    else if(!std::isnan(GPSLatMin) && !std::isnan(GPSLatMax) && (std::isnan(GPSLonMin) || std::isnan(GPSLonMax)) && (std::isnan(GPSAltMin) || std::isnan(GPSAltMax))) failuresCampaign->append(QString("Security Attack Random Latitude: GPS Latitude Interval - [%1,%2] | Start Time - %3 | End Time - %4\n").arg(GPSLatMin).arg(GPSLatMax).arg(startTime).arg(endTime));
    else if((std::isnan(GPSLatMin) || std::isnan(GPSLatMax)) && !std::isnan(GPSLonMin) && !std::isnan(GPSLonMax) && (std::isnan(GPSAltMin) || std::isnan(GPSAltMax))) failuresCampaign->append(QString("Security Attack Random Latitude: GPS Longitude Interval - [%1,%2] | Start Time - %3 | End Time - %4\n").arg(GPSLonMin).arg(GPSLonMax).arg(startTime).arg(endTime));
    else if((std::isnan(GPSLatMin) || std::isnan(GPSLatMax)) && (std::isnan(GPSLonMin) || std::isnan(GPSLonMax)) && !std::isnan(GPSAltMin) && !std::isnan(GPSAltMax)) failuresCampaign->append(QString("Security Attack Random Latitude: GPS Altitude Interval - [%1,%2] | Start Time - %3 | End Time - %4\n").arg(GPSAltMin).arg(GPSAltMax).arg(startTime).arg(endTime));
    else if(!std::isnan(GPSLatMin) && !std::isnan(GPSLatMax) && !std::isnan(GPSLonMin) && !std::isnan(GPSLonMax) && (std::isnan(GPSAltMin) || std::isnan(GPSAltMax))) failuresCampaign->append(QString("Security Attack Random Latitude: GPS Latitude Interval - [%1,%2] | GPS Longitude Interval - [%3,%4] | Start Time - %5 | End Time - %6\n").arg(GPSLatMin).arg(GPSLatMax).arg(GPSLonMin).arg(GPSLonMax).arg(startTime).arg(endTime));
    else if(!std::isnan(GPSLatMin) && !std::isnan(GPSLatMax) && (std::isnan(GPSLonMin) || std::isnan(GPSLonMax)) && !std::isnan(GPSAltMin) && !std::isnan(GPSAltMax)) failuresCampaign->append(QString("Security Attack Random Latitude: GPS Latitude Interval - [%1,%2] | GPS Altitude Interval - [%3,%4] | Start Time - %5 | End Time - %6\n").arg(GPSLatMin).arg(GPSLatMax).arg(GPSAltMin).arg(GPSAltMax).arg(startTime).arg(endTime));
    else if((std::isnan(GPSLatMin) || std::isnan(GPSLatMax)) && !std::isnan(GPSLonMin) && !std::isnan(GPSLonMax) && !std::isnan(GPSAltMin) && !std::isnan(GPSAltMax)) failuresCampaign->append(QString("Security Attack Random Latitude: GPS Longitude Interval - [%1,%2] | GPS Altitude Interval - [%3,%4] | Start Time - %5 | End Time - %6\n").arg(GPSLonMin).arg(GPSLonMax).arg(GPSAltMin).arg(GPSAltMax).arg(startTime).arg(endTime));

    QString faultComponent = ui->comboBox_faultinjection_2->currentText();
    QVector<QString> correspondentVector(17, "");
    correspondentVector[0] = ui->comboBox_faultinjection_3->currentText();
    correspondentVector[1] = "GPS Lat Min"; correspondentVector[2] = QString::number(GPSLatMin);
    correspondentVector[3] = "GPS Lat Max"; correspondentVector[4] = QString::number(GPSLatMax);
    correspondentVector[5] = "GPS Lon Min"; correspondentVector[6] = QString::number(GPSLonMin);
    correspondentVector[7] = "GPS Lon Max"; correspondentVector[8] = QString::number(GPSLonMax);
    correspondentVector[9] = "GPS Alt Min"; correspondentVector[10] = QString::number(GPSAltMin);
    correspondentVector[11] = "GPS Alt Max"; correspondentVector[12] = QString::number(GPSAltMax);
    correspondentVector[13] = "Start Time"; correspondentVector[14] = QString::number(startTime);
    correspondentVector[15] = "End Time"; correspondentVector[16] = QString::number(endTime);
    faultsArray[faultComponent] = correspondentVector;
    qDebug() << faultsArray[faultComponent];
    ui->injectionContinueButton->setVisible(true);
}

void MainWindow::WindowClosedSecurityAttackRandomLonReaction(double GPSLatMin, double GPSLatMax, double GPSLonMin, double GPSLonMax, double GPSAltMin, double GPSAltMax, double startTime, double endTime){
    if(!std::isnan(GPSLatMin) && !std::isnan(GPSLatMax) && !std::isnan(GPSLonMin) && !std::isnan(GPSLonMax) && !std::isnan(GPSAltMin) && !std::isnan(GPSAltMax)) failuresCampaign->append(QString("Security Attack Random Longitude: GPS Latitude Interval - [%1,%2] | GPS Longitude Interval - [%3,%4] | GPS Altitude Interval - [%5,%6] | Start Time - %7 | End Time - %8\n").arg(GPSLatMin).arg(GPSLatMax).arg(GPSLonMin).arg(GPSLonMax).arg(GPSAltMin).arg(GPSAltMax).arg(startTime).arg(endTime));
    else if(!std::isnan(GPSLatMin) && !std::isnan(GPSLatMax) && (std::isnan(GPSLonMin) || std::isnan(GPSLonMax)) && (std::isnan(GPSAltMin) || std::isnan(GPSAltMax))) failuresCampaign->append(QString("Security Attack Random Longitude: GPS Latitude Interval - [%1,%2] | Start Time - %3 | End Time - %4\n").arg(GPSLatMin).arg(GPSLatMax).arg(startTime).arg(endTime));
    else if((std::isnan(GPSLatMin) || std::isnan(GPSLatMax)) && !std::isnan(GPSLonMin) && !std::isnan(GPSLonMax) && (std::isnan(GPSAltMin) || std::isnan(GPSAltMax))) failuresCampaign->append(QString("Security Attack Random Longitude: GPS Longitude Interval - [%1,%2] | Start Time - %3 | End Time - %4\n").arg(GPSLonMin).arg(GPSLonMax).arg(startTime).arg(endTime));
    else if((std::isnan(GPSLatMin) || std::isnan(GPSLatMax)) && (std::isnan(GPSLonMin) || std::isnan(GPSLonMax)) && !std::isnan(GPSAltMin) && !std::isnan(GPSAltMax)) failuresCampaign->append(QString("Security Attack Random Longitude: GPS Altitude Interval - [%1,%2] | Start Time - %3 | End Time - %4\n").arg(GPSAltMin).arg(GPSAltMax).arg(startTime).arg(endTime));
    else if(!std::isnan(GPSLatMin) && !std::isnan(GPSLatMax) && !std::isnan(GPSLonMin) && !std::isnan(GPSLonMax) && (std::isnan(GPSAltMin) || std::isnan(GPSAltMax))) failuresCampaign->append(QString("Security Attack Random Longitude: GPS Latitude Interval - [%1,%2] | GPS Longitude Interval - [%3,%4] | Start Time - %5 | End Time - %6\n").arg(GPSLatMin).arg(GPSLatMax).arg(GPSLonMin).arg(GPSLonMax).arg(startTime).arg(endTime));
    else if(!std::isnan(GPSLatMin) && !std::isnan(GPSLatMax) && (std::isnan(GPSLonMin) || std::isnan(GPSLonMax)) && !std::isnan(GPSAltMin) && !std::isnan(GPSAltMax)) failuresCampaign->append(QString("Security Attack Random Longitude: GPS Latitude Interval - [%1,%2] | GPS Altitude Interval - [%3,%4] | Start Time - %5 | End Time - %6\n").arg(GPSLatMin).arg(GPSLatMax).arg(GPSAltMin).arg(GPSAltMax).arg(startTime).arg(endTime));
    else if((std::isnan(GPSLatMin) || std::isnan(GPSLatMax)) && !std::isnan(GPSLonMin) && !std::isnan(GPSLonMax) && !std::isnan(GPSAltMin) && !std::isnan(GPSAltMax)) failuresCampaign->append(QString("Security Attack Random Longitude: GPS Longitude Interval - [%1,%2] | GPS Altitude Interval - [%3,%4] | Start Time - %5 | End Time - %6\n").arg(GPSLonMin).arg(GPSLonMax).arg(GPSAltMin).arg(GPSAltMax).arg(startTime).arg(endTime));

    QString faultComponent = ui->comboBox_faultinjection_2->currentText();
    QVector<QString> correspondentVector(17, "");
    correspondentVector[0] = ui->comboBox_faultinjection_3->currentText();
    correspondentVector[1] = "GPS Lat Min"; correspondentVector[2] = QString::number(GPSLatMin);
    correspondentVector[3] = "GPS Lat Max"; correspondentVector[4] = QString::number(GPSLatMax);
    correspondentVector[5] = "GPS Lon Min"; correspondentVector[6] = QString::number(GPSLonMin);
    correspondentVector[7] = "GPS Lon Max"; correspondentVector[8] = QString::number(GPSLonMax);
    correspondentVector[9] = "GPS Alt Min"; correspondentVector[10] = QString::number(GPSAltMin);
    correspondentVector[11] = "GPS Alt Max"; correspondentVector[12] = QString::number(GPSAltMax);
    correspondentVector[13] = "Start Time"; correspondentVector[14] = QString::number(startTime);
    correspondentVector[15] = "End Time"; correspondentVector[16] = QString::number(endTime);
    faultsArray[faultComponent] = correspondentVector;
    qDebug() << faultsArray[faultComponent];
    ui->injectionContinueButton->setVisible(true);
}

void MainWindow::WindowClosedReactionGPSMinMax(double GPSLat, double GPSLon, double GPSAlt, double startTime, double endTime){
    if(GPSLat == 1 && GPSLon == 1 && GPSAlt == 1) failuresCampaign->append(QString("GPS Min Max Value: GPSLat - True | GPSLon - True | GPSAlt - True | Start Time - %1 | End Time - %2\n").arg(startTime).arg(endTime));
    else if(GPSLat == 1 && GPSLon == 0 && GPSAlt == 0) failuresCampaign->append(QString("GPS Min Max Value: GPSLat - True | Start Time - %1 | End Time - %2\n").arg(startTime).arg(endTime));
    else if(GPSLat == 0 && GPSLon == 1 && GPSAlt == 0) failuresCampaign->append(QString("GPS Min Max Value: GPSLon - True | Start Time - %1 | End Time - %2\n").arg(startTime).arg(endTime));
    else if(GPSLat == 0 && GPSLon == 0 && GPSAlt == 1) failuresCampaign->append(QString("GPS Min Max Value: GPSAlt - True | Start Time - %1 | End Time - %2\n").arg(startTime).arg(endTime));
    else if(GPSLat == 1 && GPSLon == 1 && GPSAlt == 0) failuresCampaign->append(QString("GPS Min Max Value: GPSLat - True | GPSLon - True | Start Time - %1 | End Time - %2\n").arg(startTime).arg(endTime));
    else if(GPSLat == 1 && GPSLon == 0 && GPSAlt == 1) failuresCampaign->append(QString("GPS Min Max Value: GPSLat - True | GPSAlt - True | Start Time - %1 | End Time - %2\n").arg(startTime).arg(endTime));
    else if(GPSLat == 0 && GPSLon == 1 && GPSAlt == 1) failuresCampaign->append(QString("GPS Min Max Value: GPSLon - True | GPSAlt - True | Start Time - %1 | End Time - %2\n").arg(startTime).arg(endTime));

    QString faultComponent = ui->comboBox_faultinjection_2->currentText();
    QVector<QString> correspondentVector(11, "");
    correspondentVector[0] = ui->comboBox_faultinjection_3->currentText();
    correspondentVector[1] = "GPS Lat"; correspondentVector[2] = QString::number(GPSLat);
    correspondentVector[3] = "GPS Lon"; correspondentVector[4] = QString::number(GPSLon);
    correspondentVector[5] = "GPS Alt"; correspondentVector[6] = QString::number(GPSAlt);
    correspondentVector[7] = "Start Time"; correspondentVector[8] = QString::number(startTime);
    correspondentVector[9] = "End Time"; correspondentVector[10] = QString::number(endTime);
    faultsArray[faultComponent] = correspondentVector;
    qDebug() << faultsArray[faultComponent];
    ui->injectionContinueButton->setVisible(true);
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

void MainWindow::on_pushButton_campaignprogress_goback_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_abortCampaign_clicked()
{
    currentRun = numberRuns + 1;
    mustAbort = 1;
    ui->textEditGPSRProgress->append("Aborting campaign");
    MissionProcess->terminate();
    PX4Process->terminate();
}

void MainWindow::on_pushButton_downloadfaulty_clicked()
{
    if(!QDir("Campaign Results").exists()){
        QDir().mkdir("Campaign Results");
    }
    //ir buscar o ultimo logfile, criar pasta da campanha e por o logfile na pasta
    QString logfilepath, validity;
    logfilepath = option_settings.value("PX4 system path").toString() + "/build/px4_sitl_default/tmp/rootfs/log";
    QDirIterator it(logfilepath, QDir::AllEntries | QDir::Hidden | QDir::System, QDirIterator::Subdirectories);
    QDateTime tempDate = QDateTime();
    QString tempFilePath, tempFileName;
    int i = 1;
    while(i <= numberRuns){
        qDebug() << QString("%1 iteration on save logfiles");
        while(it.hasNext()){
            QFile f(it.next());
            QFileInfo fileInfo(f);
            if(tempDate.isNull()){
                tempDate = fileInfo.lastModified();
                tempFilePath = fileInfo.absoluteFilePath();
                tempFileName = fileInfo.fileName();
            }
            else if((fileInfo.lastModified() >= tempDate)){
                tempDate = fileInfo.lastModified();
                tempFilePath = fileInfo.absoluteFilePath();
                tempFileName = fileInfo.fileName();
            }
        }
        if(validRuns.at(numberRuns - i) == "valid"){
            validity = "t";
        }else if(validRuns.at(numberRuns - i) == "invalid") validity = "f";
        QFile::copy(tempFilePath, QString(currentCampaignFolder + "/" + QString("Fault File %1%2.ulg").arg(numberRuns + 1 - i).arg(validity)));
        QFile::remove(tempFilePath);
        i++;
    }
}

//Allow user to change GPS Failsafe delay duration, must be between 1 and 100 seconds
void MainWindow::on_actionFailsafe_Detection_triggered()
{
    QString gpsFailsafeConfigPath;
    if(!option_settings.contains("PX4 system path")){
        QMessageBox::warning(this, "Error", "Define PX4 flight controller path in options.");
        return;
    }

    gpsFailsafeConfigPath = option_settings.value("PX4 system path").toString() + "/src/modules/commander/commander_params.c";
    bool ok;
    int GPSfailsafeValue = QInputDialog::getInt(this, tr("Input a value for GPS Failsafe Delay duration"), tr("Delay value (s):"), 1, 1, 100, 1, &ok);
    if(ok){
        QFile file(gpsFailsafeConfigPath);
        file.open(QIODevice::Text | QIODevice::ReadOnly);
        QString fileText = file.readAll();

        int i = 1;
        while(!fileText.contains(QString("COM_POS_FS_DELAY, %1)").arg(i))){
            i++;
        }

        QRegularExpression re(QString("COM_POS_FS_DELAY, %1").arg(i));
        QString replacementText(QString("COM_POS_FS_DELAY, %1").arg(GPSfailsafeValue));

        fileText.replace(re, replacementText);

        QFile newFile(option_settings.value("PX4 system path").toString() + "/src/modules/commander/commander_params.c");
        if(newFile.open(QFile::WriteOnly | QFile::Truncate)){
            QTextStream out(&newFile);
            out<< fileText;
        }
        newFile.close();
    }
}

//Allow user to change drone max speed during mission, must be between 3 to 20 m/s
void MainWindow::on_actionDrone_speed_on_mission_triggered()
{
    QString droneSpeedConfigPath;
    if(!option_settings.contains("PX4 system path")){
        QMessageBox::warning(this, "Error", "Define PX4 flight controller path in options.");
        return;
    }

    droneSpeedConfigPath = option_settings.value("PX4 system path").toString() + "/src/modules/mc_pos_control/mc_pos_control_params.c";
    bool ok;
    double droneSpeedValue = QInputDialog::getDouble(this, tr("Input drone max airspeed"), tr("Speed value (m/s):"), 5.0, 3.0, 20.0, 2, &ok, Qt::WindowFlags(), 1);

    if(ok){
        QFile file(droneSpeedConfigPath);
        file.open(QIODevice::Text | QIODevice::ReadOnly);
        QString fileText = file.readAll();

        double i = 3.00;
        while(!fileText.contains(QString("MPC_XY_CRUISE, %1f").arg(i))){
            i = i + 0.01;
        }

        QRegularExpression re(QString("MPC_XY_CRUISE, %1f").arg(i));
        QString replacementText(QString("MPC_XY_CRUISE, %1f").arg(droneSpeedValue));

        fileText.replace(re, replacementText);

        QFile newFile(option_settings.value("PX4 system path").toString() + "/src/modules/mc_pos_control/mc_pos_control_params.c");
        if(newFile.open(QFile::WriteOnly | QFile::Truncate)){
            QTextStream out(&newFile);
            out<< fileText;
        }
        newFile.close();
    }
}
