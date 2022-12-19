//
// Created by sun on 8/26/16.
//

#include <QtAlgorithms>
#include <stdlib.h>
#include <math.h>
#include "GameScene.h"
#include "MainView.h"
#include "ImageManager.h"
#include "Timer.h"
#include "Plant.h"
#include "Zombie.h"
#include "GameLevelData.h"
#include "MouseEventPixmapItem.h"
#include "PlantCardItem.h"
#include "Animate.h"
#include <QtMultimedia>

bool exitNow = false;       //退出标志位

GameScene::GameScene(GameLevelData *gameLevelData)
        : QGraphicsScene(0, 0, 900, 600),
          gameLevelData(gameLevelData),
          background(new QGraphicsPixmapItem(gImageCache->load(gameLevelData->backgroundImage))),
          gameGroup(new QGraphicsItemGroup),
          infoText(new QGraphicsSimpleTextItem),
          infoTextGroup(new QGraphicsRectItem(0, 0, 900, 50)),
          menuGroup(new MouseEventPixmapItem(gImageCache->load("interface/Button.png"))),
          sunNumText(new QGraphicsSimpleTextItem(QString::number(gameLevelData->sunNum))),
          sunNumGroup(new QGraphicsPixmapItem(gImageCache->load("interface/SunBack.png"))),
          selectCardButtonReset(new MouseEventPixmapItem(gImageCache->load("interface/SelectCardButton.png"))),
          selectCardButtonOkay(new MouseEventPixmapItem(gImageCache->load("interface/SelectCardButton.png"))),
          selectCardTextReset(new QGraphicsSimpleTextItem(tr("Reset"))),
          selectCardTextOkay(new QGraphicsSimpleTextItem(tr("Go"))),
          selectingPanel(new QGraphicsPixmapItem(gImageCache->load("interface/SeedChooser_Background.png"))),
          cardPanel(new QGraphicsItemGroup),
          shovel(new QGraphicsPixmapItem(gImageCache->load("interface/Shovel.png"))),
          shovelBackground(new QGraphicsPixmapItem(gImageCache->load("interface/ShovelBack.png"))),
          movePlantAlpha(new QGraphicsPixmapItem),
          movePlant(new QGraphicsPixmapItem),
          imgGrowSoil(new MoviePixmapItem("interface/GrowSoil.gif")),
          imgGrowSpray(new MoviePixmapItem("interface/GrowSpray.gif")),
          flagMeter(new FlagMeter(gameLevelData)),
          backgroundMusic(new QMediaPlayer(this)),
          tapMusic(new QMediaPlayer(this)),
          sunMusic(new QMediaPlayer(this)),
          shovelMusic(new QMediaPlayer(this)),
          seedliftMusic(new QMediaPlayer(this)),
          plantMusic1(new QMediaPlayer(this)),
          plantMusic2(new QMediaPlayer(this)),
          groanMusic(new QMediaPlayer(this)),
          waveMusic(new QMediaPlayer(this)),
          coordinate(gameLevelData->coord),
          choose(0), sunNum(gameLevelData->sunNum),
          waveTimer(nullptr), waveNum(0)

