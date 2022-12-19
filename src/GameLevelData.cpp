//
// Created by sun on 8/26/16.
//

#include "GameLevelData.h"
#include "GameScene.h"
#include "ImageManager.h"
#include "Timer.h"

GameLevelData::GameLevelData() : cardKind(0),
                                 dKind(1),
                                 sunNum(50),
                                 backgroundImage("interface/background1.jpg"),
                                 LF{ 0, 1, 1, 1, 1, 1 },
                                 canSelectCard(true),
                                 staticCard(true),
                                 showScroll(true),
                                 produceSun(true),
                                 hasShovel(true),            //铲子
                                 maxSelectedCards(10),       //最多装的卡
                                 coord(0),
                                 flagNum(0)
{}

void  GameLevelData::loadAccess(GameScene *gameScene)
{
    gameScene->loadAcessFinished();
}

void GameLevelData::startGame(GameScene *gameScene)
{
    initLawnMower(gameScene);
    gameScene->prepareGrowPlants( [gameScene] {
        gameScene->beginBGM();
        gameScene->beginMonitor();
        gameScene->beginCool();
        gameScene->beginSun(25);
        (new Timer(gameScene, 3000/*15000*/, [gameScene] {
            gameScene->beginZombies();
        }))->start();
    });
}

void GameLevelData::initLawnMower(GameScene *gameScene)
{
    for (int i = 0; i < LF.size(); ++i) {
        if (LF[i] == 1)
            gameScene->customSpecial("oLawnCleaner", -1, i);//oLawnCleaner
        else if (LF[i] == 2)
            gameScene->customSpecial("oPoolCleaner", -1, i);
    }
}

void GameLevelData::endGame(GameScene *gameScene)
{

}


GameLevelData_1::GameLevelData_1()
{
    qDebug()<<"enter game 1";
    backgroundImage = "interface/background1.jpg";
    backgroundMusic = "qrc:/audio/UraniwaNi.mp3";
    sunNum = 100;
    canSelectCard = true;
    showScroll = true;
    eName = "1";
    cName = tr("Level 1-1");
    pName = { "oPeashooter", "oSnowPea","oRepeater", "oSunflower","oPotatoMine", "oWallNut","oTorchwood","oCherryBomb","oGatlingPea","oTallNut"};
    zombieData = { { "oZombie3", 1, 1, {} }, { "oConeheadZombie", 1, 3, {} }, { "oBucketheadZombie", 1, 3, {} }
                   ,{"oFootballZombie",5,3,{}},{"oPoleVaultingZombie",5,3,{}},{"oScreenDoorZombie",5,3,{}} };
    //参数：ename、num、firstflag、flaglist
    //名字、展示个数、第一次出现的波数
    //firstflag：当前波数大于firstFlag时，当前波数僵尸才会出现
    flagNum = 10;               //一共十个回合
    largeWaveFlag = { 10 };     //举旗僵尸出现的回合数
    flagToSumNum = QPair<QList<int>, QList<int> >({ 5, 7, 9 }, { 1, 2, 3, 10 });    //和GameSence相联系
    //waveNum表示波数   函数qLowerBound返回的是flagToSumNum.first元素的位置——0、1、2、3，1到3回合返回0，4到6返回1，6到9返回2，10返回3
    //这里返回值对应2中的位置
    //修改后1到5回合返回1，6到7回合返回2，8到9回合返回3，10回合返回4
    //flagToSumNum.second中的数越大，levelSum越大，levelSum减小的越慢，僵尸越多
    //减小flagToSumNum.second中的值可令游戏简单
}

GameLevelData_2::GameLevelData_2()
{
    qDebug()<<"enter game 2";
    backgroundImage = "interface/background1.jpg";
    backgroundMusic = "qrc:/audio/UraniwaNi.mp3";
    sunNum = 100;
    canSelectCard = true;
    showScroll = true;
    eName = "2";
    cName = tr("Level 1-2");
    pName = { "oPeashooter", "oSnowPea","oRepeater", "oSunflower","oPotatoMine", "oWallNut","oTorchwood","oCherryBomb","oGatlingPea","oTallNut"};
    zombieData = { { "oZombie", 1, 1, {} }, { "oZombie2", 1, 2, {} }, { "oZombie3", 1, 2, {} },
                   { "oConeheadZombie", 2, 3, {} }, { "oBucketheadZombie", 2, 7, {} }, { "oScreenDoorZombie", 5, 10, {} },
                   { "oFootballZombie", 3, 5, {} }, { "oPoleVaultingZombie", 3, 3, {} }};
    flagNum = 25;
    largeWaveFlag = { 7, 13, 19, 25};
    flagToSumNum = QPair<QList<int>, QList<int> >({3, 6, 9, 12, 15, 18, 21, 24}, { 10, 2, 4, 7, 11, 16, 22, 29, 46});
}

