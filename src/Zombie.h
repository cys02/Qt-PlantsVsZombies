//
// Created by sun on 8/26/16.
//

#ifndef PLANTS_VS_ZOMBIES_ZOMBIE_H
#define PLANTS_VS_ZOMBIES_ZOMBIE_H


#include <QtCore>
#include <QtWidgets>
#include <QtMultimedia>

class MoviePixmapItem;
class GameScene;
class PlantInstance;

class Zombie
{
    Q_DECLARE_TR_FUNCTIONS(Zombie)
public:
    Zombie();
    virtual ~Zombie() {}

    QString eName, cName;

    int width, height;      //宽度，高度

    int hp, level;          //血量，等级
    qreal speed;            //速度
    int aKind, attack;      //akind，攻击
    bool canSelect, canDisplay;     //可以选择，可以展示

    QString cardGif, staticGif, normalGif, attackGif, lostHeadGif,
            lostHeadAttackGif, headGif, dieGif, boomDieGif, standGif;

    int beAttackedPointL, beAttackedPointR;
    int breakPoint, sunNum;
    qreal coolTime;     //冷却时间

    virtual bool canPass(int row) const;        //可以通过（碰撞体积？）

    void update();

    GameScene *scene;
};

class Zombie1: public Zombie
{
    Q_DECLARE_TR_FUNCTIONS(Zombie1)
public:
    Zombie1();
};

class DuckyTubeZombie1:public Zombie    //鸭子僵尸
{
    Q_DECLARE_TR_FUNCTIONS(Zombie1)
public:
    DuckyTubeZombie1();
    virtual bool canPass(int row) const;
};

class Zombie2: public Zombie1
{
    Q_DECLARE_TR_FUNCTIONS(Zombie2)
public:
    Zombie2();
};

class Zombie3: public Zombie1
{
    Q_DECLARE_TR_FUNCTIONS(Zombie3)
public:
    Zombie3();
};

class FlagZombie: public Zombie1
{
    Q_DECLARE_TR_FUNCTIONS(FlagZombie)
public:
    FlagZombie();
};

class ZombieInstance
{
public:
    ZombieInstance(const Zombie *zombie);
    virtual ~ZombieInstance();

    virtual void birth(int row);    //出生
    virtual void checkActs();       //检查行为
    virtual void judgeAttack();     //判断攻击
    virtual void normalAttack(PlantInstance *plant);    //普通攻击
    virtual void crushDie();        //粉碎死(窝瓜)
    virtual void getPea(int attack, int direction,int type);     //豌豆
    virtual void getBoom(int attack);                   //爆炸死（变灰）
    virtual void getHit(int attack);                    //打击
    virtual void getSlowed(const int time);             //减速
    virtual void autoReduceHp();                        //自动减少hp
    virtual void normalDie();                           //正常
    virtual void playNormalballAudio();                 //播放音频

    QUuid uuid;
    int hp;
    qreal speed;
    int altitude;

    int posyOffset,posxOffset;  //设置位置偏移量

    bool beAttacked, isAttacking, goingDie,beSlowed;     //被攻击，正在攻击，死亡（过程）
    int DecelerationLayor;                               //减速层

    qreal X, ZX;
    qreal attackedLX, attackedRX;           //
    int row;                                //在第几行
    const Zombie *zombieProtoType;          //僵尸种类

    QString normalGif, attackGif;           //正常，攻击

    QGraphicsPixmapItem *shadowPNG;         //阴影
    MoviePixmapItem *picture;
    QMediaPlayer *attackMusic, *hitMusic;   //攻击音效
};

//class DuckyTubeZombieInstance1: public ZombieInstance{
//    //Q_DECLARE_TR_FUNCTIONS(DuckyTubeZombieInstance1)
//public:
//    DuckyTubeZombieInstance1(const Zombie *zombie);
//};

class OrnZombie1: public Zombie1
{
    Q_DECLARE_TR_FUNCTIONS(OrnZombie1)
public:
    int ornHp;
    QString ornLostNormalGif, ornLostAttackGif;
};

class OrnZombieInstance1: public ZombieInstance
{
public:
    OrnZombieInstance1(const Zombie *zombie);
    const OrnZombie1 *getZombieProtoType();
    virtual void getHit(int attack);

    int ornHp;
    bool hasOrnaments;      //有无配件
};

class ConeheadZombie: public OrnZombie1     //路障僵尸
{
    Q_DECLARE_TR_FUNCTIONS(ConeheadZombie)
public:
    ConeheadZombie();
};

class ConeheadZombieInstance: public OrnZombieInstance1
{
public:
    ConeheadZombieInstance(const Zombie *zombie);
    virtual void playNormalballAudio();

};

class BucketheadZombie: public ConeheadZombie       //铁桶僵尸
{
    Q_DECLARE_TR_FUNCTIONS(BucketheadZombie)
public:
    BucketheadZombie();
};

class BucketheadZombieInstance: public OrnZombieInstance1
{
public:
    BucketheadZombieInstance(const Zombie *zombie);
    virtual void playNormalballAudio();

};

class FootballZombie: public OrnZombie1      //橄榄球僵尸
{
    Q_DECLARE_TR_FUNCTIONS(FootballZombie)
public:
    FootballZombie();
};

class FootballZombieInstance: public OrnZombieInstance1
{
public:
    FootballZombieInstance(const Zombie *zombie);
    virtual void playNormalballAudio();

};

class PoleVaultingZombie: public Zombie1    //撑杆跳僵尸（继承自普通僵尸）
{
    Q_DECLARE_TR_FUNCTIONS(PoleVaultingZombie)
public:
    PoleVaultingZombie();
    QString jumpGif1,jumpGif2;
};
class PoleVaultingZombieInstance:public ZombieInstance{
public:
    bool hasPole;   //有没有杆
    PoleVaultingZombieInstance(const Zombie *zombie);
    virtual void judgeAttack();
};


class ScreenDoorZombie: public OrnZombie1   //铁门僵尸——继承自有配件的僵尸
{
    Q_DECLARE_TR_FUNCTIONS(ScreenDoorZombie)
public:
    ScreenDoorZombie();
};

class ScreenDoorZombieInstance: public OrnZombieInstance1
{
public:
    ScreenDoorZombieInstance(const Zombie *zombie);
    virtual void playNormalballAudio();

};



Zombie *ZombieFactory(GameScene *scene, const QString &ename);
ZombieInstance *ZombieInstanceFactory(const Zombie *zombie);

#endif //PLANTS_VS_ZOMBIES_ZOMBIE_H