{

    //退出标志位初始化
    exitNow = false;
    //定义游戏状态变量，判断是否在游戏中
    //为0表示在游戏中，可以获胜
    gameStatus = 0;
    //coord初始化
    coord=gameLevelData->coord;


    // Process ProtoTypes
    for (const auto &eName: gameLevelData->pName)
        plantProtoTypes.insert(eName, PlantFactory(this, eName));//添加植物种类
    for (const auto &eName: gameLevelData->zName)
        zombieProtoTypes.insert(eName, ZombieFactory(this, eName));//添加僵尸种类
    // z-value -- 0: normal 1: tooltip 2: dialog
    // Background (parent of the zombies displayed on the road)
    //开局显示僵尸
    addItem(background);
    if (gameLevelData->showScroll) {
        QList<qreal> yPos;
        QList<Zombie *> zombies;
        for (const auto &zombieData: gameLevelData->zombieData) {
            Zombie *item = getZombieProtoType(zombieData.eName);    //僵尸的名字
            //加载僵尸种类
            if(item->canDisplay) {
                for (int i = 0; i < zombieData.num; ++i) {          //僵尸的个数
                    yPos.push_back(qFloor(100 +  qrand() % 400));
                    zombies.push_back(item);
                }
            }
        }
        qSort(yPos.begin(), yPos.end());
        std::random_shuffle(zombies.begin(), zombies.end());//随机排序
        for (int i = 0; i < zombies.size(); ++i) {
            MoviePixmapItem *pixmap = new MoviePixmapItem(zombies[i]->standGif);
            QSizeF size = pixmap->boundingRect().size();
            pixmap->setPos(qFloor(1115 + qrand() % 200) - size.width() * 0.5, yPos[i] - size.width() * 0.5);
            pixmap->setParentItem(background);//将项目添加到背景项目组
        }
    }
    // Plants, zombies and sun
    //植物，僵尸，阳光
    gameGroup->setHandlesChildEvents(false);
    addItem(gameGroup);
    // Information text
    infoText->setBrush(Qt::white);
    infoText->setFont(QFont("SimHei", 16, QFont::Bold));
    infoText->setParentItem(infoTextGroup);
    infoTextGroup->setPos(0, 500);
    infoTextGroup->setPen(Qt::NoPen);
    infoTextGroup->setBrush(QColor::fromRgb(0x5b432e));
    infoTextGroup->setOpacity(0);
    addItem(infoTextGroup);
    // Menu
    QGraphicsSimpleTextItem *menuText = new QGraphicsSimpleTextItem(tr("Menu"));
    menuText->setBrush(QColor::fromRgb(0x00cb08));
    menuText->setFont(QFont("SimHei", 12, QFont::Bold));
    menuText->setParentItem(menuGroup);
    menuText->setPos(sizeToPoint(menuGroup->boundingRect().size() - menuText->boundingRect().size()) / 2);
    menuGroup->setPos(sceneRect().topRight() - sizeToPoint(menuGroup->boundingRect().size()));
    menuGroup->setCursor(Qt::PointingHandCursor);
    addItem(menuGroup);
    // Sun number


    //退出游戏按钮（退出到菜单）
    connect(menuGroup,&MouseEventPixmapItem::clicked,[this]{pauseGame();});


    sunNumText->setFont(QFont("Verdana", 16, QFont::Bold));
    QSizeF sunNumTextSize = sunNumText->boundingRect().size();
    sunNumText->setPos(76 - sunNumTextSize.width() / 2,
                       (sunNumGroup->boundingRect().height() - sunNumTextSize.height()) / 2);
    sunNumText->setParentItem(sunNumGroup);
    sunNumGroup->setPos(100, 560);
    sunNumGroup->setVisible(false);
    addItem(sunNumGroup);



    // Select Card
    //选择卡片
    if (gameLevelData->canSelectCard && gameLevelData->maxSelectedCards > 0) {
        // Title
        //标题
        QGraphicsSimpleTextItem *selectCardTitle = new QGraphicsSimpleTextItem(tr("Choose your cards"));
        selectCardTitle->setBrush(QColor::fromRgb(0xf0c060));
        selectCardTitle->setFont(QFont("NSimSun", 12, QFont::Bold));
        QSizeF selectCardTitleSize = selectCardTitle->boundingRect().size();
        selectCardTitle->setPos((selectingPanel->boundingRect().width() - selectCardTitleSize.width()) / 2,
                                15 - selectCardTitleSize.height() / 2);
        selectCardTitle->setParentItem(selectingPanel);
        // Reset button
        //重置
        selectCardTextReset->setBrush(QColor::fromRgb(0x808080));
        selectCardTextReset->setFont(QFont("SimHei", 12, QFont::Bold));
        selectCardTextReset->setPos(sizeToPoint(selectCardButtonReset->boundingRect().size()
                                                - selectCardTextReset->boundingRect().size()) / 2);
        selectCardTextReset->setParentItem(selectCardButtonReset);
        selectCardButtonReset->setPos(162, 500);    //设置按钮坐标
        selectCardButtonReset->setEnabled(false);
        selectCardButtonReset->setParentItem(selectingPanel);
        // Okay button
        //确认
        selectCardTextOkay->setBrush(QColor::fromRgb(0x808080));
        selectCardTextOkay->setFont(QFont("SimHei", 12, QFont::Bold));
        selectCardTextOkay->setPos(sizeToPoint(selectCardButtonOkay->boundingRect().size()
                                               - selectCardTextOkay->boundingRect().size()) / 2);
        selectCardTextOkay->setParentItem(selectCardButtonOkay);
        selectCardButtonOkay->setPos(237, 500);     //设置按钮坐标
        selectCardButtonOkay->setEnabled(false);
        selectCardButtonOkay->setParentItem(selectingPanel);
        // Plant cards to select
        //植物卡片
        int cardIndex = 0;
        for (auto item: plantProtoTypes.values()) {
            if (!item->canSelect) continue;
            // Plant cards
            //创建卡片实例
            PlantCardItem *plantCardItem = new PlantCardItem(item, true);
            plantCardItem->setPos(15 + cardIndex % 6 * 72, 40 + cardIndex / 6 * 50);
            plantCardItem->setCursor(Qt::PointingHandCursor);
            plantCardItem->setParentItem(selectingPanel);
            // Tooltip
            //提示栏
            QString tooltipText = "<b>" + item->cName + "</b><br />" +
                    QString(tr("Cool down: %1s")).arg(item->coolTime) + "<br />";
            if (gameLevelData->dKind != 0 && item->night)
                tooltipText += "<span style=\"color:#F00\">" + tr("Nocturnal - sleeps during day") + "</span><br>";
            tooltipText += item->toolTip;
            TooltipItem *tooltipItem = new TooltipItem(tooltipText);
            tooltipItem->setVisible(false);
            tooltipItem->setOpacity(0.9);
            tooltipItem->setZValue(1);
            addItem(tooltipItem);
            // Displaying tooltip when hovering
            QPointF posDelta(5, 15);
            connect(plantCardItem, &PlantCardItem::hoverEntered, [tooltipItem, posDelta](QGraphicsSceneHoverEvent *event) {
                tooltipItem->setPos(event->scenePos() + posDelta);
                tooltipItem->setVisible(true);
            });
            connect(plantCardItem, &PlantCardItem::hoverMoved, [tooltipItem, posDelta](QGraphicsSceneHoverEvent *event) {
                tooltipItem->setPos(event->scenePos() + posDelta);
            });
            connect(plantCardItem, &PlantCardItem::hoverLeft, [tooltipItem, posDelta](QGraphicsSceneHoverEvent *event) {
                tooltipItem->setPos(event->scenePos() + posDelta);
                tooltipItem->setVisible(false);
            });
            // Move and scale to selected card panel when clicking
            connect(plantCardItem, &PlantCardItem::clicked, [this, item, plantCardItem] {
                // Check
                if (!plantCardItem->isChecked()) return;
                tapMusic->stop();
                tapMusic->play();
                int count = selectedPlantArray.size();
                if (this->gameLevelData->maxSelectedCards > 0 && count >= this->gameLevelData->maxSelectedCards)
                    return;
                // Okay
                plantCardItem->setChecked(false);
                PlantCardItem *selectedPlantCardItem = new PlantCardItem(item, true);
                selectedPlantCardItem->setPos(plantCardItem->scenePos());
                cardPanel->addToGroup(selectedPlantCardItem);
                selectedPlantArray.push_back(item);
                if (count == 0) {
                    selectCardTextReset->setBrush(QColor::fromRgb(0xf0c060));
                    selectCardTextOkay->setBrush(QColor::fromRgb(0xf0c060));
                    selectCardButtonReset->setEnabled(true);
                    selectCardButtonOkay->setEnabled(true);
                    selectCardButtonReset->setCursor(Qt::PointingHandCursor);
                    selectCardButtonOkay->setCursor(Qt::PointingHandCursor);
                }
                Animate(selectedPlantCardItem).move(QPointF(0, 60 * count)).scale(1).speed(1).replace().finish();
                // Move and scale back to unselected card panel when clicking
                QSharedPointer<QMetaObject::Connection> deselectConnnection(new QMetaObject::Connection), resetConnnection(new QMetaObject::Connection);
                auto deselectFunctor = [this, item, plantCardItem, selectedPlantCardItem, deselectConnnection, resetConnnection] {
                    disconnect(*deselectConnnection);
                    disconnect(*resetConnnection);
                    cardPanel->removeFromGroup(selectedPlantCardItem);
                    selectedPlantArray.removeOne(item);
                    if (selectedPlantArray.size() == 0) {
                        selectCardTextReset->setBrush(QColor::fromRgb(0x808080));
                        selectCardTextOkay->setBrush(QColor::fromRgb(0x808080));
                        selectCardButtonReset->setEnabled(false);
                        selectCardButtonOkay->setEnabled(false);
                        selectCardButtonReset->setCursor(Qt::ArrowCursor);
                        selectCardButtonOkay->setCursor(Qt::ArrowCursor);
                    }
                    Animate(selectedPlantCardItem).move(plantCardItem->scenePos()).scale(0.7).speed(1).replace().finish(
                        [plantCardItem, selectedPlantCardItem] {
                            plantCardItem->setChecked(true);
                            delete selectedPlantCardItem;
                    });
                };
                *deselectConnnection = connect(selectedPlantCardItem, &PlantCardItem::clicked, [this, selectedPlantCardItem, deselectFunctor] {
                    tapMusic->stop();
                    tapMusic->play();
                    QList<QGraphicsItem *> selectedCards = cardPanel->childItems();
                    for (int i = qFind(selectedCards, selectedPlantCardItem) - selectedCards.begin() + 1; i != selectedCards.size(); ++i)
                        Animate(selectedCards[i]).move(QPointF(0, 60 * (i - 1))).speed(1).replace().finish();
                    deselectFunctor();
                });
                *resetConnnection = connect(selectCardButtonReset, &MouseEventPixmapItem::clicked, deselectFunctor);
            });
            ++cardIndex;
        }
        connect(selectCardButtonOkay, &MouseEventPixmapItem::clicked, [this] { tapMusic->stop(); tapMusic->play(); });
        connect(selectCardButtonReset, &MouseEventPixmapItem::clicked, [this] { tapMusic->stop(); tapMusic->play(); });
        selectingPanel->setPos(100, -selectingPanel->boundingRect().height());
        addItem(selectingPanel);
    }
    // Selected card
    cardPanel->setHandlesChildEvents(false);
    addItem(cardPanel);
    shovel->setPos(0, -5);
    shovel->setCursor(Qt::PointingHandCursor);
    shovel->setParentItem(shovelBackground);
    shovelBackground->setPos(235, -100);
    shovelBackground->setCursor(Qt::PointingHandCursor);
    shovelBackground->setZValue(1);
    addToGame(shovelBackground);
    // Move plant
    movePlantAlpha->setOpacity(0.4);
    movePlantAlpha->setVisible(false);
    movePlantAlpha->setZValue(30);
    gameGroup->addToGroup(movePlantAlpha);
    movePlant->setVisible(false);
    movePlant->setZValue(254);
    addItem(movePlant);
    // Effects for growing plants
    imgGrowSoil->setVisible(false);
    imgGrowSoil->setZValue(50);
    gameGroup->addToGroup(imgGrowSoil);
    imgGrowSpray->setVisible(false);
    imgGrowSpray->setZValue(50);
    gameGroup->addToGroup(imgGrowSpray);
    // Flag progress
    flagMeter->setPos(700, 610);
    addItem(flagMeter);

    connect(backgroundMusic, &QMediaPlayer::stateChanged, [this](QMediaPlayer::State state) {
        if (state == QMediaPlayer::StoppedState)
            backgroundMusic->play();
    });
    tapMusic->setMedia(QUrl("qrc:/audio/tap.mp3"));
    sunMusic->setMedia(QUrl("qrc:/audio/points.mp3"));
    shovelMusic->setMedia(QUrl("qrc:/audio/shovel.mp3"));
    seedliftMusic->setMedia(QUrl("qrc:/audio/seedlift.mp3"));
    plantMusic1->setMedia(QUrl("qrc:/audio/plant1.mp3"));
    plantMusic2->setMedia(QUrl("qrc:/audio/plant2.mp3"));
    // Plant Triggers & Zombie Rows
    for (int i = 0; i <= coordinate.rowCount(); ++i) {
        plantTriggers.push_back(QList<Trigger *>());
        zombieRow.push_back(QList<ZombieInstance *>());
    }
}

