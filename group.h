#ifndef GROUP_H
#define GROUP_H

#include <QString>

class Group
{
public:
    Group(QString name, unsigned int subgroupIndex);

    QString getName() const;
    unsigned int getSubgroupIndex() const;

private:
    QString name;
    unsigned int subgroupIndex;
};

#endif // GROUP_H
