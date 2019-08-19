#include "group.h"

Group::Group(QString n, unsigned int s) :
    name(n), subgroupIndex(s)
{
}

QString Group::getName() const
{
    return name;
}

unsigned int Group::getSubgroupIndex() const
{
    return subgroupIndex;
}
