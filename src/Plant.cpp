//
// Created by sun on 8/26/16.
//

#include <QtMultimedia>
#include "Plant.h"
#include "ImageManager.h"
#include "GameScene.h"
#include "GameLevelData.h"
#include "MouseEventPixmapItem.h"
#include "Timer.h"
#include "Animate.h"

Plant::Plant()
        : hp(300),
          pKind(1), bKind(0),
          beAttackedPointL(20), beAttackedPointR(20),
          zIndex(0),
          canEat(true), canSelect(true), night(false),
          coolTime(7.5), stature(0), sleep(0), scene(nullptr)
{}

double Plant::getDX() const
{
    return -0.5 * width;
}

double Plant::getDY(int x, int y) const
{
    return scene->getPlant(x, y).contains(0) ? -21 : -15;
}

bool Plant::canGrow(int x, int y) const
{   
    if (x < 1 || x > 9 || y < 1 || y > 6)//5
        return false;
    if (scene->isCrater(x, y) || scene->isTombstone(x, y))
        return false;

    int groundType = scene->getGameLevelData()->LF[y];
    QMap<int, PlantInstance *> plants = scene->getPlant(x, y);
    if (groundType == 1||groundType == 2){
        if(scene->coord==1&&(y==3||y==4)&&!plants.contains(1)){
            return false;
        }
        else {
            return !plants.contains(1) || plants[1]->plantProtoType->eName == "oLilyPad";
        }
    }
    return plants.contains(0) && (!plants.contains(1)||plants[1]->plantProtoType->eName == "oLilyPad");
}

void Plant::update()
{
    QPixmap pic = gImageCache->load(staticGif);
    width = pic.width();
    height = pic.height();
}

PlantInstance::PlantInstance(const Plant *plant) : plantProtoType(plant)
{
    uuid = QUuid::createUuid();
    hp = plantProtoType->hp;
    canTrigger = true;
    canVaulted = true;
    picture = new MoviePixmapItem;
}

PlantInstance::~PlantInstance()
{
    picture->deleteLater();
}

void PlantInstance::birth(int c, int r)
{
    Coordinate &coordinate = plantProtoType->scene->getCoordinate();
    double x = coordinate.getX(c) + plantProtoType->getDX(), y = coordinate.getY(r) + plantProtoType->getDY(c, r) - plantProtoType->height;
    col = c, row = r;
    attackedLX = x + plantProtoType->beAttackedPointL;
    attackedRX = x + plantProtoType->beAttackedPointR;      //attackedLX???attackedRX??????????????????
    picture->setMovie(plantProtoType->normalGif);
    picture->setPos(x, y);
    picture->setZValue(plantProtoType->zIndex + 3 * r);
    shadowPNG = new QGraphicsPixmapItem(gImageCache->load("interface/plantShadow.png"));
    shadowPNG->setPos(plantProtoType->width * 0.5 - 48, plantProtoType->height - 22);
    shadowPNG->setFlag(QGraphicsItem::ItemStacksBehindParent);
    shadowPNG->setParentItem(picture);
    picture->start();
    plantProtoType->scene->addToGame(picture);
    initTrigger();
}

void PlantInstance::initTrigger()
{
    Trigger *trigger = new Trigger(this, attackedLX, 880, 0, 0);
    triggers.insert(row, QList<Trigger *>{ trigger } );
    plantProtoType->scene->addTrigger(row, trigger);
}

bool PlantInstance::contains(const QPointF &pos)
{
    return picture->contains(pos);
}

void PlantInstance::triggerCheck(ZombieInstance *zombieInstance, Trigger *trigger)
{
    if (zombieInstance->altitude > 0) {
        canTrigger = false;
        QSharedPointer<std::function<void(QUuid)> > triggerCheck(new std::function<void(QUuid)>);
        *triggerCheck = [this, triggerCheck] (QUuid zombieUuid) {
            (new Timer(picture, 1400, [this, zombieUuid, triggerCheck] {
                ZombieInstance *zombie = this->plantProtoType->scene->getZombie(zombieUuid);
                if (zombie) {
                    for (auto i: triggers[zombie->row]) {
                        if (zombie->hp > 0 && i->from <= zombie->ZX && i->to >= zombie->ZX && zombie->altitude > 0) {
                            normalAttack(zombie);
                            (*triggerCheck)(zombie->uuid);
                            return;
                        }
                    }
                }
                canTrigger = true;
            }))->start();
        };
        normalAttack(zombieInstance);
        (*triggerCheck)(zombieInstance->uuid);
    }
}

