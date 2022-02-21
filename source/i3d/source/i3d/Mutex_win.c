/*
==========================================================================================================================================
CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
Copyright (C) 2021 James Steele. All Rights Reserved.
==========================================================================================================================================
*/

#include "i3d/Mutex.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <io.h>

/*======================================================================================================================================*/
void Mutex_Create( Mutex_t * self_ ) {
	assert( sizeof( self_->data ) <= sizeof( CRITICAL_SECTION ) );
	InitializeCriticalSection( ( CRITICAL_SECTION * ) self_->data );
}

/*======================================================================================================================================*/
void Mutex_Destroy( Mutex_t * self_ ) {
	DeleteCriticalSection( ( CRITICAL_SECTION * ) self_->data );
}

/*======================================================================================================================================*/
void Mutex_Lock( Mutex_t * self_ ) {
	EnterCriticalSection( ( CRITICAL_SECTION * ) self_->data );
}

/*======================================================================================================================================*/
bool_t Mutex_Tryock( Mutex_t * self_ ) {
	BOOL res = TryEnterCriticalSection( ( CRITICAL_SECTION * ) self_->data );
	return ( res == TRUE ) ? true : false;
}

/*======================================================================================================================================*/
void Mutex_Unlock( Mutex_t * self_ ) {
	LeaveCriticalSection( ( CRITICAL_SECTION * ) self_->data );
}