GameScene::~GameScene()
{
    for (int i = 0; i < coordinate.rowCount(); ++i) {
        for (auto item: plantTriggers[i])
            delete item;
    }
    for (auto i: plantProtoTypes.values())
        delete i;
    for (auto i: zombieProtoTypes.values())
        delete i;
    for (auto i: plantInstances)
        delete i;
    for (auto i: zombieInstances)
        delete i;

    delete gameLevelData;
}

void GameScene::setInfoText(const QString &text)
{
    if (text.isEmpty())
        Animate(infoTextGroup).fade(0).duration(200).finish();
    else {
        infoText->setText(text);
        infoText->setPos(sizeToPoint(infoTextGroup->boundingRect().size() - infoText->boundingRect().size()) / 2);
        Animate(infoTextGroup).fade(0.8).duration(200).finish();
    }
}

void GameScene::loadReady()
{
    gMainView->getMainWindow()->setWindowTitle(tr("Plants vs. Zombies") + " - " + gameLevelData->cName);
    if (!gameLevelData->showScroll)
        background->setPos(-115, 0);
    gameLevelData->loadAccess(this);
}

void GameScene::loadAcessFinished()
{
    if (!gameLevelData->showScroll || !gameLevelData->canSelectCard) {
        for (auto item: plantProtoTypes.values()) {
            if (item->canSelect) {
                selectedPlantArray.push_back(item);
                if (gameLevelData->maxSelectedCards > 0 && selectedPlantArray.size() >= gameLevelData->maxSelectedCards)
                    break;
            }
        }
    }
    if (gameLevelData->showScroll) {
        backgroundMusic->setMedia(QUrl("qrc:/audio/Look_up_at_the_Sky.mp3"));
        backgroundMusic->play();
        setInfoText(QString(tr("%1\' house")).arg(QSettings().value("Global/Username").toString()));
        (new Timer(this, 1000, [this]{
            setInfoText("");
            for (auto zombie: background->childItems())
                static_cast<MoviePixmapItem *>(zombie)->start();
            Animate(background).move(QPointF(-500, 0)).speed(0.5).finish([this] {
                Animate(menuGroup).move(QPointF(sceneRect().topRight() - QPointF(menuGroup->boundingRect().width(), 0))).speed(0.5).finish();
                auto scrollBack = [this] {
                    Animate(background).move(QPointF(-115, 0)).speed(0.5).finish([this] {
                        for (auto zombie: background->childItems())
                            delete zombie;
                        letsGo();
                    });
                };
                if (gameLevelData->canSelectCard) {
                    Animate(selectingPanel).move(QPointF(100, 0)).speed(3).finish([this] { sunNumGroup->setVisible(true); });
                    connect(selectCardButtonOkay, &MouseEventPixmapItem::clicked, [this, scrollBack] {
                        sunNumGroup->setVisible(false);
                        for (auto card: cardPanel->childItems()) {
                            card->setCursor(Qt::ArrowCursor);
                            card->setEnabled(false);
                        }
                        Animate(selectingPanel).move(QPointF(100, -selectingPanel->boundingRect().height())).speed(3).finish(scrollBack);
                    });
                }
                else {
                    (new Timer(this, 1000, scrollBack))->start();
                }
            });
        }))->start();
    }
    else {
        Animate(menuGroup).move(QPointF(sceneRect().topRight() - QPointF(menuGroup->boundingRect().width(), 0))).speed(0.5).finish();
        letsGo();
    }
}