void PlantInstance::normalAttack(ZombieInstance *zombieInstance)
{
    qDebug() << plantProtoType->cName << uuid << "Attack" << zombieInstance->zombieProtoType->cName << zombieInstance;
}

void PlantInstance::getHurt(ZombieInstance *zombie, int aKind, int attack)
{
    if (aKind == 0)
        hp -= attack;
    if (hp < 1 || aKind != 0)
        plantProtoType->scene->plantDie(this);
}

Peashooter::Peashooter()        //????????????
{
    eName = "oPeashooter";
    cName = tr("Peashooter");
    beAttackedPointR = 51;
    sunNum = 0;
    cardGif = "Card/Plants/Peashooter.png";
    staticGif = "Plants/Peashooter/0.gif";
    normalGif = "Plants/Peashooter/Peashooter.gif";
    toolTip = tr("Shoots peas at zombies");
}

Repeater::Repeater(){           //????????????
    eName="oRepeater";
    cName=tr("Repeater");
    beAttackedPointR=51;
    sunNum=0;
    cardGif="Card/Plants/Repeater.png";
    staticGif="Plants/Repeater/0.gif";
    normalGif="Plants/Repeater/Repeater.gif";
    toolTip=tr("????????????????????????????????????");
}

GatlingPea::GatlingPea()
{
    eName = "oGatlingPea";
    cName = tr("GatlingPea");
    beAttackedPointR = 51;
    sunNum = 0;
    //coolTime = 50;
    cardGif = "Card/Plants/GatlingPea.png";
    staticGif = "Plants/GatlingPea/0.gif";
    normalGif = "Plants/GatlingPea/GatlingPea.gif";
    toolTip = tr("????????????????????????????????????");
}

SnowPea::SnowPea()
{
    eName = "oSnowPea";
    cName = tr("Snow Pea");
    bKind = -1;
    beAttackedPointR = 51;
    sunNum = 0;
    cardGif = "Card/Plants/SnowPea.png";
    staticGif = "Plants/SnowPea/0.gif";
    normalGif = "Plants/SnowPea/SnowPea.gif";
    toolTip = tr("Slows down zombies with cold precision");
}

SunFlower::SunFlower()      //?????????
{
    eName = "oSunflower";
    cName = tr("Sunflower");
    beAttackedPointR = 53;
    sunNum = 0;
    cardGif = "Card/Plants/SunFlower.png";
    staticGif = "Plants/SunFlower/0.gif";
    normalGif = "Plants/SunFlower/SunFlower1.gif";
    toolTip = tr("Makes extra Sun for placing plants");
}

SunFlowerInstance::SunFlowerInstance(const Plant *plant)
        : PlantInstance(plant),
          lightedGif("Plants/SunFlower/SunFlower2.gif")
{

}

void SunFlowerInstance::initTrigger()
{
    (new Timer(picture, 5000, [this] {
        QSharedPointer<std::function<void(void)> > generateSun(new std::function<void(void)>);
        *generateSun = [this, generateSun] {
            picture->setMovieOnNewLoop(lightedGif, [this, generateSun] {
                (new Timer(picture, 1000, [this, generateSun] {
                    auto sunGifAndOnFinished = plantProtoType->scene->newSun(25);
                    MoviePixmapItem *sunGif = sunGifAndOnFinished.first;
                    std::function<void(bool)> onFinished = sunGifAndOnFinished.second;
                    Coordinate &coordinate = plantProtoType->scene->getCoordinate();
                    double fromX = coordinate.getX(col) - sunGif->boundingRect().width() / 2 + 15,
                            toX = coordinate.getX(col) - qrand() % 80,
                            toY = coordinate.getY(row) - sunGif->boundingRect().height();
                    sunGif->setScale(0.6);
                    sunGif->setPos(fromX, toY - 25);
                    sunGif->start();
                    Animate(sunGif).move(QPointF((fromX + toX) / 2, toY - 50)).scale(0.9).speed(0.2).shape(
                                    QTimeLine::EaseOutCurve).finish()
                            .move(QPointF(toX, toY)).scale(1.0).speed(0.2).shape(QTimeLine::EaseInCurve).finish(
                                    onFinished);
                    picture->setMovieOnNewLoop(plantProtoType->normalGif, [this, generateSun] {
                        (new Timer(picture, 24000, [this, generateSun] {
                            (*generateSun)();
                        }))->start();
                    });
                }))->start();
            });
        };
        (*generateSun)();
    }))->start();
}

