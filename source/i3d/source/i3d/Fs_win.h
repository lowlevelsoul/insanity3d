/*
==========================================================================================================================================
CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
Copyright (C) 2021 James Steele. All Rights Reserved.
==========================================================================================================================================
*/

#ifndef __I3D_FS_WIN_H__
#define __I3D_FS_WIN_H__

#include "i3d/Platform.h"
#include "i3d/Fs.h"

namespace i3d {

	class FileWin : public File {
	public:
		FileWin() {
			m_file = nullptr;
			m_open = 0;
		}

		~FileWin() {

		}

		virtual size_t FileWrite( const void * buffer, size_t size, size_t count ) override {
			assert( m_file != nullptr );
			return fwrite( buffer, size, count, m_file );
		}

		virtual size_t FileRead( void * buffer, size_t size, size_t count ) override {
			assert( m_file != nullptr );
			return fread( buffer, size, count, m_file );
		}

		virtual size_t FileLength() {
			assert( m_file != nullptr );
			return _filelengthi64( _fileno( m_file ) );
		}

		virtual uintptr_t FilePos() {
			assert( m_file != nullptr );
			return _ftelli64( m_file );
		}

		virtual void FileSeek( uintptr_t pos ) {
			assert( m_file != nullptr );
			_fseeki64( m_file, pos, SEEK_SET );
		}

		FILE *		m_file;
		uint64_t	m_open : 1;
	};

	class FileSystem {
	public:
		static constexpr size_t MAX_FILES = 64;

		FileSystem();

		~FileSystem();

		void Initialise( void);
		void Finalise( void );

		File * FileOpen( const char * path, const char * mode );
		void FileClose( File *  self_ );
		size_t FileWrite( File *  self_, const void * buffer, size_t size, size_t count );
		size_t FileRead( File *  self_, void * buffer, size_t size, size_t count );
		size_t FileLength( File *  self_ );
		uintptr_t FilePos( File *  self_ );
		void FileSeek( File *  self_, uintptr_t pos );

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

	protected:
		Mutex		m_mutex;
		File		m_files[ MAX_FILES ];

	};
}


#endif