void GameScene::pauseGame(){        //暂停按钮起退出游戏的作用    
    exitGame();
}

void GameScene::winGame(){          //赢游戏
    //QSettings().setValue用于读写配置文件
    //QString("%1").arg为格式化，在该语句中将String类型转换为int类型，加1再转换为String类型
    //QSettings().setValue("Global/NextLevel", QString("%1").arg(gameLevelData->eName.toInt()+1));
    exitGame();
}

void GameScene::loseGame(){         //输游戏
    //清除所有植物和僵尸
    for (auto i: plantInstances)
        plantDie(i);
    for (auto i: zombieInstances)
        zombieDie(i);
    //停止背景音乐
    backgroundMusic->stop();
        backgroundMusic->setMedia(QUrl("qrc:/audio/losemusic.mp3"));
        backgroundMusic->play();
        QMediaPlayer *screamMusic = new QMediaPlayer(this);
        screamMusic->play();
        //设置一个图层，图片是僵尸胜利的图片
        QGraphicsPixmapItem *imgLose = new QGraphicsPixmapItem(gImageCache->load("interface/ZombiesWon.png"));

        //参考准备安放植物（prepareGrowPlants）
        //使用QPointF将图画设置到中间
        QPointF pos = sizeToPoint(sceneRect().size() - imgLose->boundingRect().size()) / 2;
        //设置位置和高度
        imgLose->setPos(pos);
        imgLose->setZValue(1);
        addItem(imgLose);
        //5秒后停止图片和声音，退出游戏
        (new Timer(this, 5000, [this, imgLose, screamMusic] {
            delete imgLose;
            screamMusic->deleteLater();
            exitGame();
        }))->start();
//    });
}

void GameScene::exitGame(){         //退出游戏
    exitNow=true;
    backgroundMusic->stop();
    gMainView->switchToMenuScene();     //打开主菜单
    deleteLater();                      //清除空间（资源回收）
}

void GameScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseMoveEvent(mouseEvent);
    emit mouseMove(mouseEvent);
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mousePressEvent(mouseEvent);
    emit mousePress(mouseEvent);
}

GameLevelData *GameScene::getGameLevelData() const
{
    return gameLevelData;
}