WallNut::WallNut()              //?????????
{
    eName = "oWallNut";
    cName = tr("Wall-nut");
    hp = 4000;
    beAttackedPointR = 45;
    sunNum = 0;
    //coolTime = 30;
    cardGif = "Card/Plants/WallNut.png";
    staticGif = "Plants/WallNut/0.gif";
    normalGif = "Plants/WallNut/WallNut.gif";
    toolTip = tr("Stops zombies with its chewy shell");
}

bool WallNut::canGrow(int x, int y) const   //?????????????????????????????????????????????
{
   if (x < 1 || x > 9 || y < 1 || y > 6)
        return false;
    if (scene->isCrater(x, y) || scene->isTombstone(x, y))
        return false;
    int groundType = scene->getGameLevelData()->LF[y];
    QMap<int, PlantInstance *> plants = scene->getPlant(x, y);
    if (groundType == 1||groundType == 2){
        if(scene->coord==1&&(y==3||y==4)&&!plants.contains(1)){
            return false;
        }
        else {
            return !plants.contains(1) || plants[1]->plantProtoType->eName == "oLilyPad"||plants[1]->plantProtoType->eName == "oWallNut";
        }
    }
    return plants.contains(0) && (!plants.contains(1)||plants[1]->plantProtoType->eName == "oLilyPad"||plants[1]->plantProtoType->eName == "oWallNut");
}

TallNut::TallNut()
{
    eName = "oTallNut";
    cName = tr("Tall-nut");
    hp = 8000;
    beAttackedPointR = 45;
    sunNum = 0;
    //coolTime = 30;
    cardGif = "Card/Plants/TallNut.png";
    staticGif = "Plants/TallNut/0.gif";
    normalGif = "Plants/TallNut/TallNut.gif";
    toolTip = tr("?????????????????????");
}

bool TallNut::canGrow(int x, int y) const
{
   if (x < 1 || x > 9 || y < 1 || y > 6)
        return false;
    if (scene->isCrater(x, y) || scene->isTombstone(x, y))
        return false;
    int groundType = scene->getGameLevelData()->LF[y];
    QMap<int, PlantInstance *> plants = scene->getPlant(x, y);
    if (groundType == 1||groundType == 2){
        if(scene->coord==1&&(y==3||y==4)&&!plants.contains(1)){
            return false;
        }
        else {
            return !plants.contains(1) || plants[1]->plantProtoType->eName == "oLilyPad"||plants[1]->plantProtoType->eName == "oTallNut";
        }
    }
    return plants.contains(0) && (!plants.contains(1)||plants[1]->plantProtoType->eName == "oLilyPad"||plants[1]->plantProtoType->eName == "oTallNut");
}

Torchwood::Torchwood(){         //????????????
    eName="oTorchwood";
    cName=tr("Torchwood");
    beAttackedPointR = 45;
    sunNum = 0;
    cardGif = "Card/Plants/Torchwood.png";
    staticGif = "Plants/Torchwood/0.gif";
    normalGif = "Plants/Torchwood/Torchwood.gif";
    toolTip = tr("??????????????????????????????????????????????????????????????????");
}

CherryBomb::CherryBomb(){
    eName="oCherryBomb";
    cName=tr("CherryBomb");
    beAttackedPointR=45;
    sunNum=0;
    //coolTime=30;                                        //??????????????????7.5
    cardGif = "Card/Plants/CherryBomb.png";
    staticGif = "Plants/CherryBomb/0.gif";
    normalGif = "Plants/CherryBomb/CherryBomb.gif";
    toolTip = tr("?????????????????????????????????????????????");
}

