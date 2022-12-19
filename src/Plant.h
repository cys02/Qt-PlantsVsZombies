//
// Created by sun on 8/26/16.
//

#ifndef PLANTS_VS_ZOMBIES_PLANT_H
#define PLANTS_VS_ZOMBIES_PLANT_H

#include <QtCore>
#include <QtWidgets>
#include <QtMultimedia>

class MoviePixmapItem;
class GameScene;
class ZombieInstance;
class Trigger;


class Plant
{
    Q_DECLARE_TR_FUNCTIONS(Plant)

public:
    Plant();
    virtual  ~Plant() {}

    QString eName, cName;
    int width, height;
    int hp, pKind, bKind;
    int beAttackedPointL, beAttackedPointR;
    int zIndex;
    QString cardGif, staticGif, normalGif;
    bool canEat, canSelect, night;
    double coolTime;
    int stature, sleep;
    int sunNum;
    QString toolTip;

    virtual double getDX() const;
    virtual double getDY(int x, int y) const;
    virtual bool canGrow(int x, int y) const;

    GameScene *scene;   //游戏场景
    void update();
};

class PlantInstance
{
public:
    PlantInstance(const Plant *plant);
    virtual ~PlantInstance();

    virtual void birth(int c, int r);
    virtual void initTrigger();                                                     //触发函数
    virtual void triggerCheck(ZombieInstance *zombieInstance, Trigger *trigger);    //触发条件判断——僵尸作为参数
    virtual void normalAttack(ZombieInstance *zombieInstance);                      //攻击函数
    virtual void getHurt(ZombieInstance *zombie, int aKind, int attack);            //被攻击函数

    bool contains(const QPointF &pos);

    const Plant *plantProtoType;

    QUuid uuid;
    int row, col;
    int hp;
    bool canTrigger;

    //可以翻过
    bool canVaulted;

    qreal attackedLX, attackedRX;
    QMap<int, QList<Trigger *> > triggers;

    QGraphicsPixmapItem *shadowPNG;     //图形像素图
    MoviePixmapItem *picture;           //电影像素图

    int posx;
    int posy;
};

class Peashooter: public Plant  //豌豆射手
{
    Q_DECLARE_TR_FUNCTIONS(Peashooter)
public:
    Peashooter();
};

class PeashooterInstance: public PlantInstance
{
public:
    PeashooterInstance(const Plant *plant);
    virtual void normalAttack(ZombieInstance *zombieInstance);
private:
    QString bulletGif, bulletHitGif;
    QMediaPlayer *firePea;
};

class Repeater:public Peashooter{   //双发射手
    Q_DECLARE_TR_FUNCTIONS(Repeater)
public:
    Repeater();
};

class RepeaterInstance: public PeashooterInstance{
public:
    RepeaterInstance(const Plant *plant);
    virtual void normalAttack(ZombieInstance *zombieInstance);
private:
    QString bulletGif, bulletHitGif;
    QMediaPlayer *firePea;
};

class GatlingPea: public Peashooter
{
    Q_DECLARE_TR_FUNCTIONS(GatlingPea)
public:
    GatlingPea();
};

class GatlingPeaInstance: public PeashooterInstance
{
public:
    GatlingPeaInstance(const Plant *plant);
    virtual void normalAttack(ZombieInstance *zombieInstance);
private:
    QString bulletGif, bulletHitGif;
    QMediaPlayer *firePea;
};

class SnowPea: public Peashooter        //寒冰射手
{
    Q_DECLARE_TR_FUNCTIONS(SnowPea)
public:
    SnowPea();
};

class SnowPeaInstance:public PeashooterInstance{
public:
    SnowPeaInstance(const Plant *plant);
    virtual void normalAttack(ZombieInstance *zombieInstance);
private:
    QString bulletGif,bulletHitGif;
    QMediaPlayer *firePea;
};

class SunFlower: public Plant
{
    Q_DECLARE_TR_FUNCTIONS(SunFlower)
public:
    SunFlower();
};

class SunFlowerInstance: public PlantInstance
{
public:
    SunFlowerInstance(const Plant *plant);
    virtual void initTrigger();
private:
    QString lightedGif;
};

