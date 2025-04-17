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
*		- This is necessary, otherwise no functions will be implemented.
* 
*	CREDITS:
*		sneakyevil		-	Lead Developer
* 
*/
#pragma once

//==================================================================
//	Optional Defines
//==================================================================
/*
**	THEORY_DUCKTAPE
**	- Use "duck tape" version implementation for missing stuff.
*	- As example: 'qBaseTreeRB' uses 'std::unordered_map'.
* 
*	THEORY_TRIADWARS
*	- Implements more checks to support Triad Wars stuff.
* 
*	THEORY_PRINTCHANNEL_DISABLE
*	- Disable usage of print channels.
* 
*	THEORY_PRINTCHANNEL_FILE
*	- Enables usage of file for print channel.
* 
*	THEORY_QSYMBOL_TABLE_INVENTORY
*	- Enables qSymbolTableResourceInventory to be initialized.
*/
//==================================================================

//--------------------------------------------------
//	Platform Defines
//--------------------------------------------------

#define THEORY_WINDOWS
#define THEORY_PLATFORM_PC

//--------------------------------------------------
//	Keyword Defines
//--------------------------------------------------

#define THEORY_SINLINE	static inline
#define THEORY_INLINE	__forceinline

//--------------------------------------------------
//	Libraries
//--------------------------------------------------

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>

#ifdef THEORY_DUCKTAPE

#include <unordered_map>
#include <map>

#endif

#ifdef THEORY_PLATFORM_PC
	#include <d3d11.h>
#endif

//--------------------------------------------------
//	Defines
//--------------------------------------------------

#ifdef THEORY_PLATFORM_PC
	#define qDebugBreak()		__debugbreak()
#else
	#define qDebugBreak()		(void)(0)
#endif

#define qMin(a, b)				(a < b ? a : b)
#define qMax(a, b)				(a > b ? a : b)

//==================================================================
//	Includes
//==================================================================

#include "extras/resource_uid.hh"

//--------------------------------------------------
//	Quark
//--------------------------------------------------

#include "quark/types.hh"
#include "quark/array.hh"
#include "quark/assert.hh"
#include "quark/bitflags.hh"
#include "quark/colour.hh"
#include "quark/offset.hh"
#include "quark/list.hh"
#include "quark/treerb.hh"
#include "quark/treerb64.hh"
#include "quark/math.hh"
#include "quark/halfmath.hh"
#include "quark/process.hh"
#include "quark/memory.hh"
#include "quark/string.hh"

#include "quark/internal/assertplat.hh"

#ifdef THEORY_PLATFORM_PC
	#include "quark/pc/processplat.hh"
	#include "quark/pc/stringplat.hh"
#endif

#include "quark/internal/allocator.hh"

#include "quark/checksum.hh"
#include "quark/memorypool.hh"
#include "quark/memorypoollinearallocator.hh"
#include "quark/timer.hh"
#include "quark/compresslz.hh"
#include "quark/file.hh"
#include "quark/printchannel.hh"
#include "quark/vramemorypool.hh"
#include "quark/resource.hh"
#include "quark/resourcefile.hh"
#include "quark/qsymbol.hh"
#include "quark/qwisesymbol.hh"
#include "quark/quark.hh"

#ifdef THEORY_PLATFORM_PC
	#include "quark/pc/fileplat.hh"
	#include "quark/pc/timerplat.hh"
#endif

//--------------------------------------------------
//	SimpleXML
//--------------------------------------------------

#include "simplexml/private/pugixml.hh"
#include "simplexml/simplexml.hh"

//--------------------------------------------------
//	Illusion
//--------------------------------------------------

#include "illusion/memimageschema.hh"
#include "illusion/state.hh"

#include "illusion/bonepalette.h"

#include "illusion/vertexdescriptor.hh"
#include "illusion/vertexdecl.hh"

#ifdef THEORY_PLATFORM_PC
	#include "illusion/pc/modelplat.hh"
#endif

#include "illusion/material.hh"
#include "illusion/materialuser.hh"
#include "illusion/materialmodifier.hh"
#include "illusion/model.hh"
#include "illusion/stateblock.hh"

#include "illusion/internal/factoryplat.hh"
#include "illusion/illusion.hh"

//--------------------------------------------------
//	PropertySet
//--------------------------------------------------

#include "propertyset/propertysymbols.hh"
#include "propertyset/qpropertyset.hh"
#include "propertyset/qpropertylist.hh"
#include "propertyset/propertysetcache.hh"
#include "propertyset/qpropertysetxml.hh"

//--------------------------------------------------
//	Streamer
//--------------------------------------------------

#include "streamer/bigfile.hh"
#include "streamer/streamermetrics.hh"
#include "streamer/streamfilewrapper.hh"
#include "streamer/streamresourceloader.hh"
#include "streamer/streamingmemorymanager.hh"
#include "streamer/datastreamer.hh"

//--------------------------------------------------
//	Object Resource Management
//--------------------------------------------------

#include "objectresourcemanagement/resourcerequest.hh"
#include "objectresourcemanagement/truecrowddefinition.hh"
#include "objectresourcemanagement/truecrowddatabase.hh"

//==================================================================
//	Implementations
//==================================================================

#ifdef THEORY_IMPL

//--------------------------------------------------
//	Quark
//--------------------------------------------------

#include "quark/resourcefile_impl.hh"
#include "quark/string_impl.hh"

//--------------------------------------------------
//	SimpleXML
//--------------------------------------------------

#include "simplexml/private/pugixml_impl.hh"
#include "simplexml/simplexml_impl.hh"

//--------------------------------------------------
//	Illusion
//--------------------------------------------------

#include "illusion/material_impl.hh"

//--------------------------------------------------
//	PropertySet
//--------------------------------------------------

#include "propertyset/qpropertyset_impl.hh"

#endif