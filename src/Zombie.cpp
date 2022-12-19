//
// Created by sun on 8/26/16.
//

#include "Zombie.h"
#include "GameScene.h"
#include "GameLevelData.h"
#include "ImageManager.h"
#include "MouseEventPixmapItem.h"
#include "Plant.h"
#include "Timer.h"

Zombie::Zombie()
    : hp(270), level(1), speed(1.5),
      aKind(0), attack(100),
      canSelect(true), canDisplay(true),
      beAttackedPointL(82), beAttackedPointR(156),
      breakPoint(90), sunNum(0), coolTime(0)
{}

bool Zombie::canPass(int row) const
{
    return scene->getGameLevelData()->LF[row] == 1;
}

void Zombie::update()
{
    QPixmap pic = gImageCache->load(staticGif);
    width = pic.width();
    height = pic.height();
}

Zombie1::Zombie1()
{
    eName = "oZombie";
    cName = tr("Zombie");
    cardGif = "Card/Zombies/Zombie.png";
    QString path = "Zombies/Zombie/";
    staticGif = path + "0.gif";     //静态
    normalGif = path + "Zombie.gif";    //动态
    attackGif = path + "ZombieAttack.gif";  //僵尸攻击
    lostHeadGif = path + "ZombieLostHead.gif";      //僵尸掉头
    lostHeadAttackGif = path + "ZombieLostHeadAttack.gif";  //僵尸掉头后攻击
    headGif = path + "ZombieHead.gif";  //僵尸头
    dieGif = path + "ZombieDie.gif";    //僵尸死
    boomDieGif = path + "BoomDie.gif";  //爆炸死亡
    standGif = path + "1.gif";          //站着
}

DuckyTubeZombie1::DuckyTubeZombie1()
{
    eName = "oDuckyTubeZombie1";
    cName = tr("DuckyTubeZombie1");
    cardGif = "Card/Zombies/DuckyTubeZombie1.png";
    QString path = "Zombies/DuckyTubeZombie1/";
    staticGif = path + "0.gif";     //静态
    normalGif = path + "Walk2.gif";    //动态
    attackGif = path + "Attack.gif";  //僵尸攻击
    //lostHeadGif ="Zombies/Zombie/ZombieLostHead.gif";      //僵尸掉头
    lostHeadAttackGif = "Zombies/Zombie/ZombieLostHeadAttack.gif";  //僵尸掉头后攻击
    headGif = "Zombies/Zombie/ZombieHead.gif";  //僵尸头
    dieGif = path + "Die.gif";    //僵尸死
    //boomDieGif = path + "BoomDie.gif";  //爆炸死亡
    standGif = path + "1.gif";          //站着
}

bool DuckyTubeZombie1::canPass(int row) const
{
    return scene->getGameLevelData()->LF[row] == 2;
}

//DuckyTubeZombieInstance1::DuckyTubeZombieInstance1(const Zombie *zombie)
//    :ZombieInstance(zombie)
//{
//    shadowPNG = new QGraphicsPixmapItem(gImageCache->load(""));
//}



Zombie2::Zombie2()      //不同形状的普通僵尸
{
    eName = "oZombie2";
    normalGif = "Zombies/Zombie/Zombie2.gif";
    standGif = "Zombies/Zombie/2.gif";
}

Zombie3::Zombie3()      //不同形状的普通僵尸
{
    eName = "oZombie3";
    normalGif = "Zombies/Zombie/Zombie3.gif";
    standGif = "Zombies/Zombie/3.gif";
}

FlagZombie::FlagZombie()        //举旗僵尸
{
    eName = "oFlagZombie";
    cName = tr("Flag Zombie");
    speed = 2.2;
    beAttackedPointR = 101;
    QString path = "Zombies/FlagZombie/";
    cardGif = "Card/Zombies/FlagZombie.png";
    staticGif = path + "0.gif";
    normalGif = path + "FlagZombie.gif";
    attackGif = path + "FlagZombieAttack.gif";
    lostHeadGif = path + "FlagZombieLostHead.gif";
    lostHeadAttackGif = path + "FlagZombieLostHeadAttack.gif";
    standGif = path + "1.gif";
}