LilyPad::LilyPad(){
    eName="oLilyPad";
    cName=tr("LilyPad");
    beAttackedPointR=45;
    sunNum=0;
    cardGif = "Card/Plants/LilyPad.png";
    staticGif = "Plants/LilyPad/0.gif";
    normalGif = "Plants/LilyPad/LilyPad.gif";
    toolTip = tr("??????????????????????????????");
    //height-=40;
}

bool LilyPad::canGrow(int x, int y) const
{
    if (x < 1 || x > 9 || y < 1 || y > 6)//5
        return false;
    if (scene->isCrater(x, y) || scene->isTombstone(x, y))
        return false;
    if(scene->coord==1&&(y==1||y==2||y==5||y==6)){
        return false;
    }
    int groundType = scene->getGameLevelData()->LF[y];
    QMap<int, PlantInstance *> plants = scene->getPlant(x, y);
    if (groundType == 1||groundType == 2){
        return !plants.contains(1);
    }

    return plants.contains(0) && !plants.contains(1);
}
void LilyPadInstance::initTrigger(){}
LilyPadInstance::LilyPadInstance(const Plant *plant)
    : PlantInstance(plant)
{}

LawnCleaner::LawnCleaner()
{
    eName = "oLawnCleaner";
    cName = tr("Lawn Cleaner");
    beAttackedPointL = 0;
    beAttackedPointR = 71;
    sunNum = 0;
    staticGif = normalGif = "interface/LawnCleaner.png";
    canEat = 0;
    stature = 1;
    toolTip = tr("Normal lawn cleaner");
}

PoolCleaner::PoolCleaner()
{
//    eName = "oPoolCleaner";
//    cName = tr("Pool Cleaner");
//    beAttackedPointR = 47;
//    staticGif = normalGif = "interface/PoolCleaner.png";
//    toolTip = tr("Pool Cleaner");
//    update();
    eName = "oPoolCleaner";
    cName = tr("Pool Cleaner");
    beAttackedPointL = 0;
    beAttackedPointR = 71;
    sunNum = 0;
    staticGif = normalGif = "interface/PoolCleaner.png";
    canEat = 0;
    stature = 1;
    toolTip = tr("Normal pool cleaner");
}


void WallNutInstance::initTrigger()
{}

WallNutInstance::WallNutInstance(const Plant *plant)
    : PlantInstance(plant)
{
    hurtStatus = 0;
    crackedGif1 = "Plants/WallNut/Wallnut_cracked1.gif";
    crackedGif2 = "Plants/WallNut/Wallnut_cracked2.gif";
}

void WallNutInstance::getHurt(ZombieInstance *zombie, int aKind, int attack)
{
    PlantInstance::getHurt(zombie, aKind, attack);
    if (hp > 0) {
        if (hp < 1334) {
            if (hurtStatus < 2) {
                hurtStatus = 2;
                picture->setMovie(crackedGif2);
                picture->start();
            }
        }
        else if (hp < 2667) {
            if (hurtStatus < 1) {
                hurtStatus = 1;
                picture->setMovie(crackedGif1);
                picture->start();
            }
        }
    }
}

PotatoMine::PotatoMine()
{
    eName = "oPotatoMine";
    cName = tr("Potato Mine");
    beAttackedPointR = 50;  //????????????
    sunNum = 00;
    //coolTime = 30;
    cardGif = "Card/Plants/PotatoMine.png";
    staticGif = "Plants/PotatoMine/0.gif";      //??????
    normalGif = "Plants/PotatoMine/PotatoMineNotReady.gif"; //????????????
    toolTip = tr("??????????????????????????????????????????");
}

PotatoMineInstance::PotatoMineInstance(const Plant *plant)
    : PlantInstance(plant)
{
    //??????????????????????????????
    explosionSpudowGif = "Plants/PotatoMine/ExplosionSpudow.gif";
    mashedGif = "Plants/PotatoMine/PotatoMine_mashed.gif";
}

