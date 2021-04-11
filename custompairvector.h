#ifndef CUSTOMPAIRVECTOR_H
#define CUSTOMPAIRVECTOR_H

#include "customfile.h"

#include <QVector>
#include <QDebug>
#include <QAction>

template<typename T>
class CustomPairVector : public QVector<T>
{
public:
    using QVector<T>::QVector;

    QAction *find(int index)
    {
        for (int i=0;i<this->length();i++)
            if ((*this)[i].first == index)
                return (*this)[i].second;
    }

    QPair<int, QAction*> removeByNumber(int index)
    {
        for (int i=0;i<this->length();i++) {
            if ((*this)[i].first == index) {
                auto item = (*this)[i];
                (*this).remove(i);
                return item;
            }
        }
    }
};

#endif // CUSTOMPAIRVECTOR_H
