/*
==========================================================================================================================================
CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
Copyright (C) 2021 James Steele. All Rights Reserved.
==========================================================================================================================================
*/

#include "i3d/Sys.h"
#include "i3d/String.h"
#include <string.h>

typedef struct StringHeader_s {
    uint64_t        marker;
    size_t          length;
    size_t          capacity;
    void *          mem;
} StringHeader_t;

/* Minimum string capacity size is 64-bytes */
#define STR_MIN_CAPACITY 64
#define STR_STRUCT_SIZE sizeof(StringHeader_t)
#define STR_ALIGN 64
#define STR_MARKER 0xb00b135

#define Str_getBufferAddr(STR) (char*) ((((uintptr_t)(STR)) + STR_STRUCT_SIZE))
#define Str_getStructAddr(STR) (StringHeader_t*) (((uintptr_t)(STR)) - STR_STRUCT_SIZE)

/*======================================================================================================================================*/
static size_t Str_RoundCapacity( size_t wantedCapacity ) {
    wantedCapacity = (wantedCapacity < STR_MIN_CAPACITY) ? STR_MIN_CAPACITY : wantedCapacity;
    wantedCapacity = Sys_Align(wantedCapacity, 4);
    return wantedCapacity;
}

/*======================================================================================================================================*/
static StringHeader_t * _Str_Alloc( size_t length ) {
    
    /* Calculate total size for allocation with enough space to leave for
       alignment purposes. Note: adding 1 for terminating zero. */
    size_t capacity = Str_RoundCapacity( length + 1);
    size_t totalAllocSize = capacity + Sys_Align(STR_STRUCT_SIZE, STR_ALIGN);
    
    /* Allocate memory and align string start address */
    uintptr_t memAddr = (uintptr_t) malloc( totalAllocSize );
    uintptr_t strStartAddr = memAddr + STR_STRUCT_SIZE;
    strStartAddr = Sys_Align(strStartAddr, STR_ALIGN );
    
    uintptr_t structAddr = strStartAddr - STR_STRUCT_SIZE;
    assert( structAddr >= memAddr);
    
    /* Get address of string struct and fill it in */
    StringHeader_t * str = (StringHeader_t*) structAddr;
    str->capacity = capacity;
    str->length = length;
    str->marker = STR_MARKER;
    str->mem = (void*) memAddr;
    
    /* Add a null terminator to the end of the string */
    char * strStart = (char *) strStartAddr;
    strStart[length] = 0;
    
    return str;
}

/*======================================================================================================================================*/
void _Str_Free( StringHeader_t * str ) {
    assert( str != NULL );
    free( str->mem );
}

/*======================================================================================================================================*/
static StringHeader_t * _Str_Resize( StringHeader_t * str, size_t newSize, bool_t doCopy ) {
    if ( str->capacity < newSize + 1 ) {
        /* String is not large enough for the new size, so create a new string */

        StringHeader_t * oldStr = str;
        str = _Str_Alloc( newSize );
        
        if ( doCopy == true && str->length != 0 ) {
            /* Old string is not zero, so copy over the contents */
            char * dstBuff = Str_getBufferAddr( str );
            char * srcBuff = Str_getBufferAddr( oldStr );
            strncpy( srcBuff, dstBuff, oldStr->length);
            str->length = oldStr->length;
        }
        
        _Str_Free( oldStr );
    }
    else {
        /* String is large ebough to hold the new size, so just adjust the length */
        char * strBuff = Str_getBufferAddr(str);
        str->length = newSize;
        strBuff[newSize] = 0;
    }
    
    return str;
}

/*======================================================================================================================================*/
char * Str_Create( size_t length ) {
    StringHeader_t * str = _Str_Alloc( length );
    return Str_getBufferAddr( str );
}

/*======================================================================================================================================*/
void Str_Free( char * rhs ) {
    if ( rhs == NULL ) {
        return;
    }
    
    StringHeader_t * str = Str_getStructAddr( rhs );
    assert(str->marker == STR_MARKER );
    
    _Str_Free( str );
}

/*======================================================================================================================================*/
void Str_Resize( String_t * rhs, size_t capacity ) {
    if ( *rhs == NULL ) {
        /* Dest string is NULL, so just create a new string */
        *rhs = Str_Create( capacity );
    }
    else {
        /* Resize existing string */
        StringHeader_t * header = Str_getStructAddr( *rhs );
        header = _Str_Resize( header, capacity, true );
        *rhs = Str_getBufferAddr( header );
    }
}

