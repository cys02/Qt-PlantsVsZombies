//
// Created by sun on 8/25/16.
//

#ifndef PLANTS_VS_ZOMBIES_MAINVIEW_H
#define PLANTS_VS_ZOMBIES_MAINVIEW_H

#include <QtWidgets>
#include "Timer.h"

class SelectorScene;
class GameScene;
class MainWindow;

class MainView: public QGraphicsView
{
    Q_OBJECT

public:
    MainView(MainWindow *mainWindow);
    ~MainView();

    QString getUsername() const;
    void setUsername(const QString &username);
    MainWindow *getMainWindow() const;

    void switchToMenuScene();                       //选择菜单界面
    void switchToGameScene(const QString &eName);   //选择游戏界面
    void pauseGameScene();      //暂停游戏场景
    void endGameScene();        //结束游戏场景

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    const int width, height;
    const QString usernameSettingEntry;

    SelectorScene *selectorScene;
    GameScene *gameScene;           //gameScene是GameScene的指针
    MainWindow *mainWindow;
};

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    QAction *getFullScreenAction() const;
private:
    const QString fullScreenSettingEntry;

    QGraphicsView *mainView;
    QAction *fullScreenAction;
};

extern MainView *gMainView;

#endif //PLANTS_VS_ZOMBIES_MAINVIEW_H