void PotatoMineInstance::initTrigger(){
    //??????????????????????????????????????????15???
    (new Timer(picture, 500, [this] {
        picture->setMovie("Plants/PotatoMine/PotatoMine.gif");

        //Trigger??????GameSence??????????????????????????????????????????????????????from???to????????????ID
        //??????????????????
        Trigger *trigger = new Trigger(this, attackedLX, attackedRX+30, 0, 0);
        //??????????????????????????????????????????
        triggers.insert(row, QList<Trigger *>{ trigger } );
        //??????????????????????????????
        plantProtoType->scene->addTrigger(row, trigger);
    }))->start();
}

//????????????
void PotatoMineInstance::triggerCheck(ZombieInstance *zombieInstance, Trigger *trigger)//??????
{
    if (zombieInstance->altitude > 0) {
        normalAttack();

        //gethurt???akind?????????1??????????????????????????????????????????
        getHurt(zombieInstance, 1, 300);
    }
}

void PotatoMineInstance::normalAttack(){
    //????????????????????????
    MoviePixmapItem* explosionPixmap = new MoviePixmapItem;
    //??????
    explosionPixmap->setPos(picture->pos());
    //??????
    explosionPixmap->setZValue(picture->zValue()+1);
    //??????
    explosionPixmap->setMovie(explosionSpudowGif);
    //??????
    explosionPixmap->start();
    //????????????mashed?????????
    MoviePixmapItem* mashedPixmap = new MoviePixmapItem;
    mashedPixmap->setPos(picture->pos());
    mashedPixmap->setZValue(picture->zValue());
    mashedPixmap->setMovie(mashedGif);
    mashedPixmap->start();
    //???????????????????????????????????????
    plantProtoType->scene->addToGame(explosionPixmap);
    plantProtoType->scene->addToGame(mashedPixmap);
    //?????????????????????
    QMediaPlayer *boomMusic = new QMediaPlayer(plantProtoType->scene);
    boomMusic->setMedia(QUrl("qrc:/audio/potato_mine.mp3"));
    boomMusic->play();
    //???????????????????????????????????????????????????2.5???
    (new Timer(plantProtoType->scene, 2500, [explosionPixmap, mashedPixmap, boomMusic] {
        explosionPixmap->deleteLater();
        mashedPixmap->deleteLater();
        boomMusic->deleteLater();
    }))->start();
    Coordinate &coordinate = plantProtoType->scene->getCoordinate();
    //??????????????????X??????
    double x = coordinate.getX(col);
    //??????????????????????????????
    QList<ZombieInstance *> zombies = plantProtoType->scene->getZombieOnRow(row);
    //????????????160???????????????????????????????????????
    for (auto iter = zombies.begin(); iter != zombies.end();iter++) {
        if ((*iter)->hp > 0 && (*iter)->attackedRX >= x-80 && (*iter)->attackedLX <= x+80) {
            (*iter)->getBoom(1800);
        }
    }
}

TallNutInstance::TallNutInstance(const Plant *plant)
    : WallNutInstance(plant)
{
    canVaulted = false;                         //????????????????????????
    hurtPoint1 = 5333;
    hurtPoint2 = 2667;
    crackedGif1 = "Plants/TallNut/TallnutCracked1.gif";
    crackedGif2 = "Plants/TallNut/TallnutCracked2.gif";
}

void TorchwoodInstance::initTrigger(){}                                             //??????
TorchwoodInstance::TorchwoodInstance(const Plant *plant):PlantInstance (plant){}

CherryBombInstance::CherryBombInstance(const Plant *plant)
    :PlantInstance (plant){
    BoomGif="Plants/CherryBomb/Boom.gif";
}
void CherryBombInstance::initTrigger(){
    //??????????????????
    (new Timer(picture,700,[this]{
        normalAttack();             //??????????????????
        getHurt(nullptr,1,300);
    }))->start();
}
void CherryBombInstance::triggerCheck(ZombieInstance *zombieInstance,Trigger *trigger){}    //????????????

