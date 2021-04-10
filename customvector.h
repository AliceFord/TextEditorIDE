#ifndef CUSTOMVECTOR_H
#define CUSTOMVECTOR_H

#include "customfile.h"

#include <QVector>
#include <QDebug>

template<typename T>
class CustomVector : public QVector<T>
{
public:
    using QVector<T>::QVector;

    CustomFile *find(int index)
    {
        for (int i=0;i<this->length();i++)
            if ((*this)[i].index == index)
                return &(*this)[i];
    }
};

#endif // CUSTOMVECTOR_H
