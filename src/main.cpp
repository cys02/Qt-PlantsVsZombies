//
// Created by sun on 8/25/16.
//

#include <QtCore>
#include <QtWidgets>
#include "MainView.h"
#include "ImageManager.h"

int main(int argc, char * *argv)        //初始应用程序
{
    QApplication app(argc, argv);
    // For QSettings
    QCoreApplication::setOrganizationName("Sun Ziping");    //已经构造了QSetting
    QCoreApplication::setOrganizationDomain("sunziping.com");
    QCoreApplication::setApplicationName("Plants vs Zombies");
    // For Translators
    QTranslator appTranslator;      //翻译

    // TODO: change translation back after debugging
    appTranslator.load(QString(":/translations/main.%1.qm").arg(QLocale::system().name()));
    //appTranslator.load(QString(":/translations/main.%1.qm").arg("zh_CN"));
    app.installTranslator(&appTranslator);

    InitImageManager();//图片加载器部署


    qsrand((uint) QTime::currentTime().msec()); //播种
    MainWindow mainWindow;

    gMainView->switchToMenuScene();     //初始界面

    mainWindow.show();

    int res = app.exec();   //进入事件循环
    DestoryImageManager();  //析构
    return res;
}