/*======================================================================================================================================*/
void Str_Reserve( String_t * rhs, size_t capacity ) {
    StringHeader_t * header;
    StringHeader_t * newStr;
    char * newStrBuffer;

    if ( *rhs == NULL ) {
        /* String is NULL, we want to create a zero length string 
           but reserve a chunk of memory for expansion */
        newStrBuffer = Str_Create( capacity );

        newStr = Str_getStructAddr( newStrBuffer );
        newStr->length = 0;
        newStrBuffer[ 0 ] = 0;

        *rhs = newStrBuffer;
        return;
    }

    size_t currLen = Str_Length( *rhs );
    if ( capacity < currLen ) {
        /* We want to reserve a chunk of memory that's smaller than the existing string        
            so don't bother doing anything.
        */
        return;
    }

    header = Str_getStructAddr( *rhs );
    newStr = _Str_Resize( header, capacity, true );
    *rhs =  Str_getBufferAddr( newStr );

    (*rhs)[ currLen ] = 0;
    newStr->length = currLen;
}

/*======================================================================================================================================*/
void Str_Copy( String_t * lhs, const char * rhs ) {
    StringHeader_t * lhsStr = NULL;
    
    if ( *lhs == NULL ) {
        /* Dest string is null, so create a string to copy into */
        assert( rhs != NULL );
        size_t rhsLen = strlen( rhs );
        lhsStr = _Str_Alloc( rhsLen );
    }
    else {
        size_t rhsLen = strlen( rhs );
        lhsStr = Str_getStructAddr( *lhs );
        assert(lhsStr->marker == STR_MARKER );
        
        if (lhsStr->capacity < rhsLen + 1) {
            /* Dest string is too small, so free it and create a new one */
            _Str_Free(lhsStr);
            lhsStr = _Str_Alloc( rhsLen );
        }
        else {
            /* Dest string has enough space to hold the string, so just modify the
              the string length */
            lhsStr->length = rhsLen;
        }
    }
    
    /* Copy over the string and return with the string buffer address */
    char * lhsBuffer = Str_getBufferAddr( lhsStr );
    strcpy( lhsBuffer, rhs);
    
    *lhs = lhsBuffer;
}

/*======================================================================================================================================*/
size_t Str_Length( char * rhs ) {
    StringHeader_t * rhsStr;
    
    if ( rhs == NULL ) {
        return 0;
    }
    
    rhsStr = Str_getStructAddr(rhs);
    assert( rhsStr->marker == STR_MARKER );
    return rhsStr->length;
}

/*======================================================================================================================================*/
void Str_Concat( String_t * dst, const char * rhs1, const char * rhs2 ) {
    size_t rhs1Len, rhs2Len, totalLen;
    StringHeader_t * dstStr;
    
    assert( rhs1 != NULL );
    assert( rhs2 != NULL );
    
    rhs1Len = strlen(rhs1);
    rhs2Len = strlen(rhs2);
    totalLen = rhs1Len + rhs2Len;
    
    if ( *dst == NULL ) {
        /* Destination string is NULL, so just create a new string */
        dstStr = _Str_Alloc( totalLen );       
    }
    else {
        /* Destination string is no not null, so check to see if
           it's big enough to hold the string.
        */
        
        dstStr = Str_getStructAddr( *dst );
        assert( dstStr->marker == STR_MARKER );
        
        if ( totalLen + 1 > dstStr->capacity ) {
            dstStr = _Str_Resize( dstStr, totalLen, false );
        }
    }
    
    /* Get the buffer pointer for the destination string again as the
       string may have been newlwy allocated */
    *dst = Str_getBufferAddr( dstStr );
    
    /* Copy over the two strings we're joining and ensure we set the string length and terminating zero */
    strncpy( *dst, rhs1, rhs1Len );
    strncpy( *dst + rhs1Len, rhs2, rhs2Len );
    
    *dst[ totalLen ] = 0;
    dstStr->length = totalLen;
}

/*======================================================================================================================================*/
void Str_Append( String_t * lhs, const char * rhs ) {
    StringHeader_t * lhsStr;
    size_t rhsLen, totalLen;
   
    if ( rhs == NULL) {
        return;
    }
   
    rhsLen = strlen( rhs );
    if ( rhsLen == 0 ) {
        return;
    }
    
    if ( *lhs == NULL ) {
        /* Destination is null, so just make a copy of rhs */
        Str_Copy( lhs, rhs );
        return;
    }
  
    /* Check to see if the destination is large enough to hold the string. If not,
       resize it to fit. */
    lhsStr = Str_getStructAddr( *lhs );
    assert(lhsStr->marker == STR_MARKER );
    
    totalLen = lhsStr->length + rhsLen ;
    
    if (lhsStr->capacity < totalLen + 1) {
        lhsStr = _Str_Resize( lhsStr, totalLen, true );
        *lhs = Str_getBufferAddr( lhsStr );
    }
    
    /* Append the string to the end of the existing string */
    strncpy( (*lhs) + lhsStr->length, rhs, rhsLen );
    (*lhs)[totalLen] = 0;
    lhsStr->length = totalLen;
}