GameLevelData_3::GameLevelData_3()
{
    qDebug()<<"enter game 3";
    backgroundImage = "interface/background1.jpg";
    backgroundMusic = "qrc:/audio/UraniwaNi.mp3";
    sunNum = 100;
    canSelectCard = true;
    showScroll = true;
    eName = "3";
    cName = tr("Level 1-3");
    pName = { "oPeashooter", "oSnowPea","oRepeater", "oSunflower","oPotatoMine", "oWallNut","oTorchwood","oCherryBomb","oGatlingPea","oTallNut"};

    zombieData = { { "oZombie", 2, 1, {} }, { "oZombie2", 2, 2, {} }, { "oZombie3", 2, 2, {} },
                   { "oConeheadZombie", 2, 2, {} }, { "oBucketheadZombie", 1, 3, {} },{ "oScreenDoorZombie", 5, 7, {} },
                   { "oFootballZombie", 3, 5, {} }, { "oPoleVaultingZombie", 3, 3, {} }};
    flagNum = 10;
    largeWaveFlag = { 5, 10};
    flagToSumNum = QPair<QList<int>, QList<int> >({3, 5, 10, 12, 14 }, { 1, 5, 7, 10, 15, 30 });
}

GameLevelData_4::GameLevelData_4()
{
    qDebug()<<"enter game 4";
    backgroundImage = "interface/background1.jpg";
    backgroundMusic = "qrc:/audio/UraniwaNi.mp3";
    sunNum = 100;
    canSelectCard = true;
    showScroll = true;
    eName = "4";
    cName = tr("Level 1-4");
    pName = { "oPeashooter", "oSnowPea","oRepeater", "oSunflower","oPotatoMine", "oWallNut","oTorchwood","oCherryBomb","oGatlingPea","oTallNut"};

    zombieData = { { "oZombie", 2, 1, {} }, { "oZombie2", 1, 2, {} }, { "oZombie3", 1, 2, {} },
                   { "oConeheadZombie", 2, 2, {} }, { "oBucketheadZombie", 1, 3, {} },{ "oScreenDoorZombie", 5, 7, {} },
                   { "oFootballZombie", 3, 5, {} }, { "oPoleVaultingZombie", 3, 3, {} }};
    flagNum = 15;
    largeWaveFlag = { 1, 10, 15 };
    flagToSumNum = QPair<QList<int>, QList<int> >({3, 5, 10, 12, 14 }, { 3, 5, 7, 10, 15, 24 });
}

GameLevelData_5::GameLevelData_5()
{
    qDebug()<<"enter game 5";


    //坐标设置为池塘坐标
    coord=1;

    //再第五行之后插入第六行
    LF.insert(5,1);
    //将第三个元素设置为2
    //将第四个元素设置为2
    LF.replace(3,2);
    LF.replace(4,2);



    backgroundImage = "interface/background3.jpg";
    backgroundMusic = "qrc:/audio/UraniwaNi.mp3";
    sunNum = 100;
    canSelectCard = true;
    showScroll = true;
    eName = "5";
    cName = tr("Level 1-5");
    pName = { "oPeashooter", "oSnowPea","oRepeater", "oSunflower","oPotatoMine", "oWallNut","oTorchwood","oCherryBomb","oGatlingPea","oTallNut","oLilyPad"};

    zombieData = { { "oZombie", 1, 1, {} }, { "oZombie2", 1, 2, {} }, { "oZombie3", 1, 2, {} },
                   { "oConeheadZombie", 1, 2, {} }, { "oBucketheadZombie", 1, 3, {} },{ "oScreenDoorZombie", 1, 7, {} },
                   { "oFootballZombie", 1, 5, {} }, { "oPoleVaultingZombie", 1, 3, {} },{"oDuckyTubeZombie1",5,1,{}}};
    flagNum = 15;
    largeWaveFlag = { 1, 10, 15 };
    flagToSumNum = QPair<QList<int>, QList<int> >({3, 5, 10, 12, 14 }, { 2, 4, 7, 10, 15, 24 });
}

GameLevelData_6::GameLevelData_6()
{
    qDebug()<<"enter game 5";
    backgroundImage = "interface/background2.jpg";
    backgroundMusic = "qrc:/audio/UraniwaNi.mp3";
    sunNum = 100;
    canSelectCard = true;
    showScroll = true;
    eName = "6";
    cName = tr("Level 1-6");
    pName = { "oPeashooter", "oSnowPea","oRepeater", "oSunflower","oPotatoMine", "oWallNut","oTorchwood","oCherryBomb","oGatlingPea","oTallNut"};

    zombieData = { { "oZombie", 1, 1, {} }, { "oZombie2", 1, 2, {} }, { "oZombie3", 1, 2, {} },
                   { "oConeheadZombie", 2, 2, {} }, { "oBucketheadZombie", 1, 3, {} },{ "oScreenDoorZombie", 5, 7, {} },
                   { "oFootballZombie", 3, 5, {} }, { "oPoleVaultingZombie", 3, 3, {} }};
    flagNum = 15;
    largeWaveFlag = { 1, 10, 15 };
    flagToSumNum = QPair<QList<int>, QList<int> >({3, 5, 10, 12, 14 }, { 3, 5, 7, 10, 15, 24 });
}


GameLevelData *GameLevelDataFactory(const QString &eName)       //MainView::switchToGameScene中选择关卡
{
    if (eName == "1")
        return new GameLevelData_1;
    if (eName == "2")
        return new GameLevelData_2;
    if (eName == "3")
        return new GameLevelData_3;
    if (eName == "4")
        return new GameLevelData_4;
    if (eName == "5")
        return new GameLevelData_5;
    if (eName == "6")
        return new GameLevelData_6;
    return nullptr;
}
