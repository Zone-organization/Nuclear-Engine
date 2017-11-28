/*
 * MatrixInitializer.h
 * 
 * This file is part of the "GaussianLib" project (Copyright (c) 2015 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef GS_MATRIX_INITIALIZER_H
#define GS_MATRIX_INITIALIZER_H


#include "Decl.h"


namespace Math
{


namespace Details
{


//! Common matrix initializer.
template <class M, typename T, std::size_t Cols>
class MatrixInitializer
{
            
    public:

        MatrixInitializer(M& matrix) :
            matrix_  { matrix },
            element_ { 0      }
        {
        }

        MatrixInitializer<M, T, Cols>& operator , (const T& nextValue)
        {
            matrix_(element_ / Cols, element_ % Cols) = nextValue;
            ++element_;
            return *this;
        }

    private:

        M&          matrix_;
        std::size_t element_;

};


template <typename T, std::size_t Rows, std::size_t Cols>
struct MatrixDefaultInitializer
{
    static void Initialize(Matrix<T, Rows, Cols>& matrix)
    {
        matrix.Reset();
    }
};

template <typename T, std::size_t N>
struct MatrixDefaultInitializer<T, N, N>
{
    static void Initialize(Matrix<T, N, N>& matrix)
    {
        matrix.LoadIdentity();
    }
};


} // /namespace Details


} // /namespace Gs


#endif



// ================================================================================
