#ifndef MATH_H
#define MATH_H

#include "GLobal.h"
#include <map>
#include <vector>

#include "Color.h"
#include "Rects.h"
#include "Perspective.h"
#include "Vectors.h"
#include "Clock.h"
#include "Game.h"
#include "Transform.h"
#include "Matrix.h"

template<typename type>
size_t getMin(size_t size, const type* data) {
    size_t result = 0;
    for (size_t i = 0; i < size - 1; i++)
        if (data[result] > data[i])result = i;

    return result;
}

template<typename type>
type getSum(size_t size, const type* data) {
    type result = data[0];
    for (size_t i = 1; i < size; i++)
        result += data[i];
    return result;
}


#endif