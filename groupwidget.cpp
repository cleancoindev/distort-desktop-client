#include "groupwidget.h"
#include "ui_groupitem.h"

#include <sstream>

GroupWidget::GroupWidget(std::shared_ptr<Group> g, QWidget *parent) :
    QWidget(parent), ui(new Ui::GroupItem), group(g)
{
    ui->setupUi(this);

    ui->groupNameLabel->setText(group->getName());

    // TODO: calculate LEVEL from INDEX
    std::stringstream ss;
    ss << group->getSubgroupIndex();
    QString lvl = QString::fromStdString(ss.str());
    ui->subgroupLevelLabel->setText("Level: " + lvl);
}

GroupWidget::~GroupWidget()
{
    delete ui;
}