void GameScene::letsGo()
{
    sunNumGroup->setPos(105, -sunNumGroup->boundingRect().height());
    sunNumGroup->setVisible(true);
    Animate(sunNumGroup).move(QPointF(105, 0)).speed(0.5).finish();
    if (gameLevelData->hasShovel)
        Animate(shovelBackground).move(QPointF(235, 0)).speed(0.5).finish();
    if (!gameLevelData->showScroll || !gameLevelData->canSelectCard) {
        cardPanel->setPos(-100, 0);
        Animate(cardPanel).move(QPointF(0, 0)).speed(0.5).finish();
    }
    for (auto i: cardPanel->childItems())
        delete i;
    for (int i = 0; i < selectedPlantArray.size(); ++i) {
        auto &item = selectedPlantArray[i];

        PlantCardItem *plantCardItem = new PlantCardItem(item);
        plantCardItem->setChecked(false);
        cardPanel->addToGroup(plantCardItem);
        plantCardItem->setPos(0, i * 60);
        TooltipItem *tooltipItem = new TooltipItem("");
        tooltipItem->setVisible(false);
        tooltipItem->setOpacity(0.9);
        tooltipItem->setZValue(1);
        addItem(tooltipItem);

        connect(plantCardItem, &PlantCardItem::hoverEntered, [this, tooltipItem](QGraphicsSceneHoverEvent *event) {
            if (choose) return;
            tooltipItem->setPos(event->scenePos() + QPointF(5, 15));
            tooltipItem->setVisible(true);
        });
        connect(plantCardItem, &PlantCardItem::hoverMoved, [this, tooltipItem](QGraphicsSceneHoverEvent *event) {
            if (choose) return;
            tooltipItem->setPos(event->scenePos() + QPointF(5, 15));
        });
        connect(plantCardItem, &PlantCardItem::hoverLeft, [this, tooltipItem](QGraphicsSceneHoverEvent *event) {
            if (choose) return;
            tooltipItem->setPos(event->scenePos() + QPointF(5, 15));
            tooltipItem->setVisible(false);
        });

        cardGraphics.push_back({ plantCardItem, tooltipItem });
        cardReady.push_back({ false, false });
        updateTooltip(i);
    }
    // All excluded mousePress or clicked must be triggered from one object to avoid duplicated triggering.
    //所有排除的 mousePress 或 clicked 必须从一个对象触发，以避免重复触发
    connect(this, &GameScene::mousePress, [this](QGraphicsSceneMouseEvent *event) {
        if (choose) return;
        int i;
        for (i = 0; i < selectedPlantArray.size(); ++i) {
            if (cardReady[i].cool && cardReady[i].sun &&
                cardGraphics[i].plantCard->contains(event->scenePos() - cardGraphics[i].plantCard->scenePos()))
                break;
        }
        Plant *item = nullptr;
        QPointF delta;
        if (i != selectedPlantArray.size()) {
            cardGraphics[i].tooltip->setVisible(false);
            item = selectedPlantArray[i];
            QPixmap staticGif = gImageCache->load(item->staticGif);
            delta = QPointF(-0.5 * (item->beAttackedPointL + item->beAttackedPointR), 20 - staticGif.height());
            Animate(movePlant).finish();
            movePlantAlpha->setPixmap(staticGif);
            movePlant->setPixmap(staticGif);
            movePlant->setPos(event->scenePos() + delta);
            movePlant->setVisible(true);
            seedliftMusic->stop();
            seedliftMusic->play();
            choose = 1;
        }
        else if (shovel->contains(event->scenePos() - shovel->scenePos()) || shovelBackground->contains(event->scenePos() - shovelBackground->scenePos())) {
            delta = QPointF(-28, -25);
            Animate(shovel).finish();
            shovel->setCursor(Qt::ArrowCursor);
            shovelBackground->setCursor(Qt::ArrowCursor);
            shovel->setPos(event->scenePos() - shovelBackground->scenePos() + delta);
            shovelMusic->stop();
            shovelMusic->play();
            choose = 2;
        }
        else
            return;
        QSharedPointer<QMetaObject::Connection> moveConnection(new QMetaObject::Connection), clickConnection(new QMetaObject::Connection);
        QSharedPointer<QUuid> uuid(new QUuid);
        *moveConnection = connect(this, &GameScene::mouseMove, [this, delta, item, uuid](QGraphicsSceneMouseEvent *e) {
            if (choose == 1) {
                movePlant->setPos(e->scenePos() + delta);
                auto xPair = coordinate.choosePlantX(e->scenePos().x()), yPair = coordinate.choosePlantY( e->scenePos().y());
                if (item->canGrow(xPair.second, yPair.second)) {
                    movePlantAlpha->setVisible(true);
                    movePlantAlpha->setPos(xPair.first + item->getDX(), yPair.first + item->getDY(xPair.second, yPair.second) - item->height);
                }
                else
                    movePlantAlpha->setVisible(false);
            }
            else {
                shovel->setPos(e->scenePos() - shovelBackground->scenePos() + delta);
                PlantInstance *plant = getPlant(e->scenePos());
                if (!uuid->isNull() && (!plant || plant->uuid != *uuid)) {
                    PlantInstance *prevPlant = getPlant(*uuid);
                    if (prevPlant)
                        prevPlant->picture->setOpacity(1.0);
                }
                if (plant && plant->uuid != *uuid) {
                    plant->picture->setOpacity(0.6);
                }
                if (plant) {
                    *uuid = plant->uuid;
                    shovel->setCursor(Qt::PointingHandCursor);
                }
                else {
                    *uuid = QUuid();
                    shovel->setCursor(Qt::ArrowCursor);
                }
            }
        });
        *clickConnection = connect(this, &GameScene::mousePress, [this, i, moveConnection, clickConnection, item, uuid](QGraphicsSceneMouseEvent *e) {
            disconnect(*moveConnection);
            disconnect(*clickConnection);
            if (choose == 1) {
                movePlantAlpha->setVisible(false);
                auto xPair = coordinate.choosePlantX(e->scenePos().x()), yPair = coordinate.choosePlantY(e->scenePos().y());
                if (e->button() == Qt::LeftButton && item->canGrow(xPair.second, yPair.second)) {
                    movePlant->setVisible(false);
                    MoviePixmapItem *growGif;
                    if (gameLevelData->LF[yPair.second] == 1)
                        growGif = imgGrowSoil;
                    else
                        growGif = imgGrowSpray;
                    growGif->setPos(xPair.first - 30, yPair.first - 30);
                    growGif->setVisible(true);
                    growGif->start();
                    QSharedPointer<QMetaObject::Connection> connection(new QMetaObject::Connection);
                    *connection = connect(growGif, &MoviePixmapItem::finished, [growGif, connection]{
                        growGif->setVisible(false);
                        growGif->reset();
                        disconnect(*connection.data());
                    });
                    auto key = qMakePair(xPair.second, yPair.second);
                    if (plantPosition.contains(key) && plantPosition[key].contains(item->pKind))
                        plantDie(plantPosition[key][item->pKind]);
                    PlantInstance *plantInstance = PlantInstanceFactory(item);
                    plantInstance->birth(xPair.second, yPair.second);
                    plantInstances.push_back(plantInstance);
                    if (!plantPosition.contains(key))
                        plantPosition.insert(key, QMap<int, PlantInstance *>());
                    plantPosition[key].insert(item->pKind, plantInstance);
                    plantUuid.insert(plantInstance->uuid, plantInstance);
                    doCoolTime(i);
                    sunNum -= item->sunNum;
                    updateSunNum();
                    // TODO: Plant Water music
                    plantMusic1->stop();
                    plantMusic2->stop();
                    if (qrand() % 2)
                        plantMusic1->play();
                    else
                        plantMusic2->play();
                }
                else {
                    tapMusic->stop();
                    tapMusic->play();
                    Animate(movePlant).move(cardGraphics[i].plantCard->scenePos() + QPointF(10, 0)).speed(1.5).finish([this] {
                        movePlant->setVisible(false);
                    });
                }
            }
            else {
                Animate(shovel).move(QPointF(0, -5)).speed(1.5).finish([this] {
                    shovel->setCursor(Qt::PointingHandCursor);
                    shovelBackground->setCursor(Qt::PointingHandCursor);
                });
                if (!uuid->isNull()) {
                    PlantInstance *prevPlant = getPlant(*uuid);
                    if (prevPlant)
                        prevPlant->picture->setOpacity(1.0);
                }
                PlantInstance *plant;
                if (e->button() == Qt::LeftButton && (plant = getPlant(e->scenePos()))) {
                    plantDie(plant);
                    plantMusic1->stop();
                    plantMusic2->stop();
                    plantMusic2->play();
                }
                else {
                    tapMusic->stop();
                    tapMusic->play();
                }
            }
            choose = 0;

        });
    });
    backgroundMusic->blockSignals(true);
    backgroundMusic->stop();
    backgroundMusic->blockSignals(false);
    gameLevelData->startGame(this);
}

//冷却
void GameScene::beginCool()
{
    for (int i = 0; i < selectedPlantArray.size(); ++i) {
        auto &item = selectedPlantArray[i];
        auto &plantCardItem = cardGraphics[i].plantCard;
        if (item->coolTime < 7.6) {
            plantCardItem->setPercent(1.0);
            cardReady[i].cool = true;
            if (item->sunNum <= sunNum) {
                cardReady[i].sun = true;
                plantCardItem->setChecked(true);
            }
            updateTooltip(i);
            continue;
        }
        doCoolTime(i);
    }
}

void GameScene::updateTooltip(int index)
{
    auto &item = selectedPlantArray[index];
    QString text = "<b>" + item->cName + "</b><br />" +
                   QString(tr("Cool down: %1s")).arg(item->coolTime) + "<br />";
    text += item->toolTip;
    if (!cardReady[index].cool)
        text += "<br><span style=\"color:#f00\">" + tr("Rechanging...") + "</span>";
    if (!cardReady[index].sun)
        text += "<br><span style=\"color:#f00\">" + tr("Not enough sun!") + "</span>";
    cardGraphics[index].tooltip->setText(text);
}

