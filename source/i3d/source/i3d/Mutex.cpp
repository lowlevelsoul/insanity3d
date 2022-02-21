/*
==========================================================================================================================================
CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
Copyright (C) 2021 James Steele. All Rights Reserved.
==========================================================================================================================================
*/

#include "i3d/Mutex.h"
#include <mutex>
#include <new>

namespace i3d { namespace mutex {

	struct MutexData {
		std::mutex	mutex;
	};

	//==========================================================================================================================================
	void Create( Mutex & mutex ) {
		assert( sizeof( mutex.m_data ) >= sizeof( MutexData ) );
		new ( (void*) mutex.m_data ) MutexData;
	}

	//==========================================================================================================================================
	void Destroy( Mutex & mutex ) {
		( ( MutexData * ) mutex.m_data )->~MutexData();
	}

	//==========================================================================================================================================
	void Lock( Mutex & mutex ) {
		( ( MutexData * ) mutex.m_data )->mutex.lock();
	}

	//==========================================================================================================================================
	bool TryLock( Mutex & mutex ) {
		return ( ( MutexData * ) mutex.m_data )->mutex.try_lock();
	}

	//==========================================================================================================================================
	void Unlock( Mutex & mutex ) {
		( ( MutexData * ) mutex.m_data )->mutex.unlock();
	}

}}