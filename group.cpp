#include "group.h"

Group::Group(QString n, unsigned int s) :
    name(n), subgroupIndex(s)
{}
Group::Group(const Group& g) :
    name(g.name), subgroupIndex(g.subgroupIndex)
{}

QString Group::getName() const
{
    return name;
}

unsigned int Group::getSubgroupIndex() const
{
    return subgroupIndex;
}

void Group::setSubgroupIndex(unsigned int i)
{
    subgroupIndex = i;
}