ZombieInstance::ZombieInstance(const Zombie *zombie)
    : zombieProtoType(zombie), picture(new MoviePixmapItem),
      attackMusic(new QMediaPlayer(picture)),
      hitMusic(new QMediaPlayer(picture)),
      DecelerationLayor(0)      //减速层
{
    uuid = QUuid::createUuid();
    hp = zombieProtoType->hp;
    speed = zombie->speed;

    //不同僵尸速度不同，设置位置偏移量
    posyOffset=10;
    posxOffset=0;

    altitude = 1;
    beAttacked = true;
    beSlowed=false;
    isAttacking = false;
    goingDie = false;
    normalGif = zombie->normalGif;
    attackGif = zombie->attackGif;
}

void ZombieInstance::birth(int row)     //参数为在第几行
{
    ZX =attackedLX = zombieProtoType->scene->getCoordinate().getX(11);
    X = attackedLX - zombieProtoType->beAttackedPointL;
    attackedRX = X + zombieProtoType->beAttackedPointR;
    this->row = row;

    Coordinate &coordinate = zombieProtoType->scene->getCoordinate();   //协调
    picture->setMovie(normalGif);
    //高度
    picture->setPos(X, coordinate.getY(row) - zombieProtoType->height +15- posyOffset);
    picture->setZValue(3 * row + 1);

    //鸭子僵尸没有阴影（if不是鸭子僵尸）
    if(!(zombieProtoType->scene->coord==1&&(row==3||row==4))){
        picture->setPos(X, coordinate.getY(row) - zombieProtoType->height - posyOffset);
        picture->setZValue(3 * row + 1);
        shadowPNG = new QGraphicsPixmapItem(gImageCache->load("interface/plantShadow.png"));
        //阴影高度
        shadowPNG->setPos(zombieProtoType->beAttackedPointL, zombieProtoType->height - 32 + posyOffset);
        shadowPNG->setFlag(QGraphicsItem::ItemStacksBehindParent);
        shadowPNG->setParentItem(picture);
    }
    picture->start();
    zombieProtoType->scene->addToGame(picture);
}

void ZombieInstance::checkActs()            //检查行为
{
    if (hp < 1) return;
    if (beAttacked && !isAttacking) {       //被击打/正在攻击
        judgeAttack();                      //判断伤害
    }
    if (!isAttacking) {                     //处于不在攻击的状态
        attackedRX -= beSlowed ? (speed/5) : speed;                //攻击速度
        ZX = attackedLX -= beSlowed ? (speed/5) : speed;           //ZX是僵尸模型在X轴上的位置
        X -= beSlowed ? (speed/5): speed;                          //X是僵尸图片在X轴上的位置
        picture->setX(X);
        if (attackedRX < -50) {
            zombieProtoType->scene->zombieDie(this);
        }
        else if (attackedRX < 100) {
            // TODO: Lose
            zombieProtoType->scene->gameStatus = true;
            zombieProtoType->scene->loseGame();
        }
    }
}

void ZombieInstance::judgeAttack()      //伤害/攻击判断
{
    bool tempIsAttacking = false;
    PlantInstance *plant = nullptr;
    int col = zombieProtoType->scene->getCoordinate().getCol(ZX);
    if (col >= 1 && col <= 9) {
        auto plants = zombieProtoType->scene->getPlant(col, row);
        QList<int> keys = plants.keys();
        qSort(keys.begin(), keys.end(), [](int a, int b) { return b < a; });
        for (auto key: keys) {
            plant = plants[key];
            if (plant->plantProtoType->canEat && plant->attackedRX >= ZX && plant->attackedLX <= ZX) {
                tempIsAttacking = true;
                break;
            }
        }
    }
    if (tempIsAttacking != isAttacking) {
        isAttacking = tempIsAttacking;
        if (isAttacking) {
            picture->setMovie(attackGif);
        }
        else
            picture->setMovie(normalGif);
        picture->start();
    }
    if (isAttacking)
        normalAttack(plant);
}

void ZombieInstance::normalAttack(PlantInstance *plantInstance)     //僵尸吃植物
{
    if (qrand() % 2)
        attackMusic->setMedia(QUrl("qrc:/audio/chomp.mp3"));
    else
        attackMusic->setMedia(QUrl("qrc:/audio/chompsoft.mp3"));
    attackMusic->play();
    (new Timer(this->picture, beSlowed ? 1000 : 500, [this] {
        if (qrand() % 2)
            attackMusic->setMedia(QUrl("qrc:/audio/chomp.mp3"));
        else
            attackMusic->setMedia(QUrl("qrc:/audio/chompsoft.mp3"));
        attackMusic->play();
    }))->start();
    QUuid plantUuid = plantInstance->uuid;
    (new Timer(this->picture, beSlowed ? 2000 : 1000, [this, plantUuid] {
        if (beAttacked) {
            PlantInstance *plant = zombieProtoType->scene->getPlant(plantUuid);
            if (plant)
                plant->getHurt(this, zombieProtoType->aKind, zombieProtoType->attack);
            judgeAttack();
        }
    }))->start();
}

