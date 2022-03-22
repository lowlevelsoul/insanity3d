//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/core/FileSystem.h"
#include "i3d/core/FileSystem_common.h"

namespace i3d {

    //=========================================================================================================================================
    static stl::String::size_type _getLastSeperator(const stl::String::type & path) {
        
        stl::String::size_type lastSepPos = path.rfind( fs->GetPlatformSeparator() );
        stl::String::size_type lastOtherSepPos = path.rfind( fs->GetOtherSeparator() );
            
        stl::String::size_type actualLastPos = stl::String::type::npos;
        
        if ( lastSepPos == stl::String::npos && lastOtherSepPos == stl::String::npos ) {
            // No path separator, so just set the position to be invalid
            actualLastPos = stl::String::npos;
        }
        else if ( lastSepPos != stl::String::npos && lastOtherSepPos != stl::String::npos ) {
            // Found a path separator of both types, so use the position of the one nearest the end of the string
            actualLastPos = lastSepPos > lastOtherSepPos ? lastSepPos : lastOtherSepPos;
        } else {
            // Only found a path seperator of one type - so determine which one we founf and use its position
            actualLastPos = lastSepPos != stl::String::npos ? lastSepPos : lastOtherSepPos;
        }

        return actualLastPos;
    }

    //=========================================================================================================================================
    static stl::String::size_type _getExtensionSeperator( const stl::String::type & path) {        
        stl::String::size_type extSepPos = path.rfind('.');
        return extSepPos;
    }

    //=========================================================================================================================================
    bool FileSystemCommon::GetFilename( stl::String::type & filename, const stl::String::type & path ) {
        
        stl::String::size_type lastSepPos = path.rfind( GetPlatformSeparator() );
        stl::String::size_type lastOtherSepPos = path.rfind( GetOtherSeparator() );
        
        stl::String::size_type filenameStart = stl::String::npos;
        
        if (lastSepPos == stl::String::npos && lastOtherSepPos == stl::String::npos) {
            // Could not find a path separator - so just assume that the
            // entire path is the filename
            filename = path;
            return true;
        }
        else if (lastSepPos != stl::String::npos && lastOtherSepPos != stl::String::npos) {
            // We found both a platform  folder separator and non-platform separator - so
            // choose the position closest to the end of the string
            filenameStart = (lastSepPos > lastOtherSepPos) ? lastSepPos : lastOtherSepPos;
        } else {
            // We only found one type of seperator - so pick that as the start of the
            // filename
            filenameStart = (lastSepPos != stl::String::npos) ? lastSepPos : lastOtherSepPos;
        }
        
        // filenameStart is currently the location of the seperator, so advance by one
        // to get the start of the filename
        ++filenameStart;
        
        if (filenameStart >= path.size()) {
            // If the separator is at or past the end of the string, we can't get a filename
            // so clear the return string and return with false
            filename.resize(0);
            return false;
        }
        
        filename = path.substr( filenameStart, path.size() - filenameStart );
        return true;
    }



    //=========================================================================================================================================
    bool FileSystemCommon::ExtractFilename( stl::String::type & filename, stl::String::type & path ) {
        
      bool gotFilename = GetFilename(filename, path);
      if (gotFilename == true) {
          path.resize( path.size() - filename.size() );
      }
      
      return gotFilename;
    }

    //=========================================================================================================================================
    bool FileSystemCommon::HasExtension( const stl::String::type & path ) {
      stl::String::size_type lastSepPos = _getLastSeperator(path);
      if (lastSepPos == stl::String::npos) {
          lastSepPos = 0;
      }
      
      stl::String::size_type extSepPos = path.find('.', lastSepPos);
      return (extSepPos != stl::String::npos);
    }



    //=========================================================================================================================================
    bool FileSystemCommon::GetExtension( stl::String::type & ext, const stl::String::type & path ) {
        stl::String::size_type extSepPos = _getExtensionSeperator(path);
        if (extSepPos == stl::String::npos) {
            ext.resize(0);
            return false;
        }

        // Extension seperator position is at the preiod char - but we don;t want that.
        // So bump the position and check we're not at or past the end of the string.
        ++extSepPos;
        if (extSepPos >= path.size()) {
            ext.resize(0);
            return false;
        }
        
        stl::String::type tmp = path.substr(extSepPos, (path.size() - extSepPos) );
        ext = tmp;
        return true;
    }

    //=========================================================================================================================================
    bool FileSystemCommon::RemoveExtension( stl::String::type & path ) {
        stl::String::size_type extSepPos = _getExtensionSeperator(path);
        if (extSepPos == stl::String::npos) {
          // No extension to remove!
          return false;
        }

        path = path.substr( 0, extSepPos );
        return true;
    }

    //=========================================================================================================================================
    bool FileSystemCommon::HasExtensionType( const stl::String::type & path, const char* ext ) {
        stl::String::size_type extPos = _getExtensionSeperator(path);
        if (extPos == stl::String::npos) {
            return false;
        }

        // extPos is at the period seperator for the extension, so move past it.
        ++extPos;

        // Good 'ol strcmp to compare the extension string
        int res = strcmp( path.c_str()+extPos, ext );
        return ( res == 0 );
    }

    //=========================================================================================================================================
    void FileSystemCommon::StripExtension( stl::String::type & path ) {
        stl::String::size_type extPos = _getExtensionSeperator(path);
        if (extPos == stl::String::npos) {
          return;
        }

        path = path.substr( 0, extPos );
    }

    //=========================================================================================================================================
    bool FileSystemCommon::AppendExtension( stl::String::type & path, const char* ext ) {
        bool pathHasExtSep = (path.back() == '.');
        bool extHasSep = ext[0] == '.';

        if ( pathHasExtSep == true && extHasSep == true ) {
          // Both the path and the extension have an extension seperator, so just trim the
          // separator from the extension and append it to the path
          path.append( ext + 1 );
        } if ( pathHasExtSep == false && extHasSep == false ) {
          // Have to ad an extension separator
          path.append( "." );
          path.append( ext );
        } else {
          // Either the path or the extension has a separator, so we can append normally.
          path.append( ext );
        }

        return true;
    }

    //=========================================================================================================================================
    bool FileSystemCommon::AppendPath( stl::String::type & lhs, const char* rhs ) {
        bool lhsHasSep = lhs.back() == '\\' || lhs.back() == '/';
        bool rhsHasSep = rhs[0] == '\\' || rhs[0] == '/';

        char platformSepStr[ 2 ] = { GetPlatformSeparator(), 0 };
        
        if ( lhs.empty() == true ) {
            // The lhs string is empty - so we assume that the path doesn't need to be
            // appended (it's either relative or absolute).
            lhs = rhs;
        }
        else if ( lhsHasSep == true && rhsHasSep == true )  {
            lhs.append( rhs + 1 );
        }
        else if ( lhsHasSep == false && rhsHasSep == false ) {
            lhs.append( platformSepStr );
            lhs.append( rhs );
        }
        else {
            lhs.append( rhs );
        }

        return true;
    }

    //=========================================================================================================================================
    bool FileSystemCommon::JoinPath( stl::String::type & path, const char* lhs, const char* rhs ) {
        path = lhs;
        return AppendPath( path, rhs );
    }

    //=========================================================================================================================================
    void FileSystemCommon::MakePathCononical( stl::String::type & path ) {
        char platformSep = GetPlatformSeparator();
        char otherSep = GetOtherSeparator();
        
        for( stl::String::iterator c = path.begin(); c != path.end(); ++c ) {
            if ( *c == otherSep ) {
                *c = platformSep;
            }
        }
    }
}
