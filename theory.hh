/*
*----------------------------------------------------------------------
*	 Theory Engine / https://github.com/SDmodding/TheoryEngine
*	===========================================================
*	- Includes everything necessary for working with the engine.
*	- This is not a complete re-write of the engine.
*	- You will need to use at least 'C++17' for building.
*	- Based on "Sleeping Dogs: Definitive Edition"
* ----------------------------------------------------------------------
* 
*	REQUIREMENTS:
*		- C++17
*		- 64-bit (x64)
*			- Should be buildable under x86, but not recommended.
* 
*	BEFORE INCLUDING:
*		#define THEORY_IMPL
*		-This is necessary, otherwise no functions will be implemented.
* 
*	CREDITS:
*		sneakyevil		-	Lead Developer
* 
*/
#pragma once

#define THEORY_PLATFORM_PC

/* Libraries */
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>

#ifdef THEORY_PLATFORM_PC
	#include <d3d11.h>
#endif


//-------------------------------------------------------------------
// [Quark] Includes
//-------------------------------------------------------------------

#include "quark/types.hh"
#include "quark/offset.hh"
#include "quark/list.hh"
#include "quark/treerb.hh"
#include "quark/math.hh"
#include "quark/processplat.hh"

#ifdef THEORY_PLATFORM_PC
	#include "quark/pc/stringplat.hh"
#endif

#include "quark/checksum.hh"
#include "quark/memorypool.hh"
#include "quark/string.hh"
#include "quark/resource.hh"
#include "quark/resource_uid.hh"
#include "quark/quark.hh"

//-------------------------------------------------------------------
// [Illusion] Includes
//-------------------------------------------------------------------

#include "illusion/memimageschema.hh"

#ifdef THEORY_PLATFORM_PC
	#include "illusion/pc/modelplat.hh"
#endif

#include "illusion/model.hh"

#include "illusion/internal/factoryplat.hh"