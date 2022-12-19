//
// Created by sun on 8/31/16.
//

#ifndef PLANTS_VS_ZOMBIES_ASPECTRATIOLAYOUT_H
#define PLANTS_VS_ZOMBIES_ASPECTRATIOLAYOUT_H

#include <QtWidgets>

class AspectRatioLayout: public QLayout     //纵横比布局
{
public:
    explicit AspectRatioLayout(QWidget *parent = nullptr);
    virtual ~AspectRatioLayout();

    virtual void addItem(QLayoutItem *item) override;   //添加子布局
    virtual QLayoutItem *itemAt(int index) const override;  //访问编号为index的item
    virtual QLayoutItem *takeAt(int index) override;    //弹出编号为index的item
    virtual int count() const override;     //获取数量

    virtual QSize minimumSize() const override;     //最小大小限制
    virtual QSize sizeHint() const override;    //获取大小坐标
    virtual void setGeometry(const QRect &rect) override;   //布局子布局不见
    virtual Qt::Orientations expandingDirections() const override;  //设置可否拉伸和拉伸方式

private:
    QLayoutItem *item;
};

#endif //PLANTS_VS_ZOMBIES_ASPECTRATIOLAYOUT_H
