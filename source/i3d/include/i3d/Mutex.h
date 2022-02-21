/*
==========================================================================================================================================
CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
Copyright (C) 2021 James Steele. All Rights Reserved.
==========================================================================================================================================
*/

#ifndef __I3D_MUTEX_H__
#define __I3D_MUTEX_H__

#include "i3d/Platform.h"

namespace i3d {
	struct Mutex {
		static constexpr size_t DATA_WORD_COUNT = 4;
		uint64_t		m_data;
	};
}

namespace i3d { namespace mutex {

	void Create( Mutex & mutex );
	void Destroy( Mutex & mutex );
	void Lock( Mutex & mutex );
	bool TryLock( Mutex & mutex );
	void Unlock( Mutex & mutex );
}}

namespace i3d {
	class ScopedLock {
	public:
		ScopedLock( Mutex & rhs ) {
			m_mutex = &rhs;
			mutex::Lock( *m_mutex );
		}

		~ScopedLock() {
			mutex::Unlock( *m_mutex );
		}

	protected:
		Mutex * m_mutex;
	};
}


#endif
