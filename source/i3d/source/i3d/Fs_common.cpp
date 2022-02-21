/*
==========================================================================================================================================
CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
Copyright (C) 2021 James Steele. All Rights Reserved.
==========================================================================================================================================
*/

#include "i3d/Fs.h"

namespace i3d { namespace fs {

	//==========================================================================================================================================
	bool AppendPath( std::string & lhs, const char * rhs ) {
		if ( rhs == nullptr || rhs[ 0 ] == 0 ) {
			return true;
		}

		bool lhsSep = ( lhs.back() == GetSeparator() || lhs.back() == GetOtherSeparator() );
		bool rhsSep = ( rhs[0] == GetSeparator() || rhs[0] == GetOtherSeparator() );

		if ( lhsSep == rhsSep ) {
			// Both paths either have a separator or no separator
			if ( lhsSep == true ) {
				lhs.append( rhs );
			}
			else {
				lhs.push_back( GetSeparator() );
				lhs.append( rhs + 1 );
			}
		}
		else {
			// One of the paths has a separator, so we can just append as noma;
			lhs.append( rhs );
		}

		return true;
	}

	//==========================================================================================================================================
	bool JoinPath( std::string & dst, const char * lhs, const char * rhs ) {
		if ( lhs != nullptr ) {
			dst = lhs;
		}

		if ( rhs != nullptr ) {
			AppendPath( dst, rhs );		
		}

		return true;
	}
}}