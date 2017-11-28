/*
 * Tags.h
 * 
 * This file is part of the "GaussianLib" project (Copyright (c) 2015 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef GS_TAGS_H
#define GS_TAGS_H


namespace Math
{


/**
\brief Common uninitialize tag.
\remarks This can be used to explicitly construct an uninitialized vector, matrix or quaternion:
\code
Math::Matrix4 m(Math::UninitializeTag{});
// ...
//m(0, 1) = ...
\endcode
*/
struct UninitializeTag {};


} // /namespace Math


#endif



// ================================================================================
