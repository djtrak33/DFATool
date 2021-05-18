#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ctime>
#include <cstdlib>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <cmath>
#include <limits>
#include <QDebug>
#include <QProcess>
#include <QCloseEvent>
#include "secondarywindow.h"

extern QVariant px4_system_path;
extern QVariant mission_plan_path;
extern QSettings option_settings;

void options_config();

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    SecondaryWindow *newWindow = new SecondaryWindow(0);
    ~MainWindow();

private slots:

    void on_pushButton_clicked();

    void on_buttonMissionPlanChoice_clicked();

    void on_buttonMissionDetailsContinue_clicked();

    void textChangedDrone(QString text);

    void on_buttonEnvironmentDetailsContinue_clicked();

    void on_actionPX4_System_path_triggered();

    void on_actionMissions_folder_path_triggered();

    void on_pushButton_SFGPSO_confirm_clicked();

    void on_pushButton_SFGPSO_goback_clicked();

    void on_buttonWorldChoice_clicked();

    void on_pushButton_TOF_goback_clicked();

    void readOutputMissionP();

    void readOutputPX4P();

    void on_pushButton_SFGPSP_haltrun_clicked();

    void on_pushButton_Title_Start_clicked();

    void on_pushButton_SFGPSMD_goback_clicked();

    void on_pushButton_SFGPSED_goback_clicked();

    void on_pushButton_SFGPSO_savecampaign_clicked();

    void on_pushButton_Title_Upload_clicked();

    void valueChangedRunNumber(int number);

    void PX4Processfinished();

    void textChangedFaultInjection_1(QString text);

    void textChangedFaultInjection_2(QString text);

    void textChangedFaultInjection_3(QString text);

    void on_pushButton_setinputvalues_clicked();

    void on_injectionContinueButton_clicked();

    void WindowClosedReaction(double arg1, double arg2, double arg3);

    void on_pushButton_results_goback_clicked();

    void on_pushButton_SFGPSO_generatedgraphs_clicked();

    void on_pushButton_definedcampaign_clicked();

    void on_doubleSpinBox_allowedDeviation_valueChanged(double arg1);

    void on_doubleSpinBox_startInjection_valueChanged(double arg1);

    void on_doubleSpinBox_endInjection_valueChanged(double arg1);

    void on_pushButton_campaignprogress_goback_clicked();

    void on_pushButton_abortCampaign_clicked();

    void callNextRun();

private:
    Ui::MainWindow *ui;
    QProcess *PX4Process = new QProcess();
    QProcess *MissionProcess = new QProcess();

    QStringList *failuresCampaign = new QStringList();

    bool mustRunPause = 0, worldChosen = 0, missionChosen = 0;
    int numberRuns = 1, currentRun = 0;
    QString chosenSystem, lastPressedFailure, chosenInjectionChoice;
    QString missionPlan, droneModel = "iris", worldName;

    double GPSLon = std::numeric_limits<double>::quiet_NaN(), GPSLat = std::numeric_limits<double>::quiet_NaN(), GPSAlt = std::numeric_limits<double>::quiet_NaN();
    double missionDeviation = std::numeric_limits<double>::quiet_NaN(), startInjectionTime = std::numeric_limits<double>::quiet_NaN(), endInjectionTime = std::numeric_limits<double>::quiet_NaN();

};
#endif // MAINWINDOW_H
