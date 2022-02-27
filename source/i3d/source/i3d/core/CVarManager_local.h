//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//==========================================================================================================================================

#ifndef __I3D_CVARMANAGER_LOCAL_H__
#define __I3D_CVARMANAGER_LOCAL_H__

#include "i3d/core/Types.h"
#include "i3d/core/CVar.h"

namespace i3d {

    class CVarManagerLocal : public CVarManager {
    public:
        CVarManagerLocal();

        virtual ~CVarManagerLocal();

        void Initialise();

        virtual CVar * Find( const char * name ) override;

        virtual void PublishCVars( CVar * list ) override;

    protected:

        void InitCVar( CVar * var );

        virtual void * Alloc( size_t size ) override;

        virtual void Free( void * mem ) override;

    protected:
        typedef stl::Map<uint64_t, CVar*> cvar_map_t;
        cvar_map_t::type  m_cvars;
    };
}

#endif
