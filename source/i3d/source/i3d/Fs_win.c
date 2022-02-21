/*
==========================================================================================================================================
CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
Copyright (C) 2021 James Steele. All Rights Reserved.
==========================================================================================================================================
*/

#include "i3d/Fs.h"
#include "i3d/Mutex.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <io.h>
#include <Shlwapi.h>

#define FS_MAX_FILES 64

typedef struct FileInfo_s {
	uint64_t	isNormalFile : 1;
	uint64_t	isArchive : 1;
	uint64_t	isOpen : 1;
	FILE *		file;	
} FileInfo_t;

typedef struct fs_s {
	String_t		basePath;
	String_t		dataPath;
	String_t		tempPath;

	FileInfo_t		fileInfo[ FS_MAX_FILES ];

	Mutex_t			mutex;
} fs_t;


fs_t fsInstance;
fs_t * fs = NULL;

/*======================================================================================================================================*/
static ihandle_t _FS_GetFreeFile( void ) {
	for ( uint32_t i = 0; i < FS_MAX_FILES; ++i ) {
		if ( fs->fileInfo[ i ].isOpen == 0 ) {
			return i;
		}
	}

	return IHANDLE_INVALID;
}


/*======================================================================================================================================*/
void FS_Initialise( void ) {
	if ( fs != NULL ) {
		return;
	}

	memset( &fsInstance, 0, sizeof( fs ) );
	fs = &fsInstance;
	
	Str_Reserve( &fs->basePath, 2048 );
	Str_Reserve( &fs->dataPath, 2048 );
	Str_Reserve( &fs->tempPath, 2048 );

	FS_GetCurrentFolder( &fs->basePath );
	FS_JoinPath( &fs->dataPath, fs->basePath, "\\data" );

	Mutex_Create( &fs->mutex );

}

/*======================================================================================================================================*/
void FS_Finalise( void ) {
	if ( fs == NULL ) {
		return;
	}

	Mutex_Destroy( &fs->mutex );

	Str_Free( fs->basePath );
	Str_Free( fs->dataPath );
	Str_Free( fs->tempPath );

	fs = NULL;
}

/*======================================================================================================================================*/
ihandle_t FS_FileOpen( const char * path, const char * mode ) {
	FileInfo_t * fileInfo;
	ihandle_t hfile;
	FILE * file;

	/* Look for a free file item */
	hfile = _FS_GetFreeFile();
	assert( hfile != IHANDLE_INVALID );

	fileInfo = &fs->fileInfo[ hfile ];
	
	/* We're messing about with the global state of the file system, s lock*/
	Mutex_Lock( &fs->mutex );

	/* Get the fill path into the temp buffer and try to open the file */
	FS_MakeFullPath( &fs->tempPath, path );

	file = fopen( fs->tempPath, mode );
	if ( file == NULL ) {
		/* No file - so bail with error */
		Mutex_Unlock( &fs->mutex );
		return IHANDLE_INVALID;
	}

	/* File was opened ok - set file info and return with handle */
	fileInfo->isOpen = 1;
	fileInfo->file = file;

	Mutex_Unlock( &fs->mutex );

	return hfile;
}

/*======================================================================================================================================*/
void FS_FileClose( ihandle_t self_ ) {
	assert( self_ <= FS_MAX_FILES );
	
	Mutex_Lock( &fs->mutex );
	if ( fs->fileInfo[ self_ ].isOpen == 1 ) {
		fclose( fs->fileInfo[ self_ ].file );
		fs->fileInfo[ self_ ].file = NULL;
		fs->fileInfo[ self_ ].isOpen = 0;

	}
	Mutex_Unlock( &fs->mutex );
}

/*======================================================================================================================================*/
size_t FS_FileWrite( ihandle_t self_, const void * buffer, size_t size, size_t count ) {
	FileInfo_t * info = &fs->fileInfo[ self_ ];

	assert( self_ <= FS_MAX_FILES );
	assert( info->isOpen == 1 );

	return fwrite( buffer, size, count, info->file );
}

/*======================================================================================================================================*/
size_t FS_FileRead( ihandle_t self_, void * buffer, size_t size, size_t count ) {
	FileInfo_t * info = &fs->fileInfo[ self_ ];

	assert( self_ <= FS_MAX_FILES );
	assert( info->isOpen == 1 );

	return fread( buffer, size, count, info->file );
}

/*======================================================================================================================================*/
size_t FS_FileLength( ihandle_t self_ ) {
	FileInfo_t * info = &fs->fileInfo[ self_ ];

	assert( self_ <= FS_MAX_FILES );
	assert( info->isOpen == 1 );

	return _filelengthi64( _fileno( info->file ) );
}

/*======================================================================================================================================*/
uintptr_t FS_FilePos( ihandle_t self_ ) {
	FileInfo_t * info = &fs->fileInfo[ self_ ];

	assert( self_ <= FS_MAX_FILES );
	assert( info->isOpen == 1 );

	return _ftelli64( info->file );
}

/*======================================================================================================================================*/
void FS_Seek( ihandle_t self_, uintptr_t pos ) {
	FileInfo_t * info = &fs->fileInfo[ self_ ];

	assert( self_ <= FS_MAX_FILES );
	assert( info->isOpen == 1 );

	_fseeki64( info->file, pos, SEEK_SET );
}

/*======================================================================================================================================*/
bool_t FS_MakeFullPath( String_t * fullPath, const char * path ) {
	assert( path != NULL );

	if ( path[ 0 ] == FS_MOUNT_DATA ) {
		FS_JoinPath( fullPath, fs->dataPath, path + 1 );
	}
	else {
		Str_Copy( fullPath, path );
	}

	FS_MakePathCanonoical( *fullPath );

	return true;
}
/*======================================================================================================================================*/
bool_t FS_MakePathCanonoical( char * path ) {
	if ( path == NULL ) {
		return true;
	}

	char wrongSep = FS_GetOtherSeparator();
	char rightSep = FS_GetSeparator();

	while ( *path != 0 ) {
		if ( *path == wrongSep ) {
			*path = rightSep;
		}
		++path;
	}

	return true;
}

/*======================================================================================================================================*/
char FS_GetSeparator( void ) {
	return '\\';
}

/*======================================================================================================================================*/
char FS_GetOtherSeparator( void ) {
	return '/';
}

/*======================================================================================================================================*/
bool_t FS_GetCurrentFolder( String_t * str ) {
	DWORD len = GetCurrentDirectoryA( 0, NULL );
	Str_Resize( str, len );
	GetCurrentDirectoryA( len + 1, *str );
	return true;
}

/*======================================================================================================================================*/
bool_t FS_FileExists( const char * path ) {
	Mutex_Lock( &fs->mutex );
	FS_MakeFullPath( &fs->tempPath, path );

	BOOL exists = PathFileExistsA( fs->tempPath );
	Mutex_Unlock( &fs->mutex );

	return ( exists == TRUE ) ? true : false;
}

/*======================================================================================================================================*/
bool_t FS_FolderExists( const char * path ) {
	Mutex_Lock( &fs->mutex );
	FS_MakeFullPath( &fs->tempPath, path );

	BOOL exists = PathIsDirectoryA( fs->tempPath );
	Mutex_Unlock( &fs->mutex );

	return ( exists == TRUE ) ? true : false;
}

/*======================================================================================================================================*/
bool_t FS_CreateFolder( const char * path ) {
	return false;
}
