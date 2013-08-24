////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2012 Laurent Gomila (laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef NephilimMutexImplUnix_h__
#define NephilimMutexImplUnix_h__

#include <Nephilim/Platform.h>
#include <pthread.h>

NEPHILIM_NS_BEGIN

namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Unix implementation of mutexes
////////////////////////////////////////////////////////////
class MutexImpl/* : NonCopyable*/
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    MutexImpl();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~MutexImpl();

    ////////////////////////////////////////////////////////////
    /// \brief Lock the mutex
    ///
    ////////////////////////////////////////////////////////////
    void lock();

    ////////////////////////////////////////////////////////////
    /// \brief Unlock the mutex
    ///
    ////////////////////////////////////////////////////////////
    void unlock();

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    pthread_mutex_t m_mutex; ///< pthread handle of the mutex
};

} // namespace priv

NEPHILIM_NS_END
#endif // NephilimMutexImplUnix_h__