QPair<MoviePixmapItem *, std::function<void(bool)> > GameScene::newSun(int sunNum)
{
    MoviePixmapItem *sunGif = new MoviePixmapItem("interface/Sun.gif");
    if (sunNum == 15)
        sunGif->setScale(46.0 / 79.0);
    else if (sunNum != 25)
        sunGif->setScale(100.0 / 79.0);
    sunGif->setZValue(2);
    sunGif->setOpacity(0.8);
    sunGif->setCursor(Qt::PointingHandCursor);
    addItem(sunGif);
    QSharedPointer<QTimer *> timer(new QTimer *(nullptr));
    QSharedPointer<QMetaObject::Connection> connection(new QMetaObject::Connection);

    *connection = connect(sunGif, &MoviePixmapItem::click, [this, sunGif, sunNum, timer] {
        if (choose != 0) return;
        if (*timer)
            delete *timer;
        sunMusic->stop();
        sunMusic->play();
        Animate(sunGif).finish().move(QPointF(100, 0)).speed(1).scale(34.0 / 79.0).finish([this, sunGif, sunNum] {
            delete sunGif;
            this->sunNum += sunNum;
            updateSunNum();
        });
    });
    return qMakePair(sunGif, [this, sunGif, timer, connection](bool finished) {
        if (finished) {
            (*timer = new Timer(this, 8000, [this, sunGif, connection] {
                disconnect(*connection);
                sunGif->setCursor(Qt::ArrowCursor);
                Animate(sunGif).fade(0).duration(500).finish([sunGif] {
                    delete sunGif;
                });
            }))->start();
        }
    });
}


void GameScene::beginSun(int sunNum)
{
    auto sunGifAndOnFinished = newSun(sunNum);
    MoviePixmapItem *sunGif = sunGifAndOnFinished.first;
    std::function<void(bool)> onFinished = sunGifAndOnFinished.second;
    double toX = coordinate.getX(1 + qrand() % coordinate.colCount()),
           toY = coordinate.getY(1 + qrand() % coordinate.rowCount());
    sunGif->setPos(toX, -100);
    sunGif->start();
    Animate(sunGif).move(QPointF(toX, toY - 53)).speed(0.04).finish(onFinished);

    (new Timer(this, (qrand() % 9000 + 3000), [this, sunNum] { beginSun(sunNum); }))->start();
}

void GameScene::doCoolTime(int index)
{
    auto &item = selectedPlantArray[index];
    cardGraphics[index].plantCard->setPercent(0);
    cardGraphics[index].plantCard->setChecked(false);
    if (cardReady[index].cool) {
        cardReady[index].cool = false;
        updateTooltip(index);
    }
    (new TimeLine(this, qRound(item->coolTime * 1000), 20, [this, index](qreal x) {
        cardGraphics[index].plantCard->setPercent(x);
    }, [this, index] {
        cardReady[index].cool = true;
        if (cardReady[index].sun)
            cardGraphics[index].plantCard->setChecked(true);
        updateTooltip(index);
    }))->start();
}

void GameScene::updateSunNum()
{
    sunNumText->setText(QString::number(sunNum));
    QSizeF sunNumTextSize = sunNumText->boundingRect().size();
    sunNumText->setPos(76 - sunNumTextSize.width() / 2, (sunNumGroup->boundingRect().height() - sunNumTextSize.height()) / 2);
    for (int i = 0; i < selectedPlantArray.size(); ++i) {
        auto &item = selectedPlantArray[i];
        auto &plantCardItem = cardGraphics[i].plantCard;
        if (item->sunNum <= sunNum) {
            if (!cardReady[i].sun) {
                cardReady[i].sun = true;
                updateTooltip(i);
            }
            if (cardReady[i].cool)
                plantCardItem->setChecked(true);
        }
        else {
            if (cardReady[i].sun) {
                cardReady[i].sun = false;
                updateTooltip(i);
            }
            plantCardItem->setChecked(false);
        }
    }
}

QPointF GameScene::sizeToPoint(const QSizeF &size)
{
    return QPointF(size.width(), size.height());
}

//放置小推车？
void GameScene::customSpecial(const QString &name, int col, int row)
{
    PlantInstance *plantInstance = PlantInstanceFactory(getPlantProtoType(name));
    plantInstance->birth(col, row);
    plantInstances.push_back(plantInstance);
    auto key = qMakePair(col, row);
    if (!plantPosition.contains(key))
        plantPosition.insert(key, QMap<int, PlantInstance *>());
    plantPosition[key].insert(plantInstance->plantProtoType->pKind, plantInstance);
    plantUuid.insert(plantInstance->uuid, plantInstance);
}

void GameScene::addToGame(QGraphicsItem *item)
{
    gameGroup->addToGroup(item);
}

void GameScene::beginZombies()
{
    waveMusic->setMedia(QUrl("qrc:/audio/awooga.mp3"));
    waveMusic->play();
    Animate(flagMeter).move(QPointF(700, 560)).speed(0.5).finish();
    advanceFlag();
    QSharedPointer<std::function<void(void)> > playGroan(new std::function<void(void)>);
    *playGroan = [this, playGroan] {
        switch (qrand() % 6) {
            case 0: groanMusic->setMedia(QUrl("qrc:/audio/groan1.mp3")); break;
            case 1: groanMusic->setMedia(QUrl("qrc:/audio/groan2.mp3")); break;
            case 2: groanMusic->setMedia(QUrl("qrc:/audio/groan3.mp3")); break;
            case 3: groanMusic->setMedia(QUrl("qrc:/audio/groan4.mp3")); break;
            case 4: groanMusic->setMedia(QUrl("qrc:/audio/groan5.mp3")); break;
            default: groanMusic->setMedia(QUrl("qrc:/audio/groan6.mp3")); break;
        }
        groanMusic->play();
        (new Timer(this, 20000, *playGroan))->start();
    };
    (new Timer(this, 20000, *playGroan))->start();
}

