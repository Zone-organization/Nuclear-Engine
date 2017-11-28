/*
 * Flip.h
 * 
 * This file is part of the "GaussianLib" project (Copyright (c) 2015 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef GS_FLIP_H
#define GS_FLIP_H


#include "Decl.h"


namespace Math
{


/**
\brief Flips an axis of the specified matrix 'm'.
\tparam M Specifies the matrix type. This should be Matrix<N, N>, AffineMatrix3, or AffineMatrix4.
This type must implement the following interface:
\code
static const std::size_t columns;
\endcode
\tparam T Specifies the data type. Can be any type that supports negation (i.e. x = -x).
\param[in,out] mat Specifies the matrix whose axis is to be flipped.
\param[in] axis Specifies the zero-based axis index. This value must be 
\remarks This is an example how such an axis flip can also be expressed:
\code
Math::Matrix4 m;

// Variant 1:
Math::Matrix4 flipMatrix;
flipMatrix << 1,  0, 0, 0,
              0, -1, 0, 0,
              0,  0, 1, 0,
              0,  0, 0, 1;
m = flipMatrix * m;

// Variant 2:
Math::FlipAxis(m, 1);
\endcode
*/
template <class M>
void FlipAxis(M& mat, std::size_t axis)
{
    for (std::size_t i = 0; i < M::columns; ++i)
        mat.At(axis, i) = -mat.At(axis, i);
}


} // /namespace Math


#endif



// ================================================================================
