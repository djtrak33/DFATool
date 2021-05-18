#include "mainwindow.h"

#include <QApplication>
#include <QFile>

QVariant px4_system_path;
QVariant mission_plan_path;
QSettings option_settings("University of Coimbra", "Drone Fault Injector");


int main(int argc, char *argv[]){
    srand(static_cast <unsigned> (time(0)));
    options_config();
    QApplication a(argc, argv);

    //Set app style sheet
    QFile styleSheetFile(":/Stylesheets/images/Clocker.qss");
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    a.setStyleSheet(styleSheet);

    MainWindow w;
    w.show();
    return a.exec();

}

//if its first time boot then replace files from px4 with replacer
void options_config(){
     QFile file;
     //ver se o setting ja existe
     if(option_settings.contains("Mission plan path")){
         mission_plan_path = option_settings.value("Mission plan path");
     }
     if(option_settings.contains("PX4 system path")){
         px4_system_path = option_settings.value("PX4 system path");
     }
     if(!QDir("Fault Campaigns").exists()){
         QDir().mkdir("Fault Campaigns");
     }
     if(!QDir("Campaign Results").exists()){
         QDir().mkdir("Campaign Results");
     }
}