void CherryBombInstance::normalAttack(){
    //????????????????????????
    MoviePixmapItem* boomPixmap = new MoviePixmapItem;
    //????????????????????????
    boomPixmap->setPos(picture->pos()+QPointF(-10,-25));
    //??????
    boomPixmap->setZValue(picture->zValue());
    //??????
    boomPixmap->setMovie(BoomGif);                                              //??????????????????????????????
    //??????
    boomPixmap->start();
    //??????????????????
    plantProtoType->scene->addToGame(boomPixmap);
    //??????????????????
    QMediaPlayer *boomMusic = new QMediaPlayer(plantProtoType->scene);
    //????????????
    boomMusic->setMedia(QUrl("qrc:/audio/explosion.mp3"));                      //????????????
    //????????????
    boomMusic->play();
    //2.5s????????????????????????
    (new Timer(plantProtoType->scene, 2500, [boomPixmap, boomMusic] {
        boomPixmap->deleteLater();                                              //???????????????????????????????????????
        boomMusic->deleteLater();
    }))->start();
    //???????????????????????????
    Coordinate &coordinate = plantProtoType->scene->getCoordinate();
    //??????X??????
    double x = coordinate.getX(col);
    //??????????????????????????????????????????
    for (int r = row>1?row-1:1; r <= coordinate.rowCount() && r <= row + 1; ++r) {
        QList<ZombieInstance *> zombies = plantProtoType->scene->getZombieOnRow(r);
        for (auto iter = zombies.begin(); iter != zombies.end();iter++) {
            //??????????????????X??????????????????????????????????????????
            if ((*iter)->hp > 0 && (*iter)->attackedRX >= x-120 && (*iter)->attackedLX <= x+120) {
                (*iter)->getBoom(1800);
            }
        }
    }
}
LawnCleanerInstance::LawnCleanerInstance(const Plant *plant)
    : PlantInstance(plant)
{}

void LawnCleanerInstance::initTrigger()
{
    Trigger *trigger = new Trigger(this, attackedLX, attackedRX, 0, 0);
    triggers.insert(row, QList<Trigger *>{ trigger } );
    plantProtoType->scene->addTrigger(row, trigger);
}

void LawnCleanerInstance::triggerCheck(ZombieInstance *zombieInstance, Trigger *trigger)
{
    if (zombieInstance->beAttacked && zombieInstance->altitude > 0) {
        canTrigger = 0;
        normalAttack(nullptr);
    }
}

void LawnCleanerInstance::normalAttack(ZombieInstance *zombieInstance)
{
    QMediaPlayer *player = new QMediaPlayer(plantProtoType->scene);
    player->setMedia(QUrl("qrc:/audio/lawnmower.mp3"));
    player->play();
    QSharedPointer<std::function<void(void)> > crush(new std::function<void(void)>);
    *crush = [this, crush] {
        for (auto zombie: plantProtoType->scene->getZombieOnRowRange(row, attackedLX, attackedRX)) {
            zombie->crushDie();
        }
        if (attackedLX > 900)
            plantProtoType->scene->plantDie(this);
        else {
            attackedLX += 10;
            attackedRX += 10;
            picture->setPos(picture->pos() + QPointF(10, 0));
            (new Timer(picture, 10, *crush))->start();
        }
    };
    (*crush)();
}

//???????????????

PoolCleanerInstance::PoolCleanerInstance(const Plant *plant)
    : PlantInstance(plant)
{}

void PoolCleanerInstance::initTrigger()//?????????
{
    //qDebug()<<"111gaga";
    Trigger *trigger = new Trigger(this, attackedLX, attackedRX, 0, 0);
    triggers.insert(row, QList<Trigger *>{ trigger } );
    plantProtoType->scene->addTrigger(row, trigger);
}

void PoolCleanerInstance::triggerCheck(ZombieInstance *zombieInstance, Trigger *trigger)
{
    //qDebug()<<"222gaga";
    if (zombieInstance->beAttacked && zombieInstance->altitude > 0) {
        canTrigger = 0;
        normalAttack(nullptr);
    }
}

void PoolCleanerInstance::normalAttack(ZombieInstance *zombieInstance)
{
    QMediaPlayer *player = new QMediaPlayer(plantProtoType->scene);
    player->setMedia(QUrl("qrc:/audio/pool_cleaner.mp3"));
    player->play();
    QSharedPointer<std::function<void(void)> > crush(new std::function<void(void)>);
    //qDebug()<<"331gaga";
    *crush = [this, crush] {
        //qDebug()<<"332gaga";
        for (auto zombie: plantProtoType->scene->getZombieOnRowRange(row, attackedLX, attackedRX)) {
            zombie->crushDie();
        }
        if (attackedLX > 900)
            plantProtoType->scene->plantDie(this);
        else {
            attackedLX += 10;
            attackedRX += 10;
            picture->setPos(picture->pos() + QPointF(10, 0));
            (new Timer(picture, 10, *crush))->start();
        }
    };
    (*crush)();
}