ZombieInstance::~ZombieInstance()
{
    picture->deleteLater();
}

void ZombieInstance::crushDie()
{
    if (goingDie)
        return;
    goingDie =  true;
    hp = 0;
    MoviePixmapItem *goingDieHead = new MoviePixmapItem(zombieProtoType->headGif);
    goingDieHead->setPos(zombieProtoType->beAttackedPointL, -20);
    goingDieHead->setParentItem(picture);
    goingDieHead->start();
    if(!(zombieProtoType->scene->coord==1&&(row==3||row==4))){
        shadowPNG->setPixmap(QPixmap());
    }
    picture->stop();
    picture->setPixmap(QPixmap());
    (new Timer(picture, 2000, [this] {
        // TODO: Pole Vaulting Zombie
        zombieProtoType->scene->zombieDie(this);
    }))->start();
}

void ZombieInstance::getPea(int attack, int direction,int type)//type表示目前状态，-1表示减速
{
    playNormalballAudio();
    if(type==-1)getSlowed(10);  //参数表示减速多少
    getHit(attack);
}

void ZombieInstance::getHit(int attack)
{
    if (!beAttacked || goingDie)
        return;
    hp -= attack;
    if (hp < zombieProtoType->breakPoint) {
        //hp = 0;
        if (isAttacking)
            picture->setMovie(zombieProtoType->lostHeadAttackGif);
        else
            picture->setMovie(zombieProtoType->lostHeadGif);
        picture->start();
        MoviePixmapItem *goingDieHead = new MoviePixmapItem(zombieProtoType->headGif);
        goingDieHead->setPos(attackedLX-posxOffset , picture->y() - 20);
        goingDieHead->setZValue(picture->zValue());
        zombieProtoType->scene->addToGame(goingDieHead);
        goingDieHead->start();
        (new Timer(zombieProtoType->scene, 2000, [goingDieHead] {
            goingDieHead->deleteLater();
        }))->start();
        beAttacked = 0;
        autoReduceHp();
    }
    else {
        picture->setOpacity(0.5);
        (new Timer(picture, 100, [this] {
            picture->setOpacity(1);
        }))->start();
    }
}

void ZombieInstance::getBoom(int attack){       //参数attack表示扣血
    hp-=attack;
    if(hp<=0){                                  //
        MoviePixmapItem *boomDie=new MoviePixmapItem(zombieProtoType->boomDieGif);
        //爆炸死的动画
        boomDie->setPos(picture->pos());
        //设置爆炸的位置
        zombieProtoType->scene->addToGame(boomDie);
        //设置爆炸死
        boomDie->start();
        //执行
        (new Timer(zombieProtoType->scene,3000,[boomDie]{
            boomDie->deleteLater();
        }))->start();
        //设置计时器，僵尸爆炸死的动画持续多久（3s）
        zombieProtoType->scene->zombieDie(this);
        //僵尸死亡
    }
}

void ZombieInstance::getSlowed(const int time)//减速函数
{
    if(!beSlowed){
        picture->setRGB(0,0,255);
        beSlowed=true;
    }
    DecelerationLayor++;        //设置减速层
    (new Timer(picture,3000,[this]{    //调节减速持续的时间
        DecelerationLayor--;
        if(DecelerationLayor<=0){
            beSlowed=false;
            picture->setRGB(255,255,255);
        }
    }))->start();

}
void ZombieInstance::autoReduceHp()
{
    (new Timer(picture, 1000, [this] {
        hp-= 60;
        if (hp < 1)
            normalDie();
        else
            autoReduceHp();
    }))->start();
}

void ZombieInstance::normalDie()
{
    if (goingDie)
        return;
    goingDie =  true;
    hp = 0;
    picture->setMovie(zombieProtoType->dieGif);
    picture->start();
    (new Timer(picture, 2500, [this] {
        zombieProtoType->scene->zombieDie(this);
    }))->start();
}

