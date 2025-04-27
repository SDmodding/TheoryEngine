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
//
//	THEORY_DUCKTAPE
//	- Use "duck tape" version implementation for missing stuff.
//	- As example: 'qBaseTreeRB' uses 'std::unordered_map'.
// 
//	THEORY_DISABLE_ASSERT
// 
//	THEORY_TRIADWARS
//	- Implements more checks to support Triad Wars stuff.
// 
//	THEORY_PRINTCHANNEL_DISABLE
//	- Disable usage of print channels.
// 
//	THEORY_PRINTCHANNEL_FILE
//	- Enables usage of file for print channel.
// 
//	THEORY_QSYMBOL_TABLE_INVENTORY
//	- Enables qSymbolTableResourceInventory to be initialized.
// 
//	THEORY_SLIM_BUILD
//	- Disabled majority of implementation.
// 
//	THEORY_IMPL_ACTIONTREE
//	THEORY_IMPL_BIGFILE
//	THEORY_IMPL_PROPERTYSET_INVENTORY
//
//==================================================================

#ifndef THEORY_SLIM_BUILD
#ifndef THEORY_IMPL_ACTIONTREE
	#define THEORY_IMPL_ACTIONTREE
#endif
#ifndef THEORY_IMPL_BIGFILE
	#define THEORY_IMPL_BIGFILE
#endif
#ifndef THEORY_IMPL_PROPERTYSET_INVENTORY
	#define THEORY_IMPL_PROPERTYSET_INVENTORY
#endif
#endif

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

#include "extras/forwards.hh"
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
#include "quark/safepointer.hh"
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
//	Action Tree (Dependencies)
//--------------------------------------------------

#include "actiontree/binarray.hh"
#include "actiontree/binptrarray.hh"
#include "actiontree/binstring.hh"

#include "actiontree/actionid.hh"
#include "actiontree/enums.hh"

//--------------------------------------------------
//	UEL
//--------------------------------------------------

#include "uel/uel.hh"

//--------------------------------------------------
//	Expression
//--------------------------------------------------

#include "expression/membermap.hh"

//--------------------------------------------------
//	Action Tree
//--------------------------------------------------

#ifdef THEORY_IMPL_ACTIONTREE

#include "actiontree/condition.hh"
#include "actiontree/track.hh"
#include "actiontree/task.hh"

#include "actiontree/actionnode.hh"
#include "actiontree/tracklibrary.hh"

#include "actiontree/conditionsgame.hh"

#include "actiontree/trackscore.hh"
#include "actiontree/trackscoregame.hh"

#include "actiontree/tracksanimation.hh"

#include "actiontree/trackscamera.hh"
#include "actiontree/taskscamera.hh"

#include "ai/stimulusbank.hh"
#include "nis/nisnodes.hh"

#include "actiontree/actiontreeresource.hh"

#endif

//--------------------------------------------------
//	Render
//--------------------------------------------------

#include "render/render.hh"
#include "render/renderworldstate.hh"

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

//--------------------------------------------------
//	Main
//--------------------------------------------------

#include "main/mainloop.hh"

//==================================================================
//	Implementations
//==================================================================

#ifdef THEORY_IMPL

#ifdef THEORY_IMPL_ACTIONTREE
#include "actiontree/actiontree_impl.hh"
#endif

//--------------------------------------------------
//	Quark
//--------------------------------------------------

#include "quark/quark_impl.hh"
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