PeashooterInstance::PeashooterInstance(const Plant *plant)
    : PlantInstance(plant), firePea(new QMediaPlayer(picture))
{
    firePea->setMedia(QUrl("qrc:/audio/firepea.mp3"));
}

void PeashooterInstance::normalAttack(ZombieInstance *zombieInstance)
{
    firePea->play();
    (new Bullet(plantProtoType->scene, 0, row, attackedLX, attackedLX - 40, picture->y() + 3, picture->zValue() + 2, 0))->start();
}

RepeaterInstance::RepeaterInstance(const Plant *plant)
    :PeashooterInstance(plant),firePea(new QMediaPlayer(picture))
{
    firePea->setMedia(QUrl("qrc:/audio/firepea.mp3"));
}

void RepeaterInstance::normalAttack(ZombieInstance *zombieInstance){
    firePea->play();
    (new Timer(picture, 200, [this] {
        (new Bullet(plantProtoType->scene, 0, row, attackedLX, attackedLX - 40, picture->y() + 3, picture->zValue() + 2, 0))->start();
    }))->start();
    (new Bullet(plantProtoType->scene, 0, row, attackedLX, attackedLX - 40, picture->y() + 3, picture->zValue() + 2, 0))->start();
}

GatlingPeaInstance::GatlingPeaInstance(const Plant *plant)
    : PeashooterInstance(plant),firePea(new QMediaPlayer(picture))
{
    firePea->setMedia(QUrl("qrc:/audio/firepea.mp3"));
}

void GatlingPeaInstance::normalAttack(ZombieInstance *zombieInstance)
{
    firePea->play();
    (new Timer(picture, 510, [this] {
        (new Bullet(plantProtoType->scene, 0, row, attackedLX, attackedLX - 40, picture->y() + 3, picture->zValue() + 2, 0))->start();
    }))->start();
    (new Timer(picture, 340, [this] {
        (new Bullet(plantProtoType->scene, 0, row, attackedLX, attackedLX - 40, picture->y() + 3, picture->zValue() + 2, 0))->start();
    }))->start();
    (new Timer(picture, 170, [this] {
        (new Bullet(plantProtoType->scene, 0, row, attackedLX, attackedLX - 40, picture->y() + 3, picture->zValue() + 2, 0))->start();
    }))->start();
    (new Bullet(plantProtoType->scene, 0, row, attackedLX, attackedLX - 40, picture->y() + 3, picture->zValue() + 2, 0))->start();
}

SnowPeaInstance::SnowPeaInstance(const Plant *plant)
    : PeashooterInstance(plant), firePea(new QMediaPlayer(picture)){
    firePea->setMedia(QUrl("qrc:/audio/firepea.mp3"));
}

void SnowPeaInstance::normalAttack(ZombieInstance *zombieInstance){
    firePea->play();
    (new Bullet(plantProtoType->scene, -1, row, attackedLX, attackedLX - 40, picture->y() + 3, picture->zValue() + 2, 0))->start();
}

Bullet::Bullet(GameScene *scene, int type, int row, qreal from, qreal x, qreal y, qreal zvalue, int direction)
        : scene(scene), type(type), row(row), direction(direction), from(from),beFired(false)
{
    count = 0;
    picture = new QGraphicsPixmapItem(gImageCache->load(QString("Plants/PB%1%2.gif").arg(type).arg(direction)));
    picture->setPos(x, y);
    picture->setZValue(zvalue);
}

Bullet::~Bullet()
{
    delete picture;
}

void Bullet::start()
{
    (new Timer(scene, 10, [this] {
        move();
    }))->start();
}