void ZombieInstance::playNormalballAudio()
{
    hitMusic->stop();
    switch (qrand() % 3) {
        case 0: hitMusic->setMedia(QUrl("qrc:/audio/splat1.mp3")); break;
        case 1: hitMusic->setMedia(QUrl("qrc:/audio/splat2.mp3")); break;
        default: hitMusic->setMedia(QUrl("qrc:/audio/splat3.mp3")); break;
    }
    hitMusic->play();
}


OrnZombieInstance1::OrnZombieInstance1(const Zombie *zombie)
    : ZombieInstance(zombie)
{
    ornHp = getZombieProtoType()->ornHp;
    hasOrnaments = true;
}

const OrnZombie1 *OrnZombieInstance1::getZombieProtoType()
{
    return static_cast<const OrnZombie1 *>(zombieProtoType);
}

void OrnZombieInstance1::getHit(int attack)
{
    if (hasOrnaments) {
        ornHp -= attack;
        if (ornHp < 1) {
            hp += ornHp;
            hasOrnaments = false;
            normalGif = getZombieProtoType()->ornLostNormalGif;
            attackGif = getZombieProtoType()->ornLostAttackGif;
            picture->setMovie(isAttacking ? attackGif : normalGif);
            picture->start();
        }
        picture->setOpacity(0.5);
        (new Timer(picture, 100, [this] {
            picture->setOpacity(1);
        }))->start();
    }
    else
        ZombieInstance::getHit(attack);
}

ConeheadZombie::ConeheadZombie()
{
    eName = "oConeheadZombie";
    cName = tr("Conehead Zombie");
    ornHp = 370;
    level = 2;
    sunNum = 75;
    QString path = "Zombies/ConeheadZombie/";
    cardGif = "Card/Zombies/ConeheadZombie.png";
    staticGif = path + "0.gif";
    normalGif = path + "ConeheadZombie.gif";
    attackGif = path + "ConeheadZombieAttack.gif";
    ornLostNormalGif =  "Zombies/Zombie/Zombie.gif";
    ornLostAttackGif = "Zombies/Zombie/ZombieAttack.gif";
    standGif = path + "1.gif";
}

ConeheadZombieInstance::ConeheadZombieInstance(const Zombie *zombie)
    : OrnZombieInstance1(zombie)
{}

void ConeheadZombieInstance::playNormalballAudio()
{
    if (hasOrnaments) {
        hitMusic->stop();
        hitMusic->setMedia(QUrl("qrc:/audio/plastichit.mp3"));
        hitMusic->play();
    }
    else
        OrnZombieInstance1::playNormalballAudio();
}

BucketheadZombieInstance::BucketheadZombieInstance(const Zombie *zombie)
    : OrnZombieInstance1(zombie)
{}

void BucketheadZombieInstance::playNormalballAudio()
{
    if (hasOrnaments) {
        hitMusic->stop();
        if (qrand() % 2)
            hitMusic->setMedia(QUrl("qrc:/audio/shieldhit.mp3"));
        else
            hitMusic->setMedia(QUrl("qrc:/audio/shieldhit2.mp3"));
        hitMusic->play();
    }
    else
        OrnZombieInstance1::playNormalballAudio();
}

BucketheadZombie::BucketheadZombie()
{
    eName = "oBucketheadZombie";
    cName = tr("Buckethead Zombie");
    ornHp = 1100;
    level = 3;
    sunNum = 125;
    QString path = "Zombies/BucketheadZombie/";
    cardGif = "Card/Zombies/BucketheadZombie.png";
    staticGif = path + "0.gif";
    normalGif = path + "BucketheadZombie.gif";
    attackGif = path + "BucketheadZombieAttack.gif";
    ornLostNormalGif =  "Zombies/Zombie/Zombie2.gif";
    standGif = path + "1.gif";
}

FootballZombie::FootballZombie()
{
    eName = "oFootballZombie";
    cName = tr("Football Zombie");
    ornHp = 1100;
    level = 3;
    sunNum = 150;
    QString path = "Zombies/FootballZombie/";
    cardGif = "Card/Zombies/FootballZombie.png";
    staticGif = path + "0.gif";
    normalGif = path + "FootballZombie.gif";
    attackGif = path + "Attack.gif";
    lostHeadGif = path + "LostHead.gif";
    lostHeadAttackGif = path + "LostHeadAttack.gif";
    //headGif = path + "ZombieHead.gif";
    dieGif = path + "Die.gif";
    boomDieGif = path + "BoomDie.gif";
    ornLostNormalGif =  path + "FootballZombieOrnLost.gif";
    ornLostAttackGif = path + "FootballZombieOrnLostAttack.gif";
    standGif = path + "1.gif";
}

