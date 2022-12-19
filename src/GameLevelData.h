//
// Created by sun on 8/26/16.
//

#ifndef PLANTS_VS_ZOMBIES_GAMELEVELDATA_H
#define PLANTS_VS_ZOMBIES_GAMELEVELDATA_H

#include <QtCore>

class GameScene;        //游戏场景

class ZombieData
{
public:
    QString eName;
    int num;
    int firstFlag;      //出现波数
    QList<int> flagList;
};

class GameLevelData
{
    Q_DECLARE_TR_FUNCTIONS(GameLevelData)
public:
    GameLevelData();
    virtual ~GameLevelData() {}

    QString eName, cName;

    QList<QString> pName, zName;
    int cardKind;           // 0: plant     1: zombies
    int dKind;
    int sunNum;

    QString backgroundImage;
    QList<int> LF;
    bool canSelectCard, staticCard, showScroll, produceSun, hasShovel;  //可以选择卡片，静态卡片，显示卷轴，产生太阳，有铲子
    int maxSelectedCards;       //最多可选卡片
    int coord;                  //坐标

    int flagNum;
    QList<int> largeWaveFlag;
    QPair<QList<int>, QList<int> > flagToSumNum;
    QMap<int, std::function<void(GameScene *)> > flagToMonitor;

    QList<ZombieData> zombieData;

    QString backgroundMusic;

    virtual void loadAccess(GameScene *gameScene);
    virtual void initLawnMower(GameScene *gameScene);
    virtual void startGame(GameScene *gameScene);
    virtual void endGame(GameScene *gameScene);
};

class GameLevelData_1: public GameLevelData
{
    Q_DECLARE_TR_FUNCTIONS(GameLevelData_1)
public:
    GameLevelData_1();
};

class GameLevelData_2: public GameLevelData
{
    Q_DECLARE_TR_FUNCTIONS(GameLevelData_2)
public:
    GameLevelData_2();
};

class GameLevelData_3: public GameLevelData
{
    Q_DECLARE_TR_FUNCTIONS(GameLevelData_3)
public:
    GameLevelData_3();
};

class GameLevelData_4: public GameLevelData
{
    Q_DECLARE_TR_FUNCTIONS(GameLevelData_4)
public:
    GameLevelData_4();
};

class GameLevelData_5: public GameLevelData
{
    Q_DECLARE_TR_FUNCTIONS(GameLevelData_5)
public:
    GameLevelData_5();
};

class GameLevelData_6: public GameLevelData
{
    Q_DECLARE_TR_FUNCTIONS(GameLevelData_6)
public:
    GameLevelData_6();
};
GameLevelData * GameLevelDataFactory(const QString &eName);

#endif //PLANTS_VS_ZOMBIES_GAMELEVEL_H
