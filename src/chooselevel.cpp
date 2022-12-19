#include "ChooseLevel.h"
#include "SelectorScene.h"
#include "MainView.h"
#include "ImageManager.h"

ChooseLevel::ChooseLevel(QWidget *parent):QWidget (parent){
    //显示界面
    this->setAutoFillBackground(true);
    //设置界面大小
    this->resize(300,200);
    //设置界面标题
    this->setWindowTitle("Please Choose Your Level");


    //设置按键
    //初始化按键对象
    Level1 = new  QPushButton(this);
    //设置关卡的名字
    Level1->setText(QString("关卡1-1"));
    //设置按键的位置和大小：四个参数分别为x，y，w，h
    Level1->setGeometry(50,30,100,50);
    //
    Level1->setFlat(true);
    //connect：第一个参数是一个对象，第二个参数是一个信号，第三个参数是该信号触发的槽函数
    //实现点击按钮触发Level1函数
    connect(Level1,SIGNAL(clicked()),this,SLOT(ChooseLevel1()));

    Level2 = new QPushButton(this);
    Level2->setText(QString("关卡1-2"));
    Level2->setGeometry(150,30,100,50);
    Level2->setFlat(true);
    connect(Level2,SIGNAL(clicked()),this,SLOT(ChooseLevel2()));

    Level3 = new QPushButton(this);
    Level3->setText(QString("关卡1-3"));
    Level3->setGeometry(50,100,100,50);
    Level3->setFlat(true);
    connect(Level3,SIGNAL(clicked()),this,SLOT(ChooseLevel3()));

    Level4 = new QPushButton(this);
    Level4->setText(QString("关卡1-4"));
    Level4->setGeometry(150,100,100,50);
    Level4->setFlat(true);
    connect(Level4,SIGNAL(clicked()),this,SLOT(ChooseLevel4()));
}

ChooseLevel::~ChooseLevel(){

}


//QPainte绘图
void ChooseLevel::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.drawPixmap(rect(),QPixmap(gImageCache->load("interface/Almanac_ZombieBack.jpg")),QRect());
}


void ChooseLevel::ChooseLevel1(){
    qDebug()<<"game 1";
    //打开关卡
    //gMainView->switchToGameScene(QSettings().value("Global/NextLevel","1").toString());
    gMainView->switchToGameScene("1");
    //关闭该界面（选择关卡的界面）
    this->close();
}

void ChooseLevel::ChooseLevel2(){
    qDebug()<<"game 2";
    //打开关卡
    //gMainView->switchToGameScene(QSettings().value("Global/NextLevel","2").toString());
    gMainView->switchToGameScene("2");
    //关闭该界面（选择关卡的界面）
    this->close();
}

void ChooseLevel::ChooseLevel3(){
    qDebug()<<"game 3";
    //打开关卡
    //gMainView->switchToGameScene(QSettings().value("Global/NextLevel","3").toString());
    gMainView->switchToGameScene("3");
    //关闭该界面（选择关卡的界面）
    this->close();
}


void ChooseLevel::ChooseLevel4(){
    qDebug()<<"game 4";
    //打开关卡
    //gMainView->switchToGameScene(QSettings().value("Global/NextLevel","4").toString());
    gMainView->switchToGameScene("4");
    //关闭该界面（选择关卡的界面）
    this->close();
}


