/* 
*	This file handles defines for resource type uids & chunk uids for now.
*	- If there is way to calculate them from string then this should be rewritten.
*/
#pragma once

#ifdef THEORY_PLATFORM_PC /* PC UIDs */

#define RTYPE_Buffer						0x92CDEC8F
#define RTYPE_Buffer_Chunk					0x7A971479

#define RTYPE_BufferD3DResource				0x171246AD
#define RTYPE_BufferD3DResource_Chunk		0x45E061F6

#endif