class WallNut: public Plant         //坚果墙
{
    Q_DECLARE_TR_FUNCTIONS(WallNut)
public:
    WallNut();
    virtual bool canGrow(int x, int y) const;
};

class WallNutInstance: public PlantInstance
{
public:
    WallNutInstance(const Plant *plant);
    virtual void initTrigger();
    virtual void getHurt(ZombieInstance *zombie, int aKind, int attack);
private:
    int hurtStatus;
    QString crackedGif1, crackedGif2;
};

class TallNut: public WallNut   //高坚果
{
    Q_DECLARE_TR_FUNCTIONS(TallNut)
public:
    TallNut();
    virtual bool canGrow(int x, int y) const;
};

class TallNutInstance: public WallNutInstance
{
public:
    TallNutInstance(const Plant *plant);
    int hurtStatus,hurtPoint1,hurtPoint2;
    QString crackedGif1, crackedGif2;
};


class Torchwood:public Plant{           //火炬树桩
    Q_DECLARE_TR_FUNCTIONS(Torchwood)
public:
    Torchwood();
};

class TorchwoodInstance:public PlantInstance{
public:
    TorchwoodInstance(const Plant *plant);
    virtual void initTrigger();
private:

};

class PotatoMine: public Plant      //土豆雷
{
    Q_DECLARE_TR_FUNCTIONS(PotatoMine)
public:
    PotatoMine();
};

class PotatoMineInstance: public PlantInstance
{
public:
    PotatoMineInstance(const Plant *plant);
    virtual void initTrigger();             //触发
    virtual void triggerCheck(ZombieInstance *zombieInstance, Trigger *trigger);    //触发判断
    virtual void normalAttack();            //攻击
private:
    QString explosionSpudowGif, mashedGif;
};

class CherryBomb:public Plant{          //樱桃炸弹
    Q_DECLARE_TR_FUNCTIONS(CherryBomb)
public:
    CherryBomb();
};
class CherryBombInstance:public PlantInstance{
public :
    CherryBombInstance(const Plant *plant);
    virtual void initTrigger();
    virtual void normalAttack();
    virtual void triggerCheck(ZombieInstance *zombieInstance, Trigger *trigger);
private:
    QString BoomGif;
};


class LilyPad:public Plant{
    Q_DECLARE_TR_FUNCTIONS(LilyPad)
public:
    LilyPad();
    virtual bool canGrow(int x, int y) const;
};

class LilyPadInstance:public PlantInstance{
public:
    LilyPadInstance(const Plant *plant);
    virtual void initTrigger();
};

class LawnCleaner: public Plant         //除草机
{
    Q_DECLARE_TR_FUNCTIONS(LawnCleaner)
public:
    LawnCleaner();
};

class LawnCleanerInstance: public PlantInstance
{
public:
    LawnCleanerInstance(const Plant *plant);
    virtual void initTrigger();
    virtual void triggerCheck(ZombieInstance *zombieInstance, Trigger *trigger);
    virtual void normalAttack(ZombieInstance *zombieInstance);
};

class PoolCleaner: public LawnCleaner
{
    Q_DECLARE_TR_FUNCTIONS(PoolCleaner)
public:
    PoolCleaner();
    //virtual bool canGrow(int x, int y) const;
};

class PoolCleanerInstance: public PlantInstance
{
public:
    PoolCleanerInstance(const Plant *plant);
    virtual void initTrigger();
    virtual void triggerCheck(ZombieInstance *zombieInstance, Trigger *trigger);
    virtual void normalAttack(ZombieInstance *zombieInstance);
};

class Bullet            //子弹（豌豆）
{
public:
    Bullet(GameScene *scene, int type, int row, qreal from, qreal x, qreal y, qreal zvalue,  int direction);
    ~Bullet();
    void start();
private:
    void move();

    GameScene *scene;
    int count, type, row, direction;
    bool beFired;
    qreal from;
    QGraphicsPixmapItem *picture;
};

Plant *PlantFactory(GameScene *scene, const QString &eName);
PlantInstance *PlantInstanceFactory(const Plant *plant);

#endif //PLANTS_VS_ZOMBIES_PLANT_H
