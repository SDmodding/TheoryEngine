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

/* (Important includes) Quark */

#include "quark/types.hh"
#include "quark/list.hh"
#include "quark/math.hh"
#include "quark/checksum.hh"
#include "quark/string.hh"