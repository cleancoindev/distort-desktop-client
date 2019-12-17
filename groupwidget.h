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
    explicit GroupWidget(Group group, QWidget *parent = nullptr);
    ~GroupWidget();

    void setActive(bool active);
    void setSubgroupIndex(unsigned int index);
    const Group& getGroup() const;

private:
    Ui::GroupItem *ui;
    Group group;

    void setSubgroupText(unsigned int index);
};

#endif // GROUPITEM_H
