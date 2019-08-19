#ifndef GROUPITEM_H
#define GROUPITEM_H

#include "group.h"

#include <QWidget>
#include <memory>

namespace Ui {
class GroupItem;
}

class GroupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GroupWidget(std::shared_ptr<Group> group, QWidget *parent = nullptr);
    ~GroupWidget();

private:
    Ui::GroupItem *ui;
    std::shared_ptr<Group> group;
};

#endif // GROUPITEM_H