void GameScene::prepareGrowPlants(std::function<void(void)> functor)
{
    QPixmap imgPrepareGrowPlants = gImageCache->load("interface/PrepareGrowPlants.png");
    QGraphicsPixmapItem *imgPrepare = new QGraphicsPixmapItem(imgPrepareGrowPlants.copy(0, 0, 255, 108)),
            *imgGrow    = new QGraphicsPixmapItem(imgPrepareGrowPlants.copy(0, 108, 255, 108)),
            *imgPlants  = new QGraphicsPixmapItem(imgPrepareGrowPlants.copy(0, 216, 255, 108));

    //将图片放在屏幕中间
    QPointF pos = sizeToPoint(sceneRect().size() - imgPrepare->boundingRect().size()) / 2;
    imgPrepare->setPos(pos);
    imgGrow->setPos(pos);
    imgPlants->setPos(pos);
    imgPrepare->setZValue(1);
    imgGrow->setZValue(1);
    imgPlants->setZValue(1);
    imgPrepare->setVisible(false);
    imgGrow->setVisible(false);
    imgPlants->setVisible(false);
    addItem(imgPrepare);
    addItem(imgGrow);
    addItem(imgPlants);
    imgPrepare->setVisible(true);
    backgroundMusic->blockSignals(true);
    backgroundMusic->stop();
    backgroundMusic->blockSignals(false);
    backgroundMusic->setMedia(QUrl("qrc:/audio/readysetplant.mp3"));
    backgroundMusic->play();
    (new Timer(this, 600, [this, imgPrepare, imgGrow, imgPlants, functor] {
        delete imgPrepare;
        imgGrow->setVisible(true);
        (new Timer(this, 400, [this, imgGrow, imgPlants, functor] {
            delete imgGrow;
            imgPlants->setVisible(true);
            (new Timer(this, 1200, [this, imgPlants, functor] {
                delete imgPlants;
                functor();
            }))->start();
        }))->start();
    }))->start();
}

void GameScene::advanceFlag()   //++wave下一波
{
    ++waveNum;
    flagMeter->updateFlagZombies(waveNum);
    if (waveNum < gameLevelData->flagNum) {
        auto iter = gameLevelData->flagToMonitor.find(waveNum);
        if (iter != gameLevelData->flagToMonitor.end())
            (new Timer(this, 16900, [this, iter] { (*iter)(this); }))->start();
        (waveTimer = new Timer(this, 19900, [this] { advanceFlag(); }))->start();
    }
    auto &flagToSumNum = gameLevelData->flagToSumNum;
    selectFlagZombie(flagToSumNum.second[qLowerBound(flagToSumNum.first, waveNum) - flagToSumNum.first.begin()]);
    //lowerbound()返回值是一个迭代器,返回指向大于等于key的第一个值的位置
    //waveNum表示波数   函数qLowerBound返回的是flagToSumNum.first元素的位置——0、1、2、3，1到3回合返回0，4到6返回1，6到9返回2，10返回3
}

void GameScene::plantDie(PlantInstance *plant)  //植物死亡
{
    plantPosition[qMakePair(plant->col, plant->row)].remove(plant->plantProtoType->pKind);
    for (int i = 0; i < plantTriggers[plant->row].size(); ) {
        if (plantTriggers[plant->row][i]->plant == plant) {
            delete plantTriggers[plant->row][i];
            plantTriggers[plant->row].removeAt(i);
            continue;
        }
        ++i;
    }
    plantInstances.removeAt(plantInstances.indexOf(plant));
    plantUuid.remove(plant->uuid);
    delete plant;
}

//处理胜利
void GameScene::zombieDie(ZombieInstance *zombie)   //僵尸死亡
{
    int i = zombieInstances.indexOf(zombie);
    zombieInstances.removeAt(i);
    zombieRow[zombie->row].removeOne(zombie);
    if (zombieInstances.isEmpty() && !gameStatus) {     //僵尸队列为空，同时在游戏中，有以下两种状态

        //第一种，这一波僵尸被打完了，开始下一波
        if (waveNum < gameLevelData->flagNum) {
                delete waveTimer;
                (new Timer(this, 5000, [this] { advanceFlag(); }))->start();        //提前下一波
        }

        //第二种，所有僵尸都打完了，游戏胜利
        else{
            //标志游戏胜利
            gameStatus = true;
            //设置奖杯图像
            MouseEventPixmapItem *trophyImg = new MouseEventPixmapItem(gImageCache->load("interface/trophy.png"));

            //设置鼠标形状，将鼠标形状设置为向上的小手
            trophyImg->setCursor(Qt::PointingHandCursor);
            //奖杯从最后一个被打死的僵尸身上出来，停到随机的X
            double fromX = zombie->ZX,
                    toX = zombie->ZX - qrand() % 80,
                    toY = coordinate.getY(zombie->row);

            //以下两个函数为以指定位置进行缩放
            //缩放的位置为setPos
            //扩大为为原来的1倍
            trophyImg->setScale(1);
            trophyImg->setPos(fromX, toY - 25);
            //添加奖杯事件
            addItem(trophyImg);

            //设置奖杯的点击效果
            //点击奖杯结束游戏
            connect(trophyImg, &MouseEventPixmapItem::clicked, [this, trophyImg]{
                trophyImg->setEnabled(false);
                trophyImg->deleteLater();
                winGame();
            });
        }
    }
    zombieUuid.remove(zombie->uuid);
    delete zombie;
}

void GameScene::selectFlagZombie(int levelSum)  //selectFlagZombie得到的是flagToSumNum.second中的一个值
{
    int timeout = 1500;
    QList<Zombie *> zombies, zombiesCandidate;
    if (gameLevelData->largeWaveFlag.contains(waveNum)) {
        waveMusic->setMedia(QUrl("qrc:/audio/siren.mp3"));
        waveMusic->play();
        Zombie *flagZombie = getZombieProtoType("oFlagZombie");
        levelSum -= flagZombie->level;
        zombies.push_back(flagZombie);
        timeout = 300;
    }
    for (const auto &zombieData: gameLevelData->zombieData) {
        if (zombieData.flagList.contains(levelSum)) {
            Zombie *item = getZombieProtoType(zombieData.eName);
            levelSum -= item->level;                    //levelSum越大，减小的越慢，僵尸越多
            zombies.push_back(item);                    //将得到的僵尸存入数组
        }
        if (zombieData.firstFlag <= waveNum) {          //如果firstFlag小于当前波数僵尸才会出现
            Zombie *item = getZombieProtoType(zombieData.eName);
            for (int i = 0; i < zombieData.num; ++i)
                zombiesCandidate.push_back(item);
        }
    }
    qSort(zombiesCandidate.begin(), zombiesCandidate.end(), [](Zombie *a, Zombie *b) { return a->level < b->level; });
    while (levelSum > 0) {
        while (zombiesCandidate.last()->level > levelSum)
            zombiesCandidate.pop_back();
        Zombie *item = zombiesCandidate[qrand() % zombiesCandidate.size()];
        levelSum -= item->level;
        zombies.push_back(item);
    }
    for (int i = 0; i < zombies.size(); ++i) {
        Zombie *zombie = zombies[i];
        (new Timer(this, i * timeout, [this, zombie] {
            int row;
            do {
                row = qrand() % coordinate.rowCount() + 1;
            } while (!zombie->canPass(row));
            ZombieInstance * zombieInstance = ZombieInstanceFactory(zombie);
            zombieInstance->birth(row);
            zombieInstances.push_back(zombieInstance);
            zombieRow[row].push_back(zombieInstance);
            qSort(zombieRow[row].begin(), zombieRow[row].end(), [](ZombieInstance *a, ZombieInstance *b) {
                return b->attackedLX < a->attackedLX;
            });
            zombieUuid.insert(zombieInstance->uuid, zombieInstance);
        }))->start();
    }
    qDebug() << "Wave: " << waveNum;
    for (auto item: zombies)
        qDebug() << "    " << item->eName;
}