FootballZombieInstance::FootballZombieInstance(const Zombie *zombie)    //铁门僵尸
    : OrnZombieInstance1(zombie){
    posxOffset=0;
    posyOffset=0;
}


//处理音频的函数
void FootballZombieInstance::playNormalballAudio()
{
    //如果有门
    if (hasOrnaments) {
        hitMusic->stop();

        //随机播放音频
        //if (qrand() % 2)
            hitMusic->setMedia(QUrl("qrc:/audio/plastichit.mp3"));
//        else
//            hitMusic->setMedia(QUrl("qrc:/audio/shieldhit2.mp3"));
        hitMusic->play();
    }
    else
        OrnZombieInstance1::playNormalballAudio();
}


PoleVaultingZombie::PoleVaultingZombie()
{
    eName = "oPoleVaultingZombie";
    cName = tr("Pole Vaulting Zombie");
    hp = 500;
    speed = 3.2;
    //被打击的范围
    beAttackedPointL = 215;
    beAttackedPointR = 260;
    level = 2;
    sunNum = 75;
    cardGif = "Card/Zombies/PoleVaultingZombie.png";
    QString path = "Zombies/PoleVaultingZombie/";
    staticGif = path + "0.gif";
    normalGif = path + "PoleVaultingZombie.gif";
    attackGif = path + "PoleVaultingZombieAttack.gif";
    lostHeadGif = path + "PoleVaultingZombieLostHead.gif";
    lostHeadAttackGif = path + "PoleVaultingZombieLostHeadAttack.gif";
    headGif = path + "PoleVaultingZombieHead.gif";
    dieGif = path + "PoleVaultingZombieDie.gif";
    boomDieGif = path + "BoomDie.gif";
    standGif = path + "1.gif";
    jumpGif1 = path + "PoleVaultingZombieJump.gif";
    jumpGif2 = path + "PoleVaultingZombieJump2.gif";
}

PoleVaultingZombieInstance::PoleVaultingZombieInstance(const Zombie *zombie)
    :ZombieInstance (zombie),hasPole(true)          //初始化为有杆
{
    //高度偏移
    posyOffset=-2;
    //位置偏移
    posxOffset=160;
}

void PoleVaultingZombieInstance::judgeAttack()
{
    //僵尸判断攻击的过程
    bool tempIsAttacking = false;
    PlantInstance *plant = nullptr;
    int col = zombieProtoType->scene->getCoordinate().getCol(ZX);
    if (col >= 1 && col <= 9) {
        //定义一个新类型，键为模型的位置，值为僵尸的行数
        auto plants = zombieProtoType->scene->getPlant(col, row);
        //取僵尸X坐标
        QList<int> keys = plants.keys();
        qSort(keys.begin(), keys.end(), [](int a, int b) { return b < a; });
        for (auto key: keys) {
            plant = plants[key];
            if (plant->plantProtoType->canEat && plant->attackedRX >= (hasPole?ZX-40:ZX) && plant->attackedLX <= ZX) {
                tempIsAttacking = true;
                break;
            }
        }
    }

    //如果应该攻击同时有杆——执行跳跃动画
    if(tempIsAttacking&&hasPole){
        //播放跳跃动画期间速度置为0
        speed = 0;
        hasPole = false;
        //设置动画
        picture->setMovie(((PoleVaultingZombie *)zombieProtoType)->jumpGif1);
        //开始动画
        picture->start();
        //设置和播放音乐
        QMediaPlayer *jumpMusic = new QMediaPlayer(picture);
        QMediaPlayer *voiceMusic = new QMediaPlayer(picture);
        jumpMusic->setMedia(QUrl("qrc:/audio/polevault.mp3"));
        voiceMusic->setMedia(QUrl("qrc:/audio/qifei.mp3"));
        jumpMusic->play();
        voiceMusic->play();
        //设置定时器改变位置
        (new Timer(picture, 1000, [this, jumpMusic, voiceMusic, plant] {
            picture->setMovie(((PoleVaultingZombie *)zombieProtoType)->jumpGif2);
            picture->start();

            //高坚果不能翻过
            if(plant->canVaulted){
                attackedRX -= 110;           //右侧攻击范围
                ZX = attackedLX -= 110;      //左侧攻击范围
                X -= 110;                   //跳跃距离
            }
            picture->setX(X);
            //新的计时器，播放音乐，更换动作——从跳跃状态到正常状态
            (new Timer(picture, 1000, [this, jumpMusic, voiceMusic] {
                speed = 1.5;
                normalGif = "Zombies/PoleVaultingZombie/PoleVaultingZombieWalk.gif";
                picture->setMovie(normalGif);
                picture->start();
                jumpMusic->deleteLater();
                voiceMusic->deleteLater();
            }))->start();
        }))->start();
    }

    if (speed==0)  tempIsAttacking = false;//防止动画期间僵尸攻击

    //将僵尸的状态保持一致
    if (tempIsAttacking != isAttacking) {
        isAttacking = tempIsAttacking;
        //播放攻击动画或正常行走动画
        if (isAttacking) {
            picture->setMovie(attackGif);
        }
        else
            picture->setMovie(normalGif);
        picture->start();
    }
    //播放攻击动画
    if (isAttacking)
        normalAttack(plant);
}


