#include "groupwidget.h"
#include "ui_groupitem.h"

#include <QPalette>

GroupWidget::GroupWidget(Group g, QWidget *parent) :
    QWidget(parent), ui(new Ui::GroupItem), group(g)
{
    ui->setupUi(this);

    ui->groupNameLabel->setText(g.getName());
    setSubgroupText(g.getSubgroupIndex());
}

GroupWidget::~GroupWidget()
{
    delete ui;
}

void GroupWidget::setActive(bool active)
{
    QPalette p;
    if(active)
    {
        p.setColor(QPalette::Background, Qt::green);
    }
    else
    {
        p.setColor(QPalette::Background, Qt::white);
    }
    setPalette(p);
}

void GroupWidget::setSubgroupIndex(unsigned int index)
{
    group.setSubgroupIndex(index);
    setSubgroupText(index);
}

void GroupWidget::setSubgroupText(unsigned int index)
{
    // TODO: calculate LEVEL from INDEX
    QString lvl = QString::number(index);
    ui->subgroupLevelLabel->setText("Level: " + lvl);
}

const Group& GroupWidget::getGroup() const
{
    return group;
}