void Bullet::move()
{
    if (count++ == 10)
        scene->addItem(picture);
    int col = scene->getCoordinate().getCol(from);
    QMap<int, PlantInstance *> plants = scene->getPlant(col, row);
    if (!beFired&&type < 1 && plants.contains(1) && plants[1]->plantProtoType->eName == "oTorchwood") {
        ++type;
        beFired=true;
        //0.2s?????????befired????????????????????????????????????
        (new Timer(scene,200,[this]{
            beFired=false;
        }))->start();
        //setPixmap??????????????????
        //????????????
        picture->setPixmap(gImageCache->load(QString("Plants/PB%1%2.gif").arg(type).arg(direction)));
    }
    ZombieInstance *zombie = nullptr;
    if (direction == 0) {
        QList<ZombieInstance *> zombies = scene->getZombieOnRow(row);
        for (auto iter = zombies.end(); iter-- != zombies.begin() && (*iter)->attackedLX <= from;) {
            if ((*iter)->hp > 0 && (*iter)->attackedRX >= from) {
                zombie = *iter;
                break;
            }
        }
    }
    // TODO: another direction
    if (zombie && zombie->altitude == 1) {
        // TODO: other attacks
        if(type==1)zombie->getPea(40, direction,1);
        else if(type==0)zombie->getPea(20, direction,0);
        else if(type==-1)zombie->getPea(20, direction,-1);
        picture->setPos(picture->pos() + QPointF(28, 0));
        picture->setPixmap(gImageCache->load("Plants/PeaBulletHit.gif"));
        (new Timer(scene, 100, [this] {
            delete this;
        }))->start();
    }
    else {
        from += direction ? -5 : 5;
        if (from < 900 && from > 100) {
            picture->setPos(picture->pos() + QPointF(direction ? -5 : 5, 0));
            (new Timer(scene, 10, [this] {
                move();
            }))->start();
        }
        else
            delete this;
    }
}

Plant *PlantFactory(GameScene *scene, const QString &eName)
{
    Plant *plant = nullptr;
    if (eName == "oPeashooter")
        plant = new Peashooter;
    else if (eName == "oRepeater")
        plant=new Repeater;
    else if (eName == "oGatlingPea")
        plant=new GatlingPea;
    else if (eName == "oSnowPea")
        plant = new SnowPea;
    else if (eName == "oSunflower")
        plant = new SunFlower;
    else if(eName == "oPotatoMine")
        plant = new PotatoMine;
    else if (eName == "oWallNut")
        plant = new WallNut;
   else if (eName == "oTallNut")
        plant = new TallNut;
    else if (eName=="oTorchwood")
        plant = new Torchwood;
    else if (eName =="oCherryBomb")
        plant = new CherryBomb;
    else if (eName =="oLilyPad")
        plant = new LilyPad;
    else if (eName == "oLawnCleaner")
        plant = new LawnCleaner;
    else if (eName == "oPoolCleaner")
        plant = new PoolCleaner;
    if (plant) {
        plant->scene = scene;
        plant->update();
    }
    return plant;
}

PlantInstance *PlantInstanceFactory(const Plant *plant)
{
    if (plant->eName == "oPeashooter")
        return new PeashooterInstance(plant);
    if  (plant->eName == "oRepeater")
        return new RepeaterInstance(plant);
    if (plant->eName == "oGatlingPea")
        return new GatlingPeaInstance(plant);
    if (plant->eName == "oSnowPea")
        return new SnowPeaInstance(plant);
    if (plant->eName == "oSunflower")
        return new SunFlowerInstance(plant);
    if(plant->eName == "oPotatoMine")
        return new PotatoMineInstance(plant);
    if (plant->eName == "oWallNut")
        return new WallNutInstance(plant);
    if(plant->eName == "oTallNut")
        return new TallNutInstance(plant);
    if(plant->eName == "oTorchwood")
        return new TorchwoodInstance(plant);
    if(plant->eName=="oCherryBomb")
        return new CherryBombInstance(plant);
    if(plant->eName=="oLilyPad")
        return new LilyPadInstance(plant);
    if (plant->eName == "oLawnCleaner")
        return new LawnCleanerInstance(plant);
    if (plant->eName == "oPoolCleaner")
        return new PoolCleanerInstance(plant);
    return new PlantInstance(plant);
}