ScreenDoorZombie::ScreenDoorZombie()
{
    eName = "oScreenDoorZombie";
    cName = tr("Screen Door Zombie");
    ornHp = 1100;
    level = 3;
    sunNum = 150;
    QString path = "Zombies/ScreenDoorZombie/";
    cardGif = "Card/Zombies/ScreenDoorZombie.png";
    staticGif = path + "0.gif";
    normalGif = path + "ScreenDoorZombie.gif";
    attackGif = path + "ScreenDoorZombieAttack.gif";
    ornLostNormalGif =  "Zombies/Zombie/Zombie.gif";
    ornLostAttackGif = "Zombies/Zombie/ZombieAttack.gif";
    standGif = path + "1.gif";
}

ScreenDoorZombieInstance::ScreenDoorZombieInstance(const Zombie *zombie)    //铁门僵尸
    : OrnZombieInstance1(zombie){
    posxOffset=0;
    posyOffset=0;
}


//处理音频的函数
void ScreenDoorZombieInstance::playNormalballAudio()
{
    //如果有门
    if (hasOrnaments) {
        hitMusic->stop();

        //随机播放音频
        if (qrand() % 2)
            hitMusic->setMedia(QUrl("qrc:/audio/shieldhit.mp3"));
        else
            hitMusic->setMedia(QUrl("qrc:/audio/shieldhit2.mp3"));
        hitMusic->play();
    }
    else
        OrnZombieInstance1::playNormalballAudio();
}



Zombie *ZombieFactory(GameScene *scene, const QString &ename)
{
    Zombie *zombie = nullptr;
    if (ename == "oZombie")
        zombie = new Zombie1;
    if(ename == "oDuckyTubeZombie1")
        zombie = new DuckyTubeZombie1;
    if (ename == "oZombie2")
        zombie = new Zombie2;
    if (ename == "oZombie3")
        zombie = new Zombie3;
    if (ename == "oFlagZombie")
        zombie = new FlagZombie;
    if (ename == "oConeheadZombie")
        zombie = new ConeheadZombie;
    if (ename == "oBucketheadZombie")
        zombie = new BucketheadZombie;
    if (ename == "oFootballZombie")
        zombie = new FootballZombie;
    if(ename == "oPoleVaultingZombie")
        zombie = new PoleVaultingZombie;
    if(ename == "oScreenDoorZombie")
        zombie= new ScreenDoorZombie;
    if (zombie) {
        zombie->scene = scene;
        zombie->update();
    }
    return zombie;
}

ZombieInstance *ZombieInstanceFactory(const Zombie *zombie)//实列工厂
{
//    if(zombie->eName == "oDuckyTubeZombie1")
//        return new DuckyTubeZombieInstance1(zombie);
    if (zombie->eName == "oConeheadZombie")
        return new ConeheadZombieInstance(zombie);
    if (zombie->eName == "oBucketheadZombie")
        return new BucketheadZombieInstance(zombie);
    if (zombie->eName == "oFootballZombie")
        return new FootballZombieInstance(zombie);
    if(zombie->eName == "oPoleVaultingZombie")
        return new PoleVaultingZombieInstance(zombie);
    if(zombie->eName == "oScreenDoorZombie")
        return new ScreenDoorZombieInstance(zombie);
    return new ZombieInstance(zombie);
}