QMap<int, PlantInstance *> GameScene::getPlant(int col, int row)
{
    auto iter = plantPosition.find(qMakePair(col, row));
    if (iter == plantPosition.end())
        return QMap<int, PlantInstance*>();
    return *iter;
}

PlantInstance *GameScene::getPlant(const QPointF &pos)
{
    for (auto plant: plantInstances)
        if (plant->contains(pos - plant->picture->scenePos()))
            return plant;
    return nullptr;
}

bool GameScene::isCrater(int col, int row) const
{
    return qBinaryFind(craters, qMakePair(col, row)) != craters.end();
}

bool GameScene::isTombstone(int col, int row) const
{
    return qBinaryFind(tombstones, qMakePair(col, row)) != tombstones.end();
}

Coordinate &GameScene::getCoordinate()
{
    return coordinate;
}

void GameScene::addTrigger(int row, Trigger *trigger)
{
    plantTriggers[row].push_back(trigger);
    qSort(plantTriggers[row].begin(), plantTriggers[row].end(), [](const Trigger *a, const Trigger *b) {
        return a->to < b->to;
    });
}

void GameScene::beginMonitor()
{
    QTimer *monitorTimer = new QTimer(this);
    monitorTimer->setInterval(100);
    connect(monitorTimer, &QTimer::timeout, [this] {
        for (int row = 1; row <= coordinate.rowCount(); ++row) {
            QList<ZombieInstance *> zombiesCopy = zombieRow[row];
            for (ZombieInstance *zombie: zombiesCopy) {
                QUuid zombieUuid = zombie->uuid;
                if (zombie->hp > 0 && zombie->ZX <= 900) {
                    QList<Trigger *> triggerCopy = plantTriggers[row];
                    for (auto trigger: triggerCopy) {
                        if (trigger->plant->canTrigger
                            && trigger->from <= zombie->attackedLX
                            && trigger->to >= zombie->attackedLX) {
                            trigger->plant->triggerCheck(zombie, trigger);
                        }
                    }
                }
                ZombieInstance *z = getZombie(zombieUuid);
                if (z)
                    z->checkActs();
            }
        }
    });
    monitorTimer->start();
}

PlantInstance *GameScene::getPlant(const QUuid &uuid)
{
    if (plantUuid.contains(uuid))
        return plantUuid[uuid];
    return nullptr;
}

ZombieInstance *GameScene::getZombie(const QUuid &uuid)
{
    if (zombieUuid.contains(uuid))
        return zombieUuid[uuid];
    return nullptr;
}

QList<ZombieInstance *> GameScene::getZombieOnRow(int row)
{
    return zombieRow[row];
}

QList<ZombieInstance *> GameScene::getZombieOnRowRange(int row, qreal from, qreal to)
{
    QList<ZombieInstance *> zombies;
    for (auto zombie: zombieRow[row])
        if (zombie->hp > 0 && zombie->attackedLX < to && (zombie->attackedLX > from || zombie->attackedRX > from))
            zombies.push_back(zombie);
    return zombies;
}

Plant *GameScene::getPlantProtoType(const QString &eName)
{
    if (plantProtoTypes.find(eName) == plantProtoTypes.end())
        plantProtoTypes.insert(eName, PlantFactory(this, eName));
    return plantProtoTypes[eName];
}

Zombie *GameScene::getZombieProtoType(const QString &eName)
{
    if (zombieProtoTypes.find(eName) == zombieProtoTypes.end())
        zombieProtoTypes.insert(eName, ZombieFactory(this, eName));
    return zombieProtoTypes[eName];
}

void GameScene::beginBGM()
{
    backgroundMusic->blockSignals(true);
    backgroundMusic->stop();
    backgroundMusic->blockSignals(false);
    backgroundMusic->setMedia(QUrl(gameLevelData->backgroundMusic));
    backgroundMusic->play();
}

FlagMeter::FlagMeter(GameLevelData *gameLevelData)
    : flagNum(gameLevelData->flagNum),
      flagHeadStep(140.0 / (flagNum - 1)),
      flagMeterEmpty(gImageCache->load("interface/FlagMeterEmpty.png")),
      flagMeterFull(gImageCache->load("interface/FlagMeterFull.png")),
      flagTitle(new QGraphicsPixmapItem(gImageCache->load("interface/FlagMeterLevelProgress.png"))),
      flagHead(new QGraphicsPixmapItem(gImageCache->load("interface/FlagMeterParts1.png")))
{
    setPixmap(flagMeterEmpty);

    flagTitle->setPos(35, 12);
    flagTitle->setParentItem(this);
    for (auto i: gameLevelData->largeWaveFlag) {
        QGraphicsPixmapItem *flag = new QGraphicsPixmapItem(gImageCache->load("interface/FlagMeterParts2.png"));
        flag->setPos(150 - (i - 1) * flagHeadStep, -3);
        flag->setParentItem(this);
        flags.insert(i, flag);
    }
    flagHead->setPos(139, -4);
    flagHead->setParentItem(this);
    updateFlagZombies(1);
}

void FlagMeter::updateFlagZombies(int flagZombies)
{
    auto iter = flags.find(flagZombies);
    if (iter != flags.end())
        iter.value()->setY(-12);
    if (flagZombies < flagNum) {
        qreal x = 150 - (flagZombies - 1) * flagHeadStep;
        flagHead->setPos(x - 11, -4);
        QPixmap flagMeter(flagMeterFull);
        QPainter p(&flagMeter);
        p.drawPixmap(0, 0, flagMeterEmpty.copy(0, 0, qRound(x), 21));
        setPixmap(flagMeter);
    }
    else {
        flagHead->setPos(-1, -3);
        setPixmap(flagMeterFull);
    }
}

Trigger::Trigger(PlantInstance *plant, qreal from, qreal to, int direction, int id)
        : plant(plant), from(from), to(to),
          direction(direction), id(id)
{}
