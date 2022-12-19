#ifndef CHOOSELEVEL_H
#define CHOOSELEVEL_H

#include<QWidget>   //Qt自带用户界面原子
#include<QPushButton>   //Qt自带按钮原子
#include<QIcon>     //程序图标
#include<QLabel>

class ChooseLevel:public QWidget
{
    //将信号和槽函数联系起来的宏
    Q_OBJECT
public:

    ChooseLevel(QWidget *parent=nullptr);   //构造函数
    //构造一个作为父级的子级的小部件
    //parent 为 nullptr，则新小部件将成为一个窗口

    ~ChooseLevel();                         //析构

    //绘图函数
    void paintEvent(QPaintEvent *event);

public slots:                   //定义connect槽函数
    void ChooseLevel1();              //关卡触发函数（槽函数）
    void ChooseLevel2();
    void ChooseLevel3();
    void ChooseLevel4();

private:
    QPushButton *Level1;        //定义按键
    QPushButton *Level2;
    QPushButton *Level3;
    QPushButton *Level4;



//关闭时发出信号
signals:
    void closeEmit();

};



#endif // CHOOSELEVEL_H
