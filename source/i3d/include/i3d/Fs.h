/*
==========================================================================================================================================
CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
Copyright (C) 2021 James Steele. All Rights Reserved.
==========================================================================================================================================
*/

#ifndef __I3D_FS_H__
#define __I3D_FS_H__

#include "i3d/Platform.h"

namespace i3d {

	class File {
	public:
		virtual ~File() {
			// Empty - does nothing
		}

		virtual size_t FileWrite( const void * buffer, size_t size, size_t count ) = 0;
		virtual size_t FileRead( void * buffer, size_t size, size_t count ) = 0;
		virtual size_t FileLength() = 0;
		virtual uintptr_t FilePos() = 0;
		virtual void FileSeek( uintptr_t pos ) = 0;
	};
}

namespace i3d { namespace  fs {

	static constexpr char MOUNT_DATA = '~';

	void Initialise( void);
	void Finalise( void );

	bool MakePathCanonoical( char * path );

	bool MakeFullPath( std::string & fullPath, const char * path );
	bool AppendPath( std::string &, const char * rhs );
	bool JoinPath( std::string &, const char * lhs, const char * rhs );

	char GetSeparator( void );
	char GetOtherSeparator( void );

	bool GetCurrentFolder( std::string & str );
	bool FileExists( const char * path );
	bool FolderExists( const char * path );
	bool CreateFolder( const char * path );
}}

#endif
