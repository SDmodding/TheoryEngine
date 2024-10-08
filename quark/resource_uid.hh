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

#define RTYPE_Material						0xB4C26312
#define RTYPE_Material_Chunk				0xF5F8516F

#define RTYPE_MaterialTable					0x80D1F139
#define RTYPE_MaterialTable_Chunk			0xEB9FE716

#define RTYPE_iTexture						0x8B43FABF
#define RTYPE_iTexture_Chunk				0xCDBFA090

#define RTYPE_VertexDecl					0x3E5FDA3E
#define RTYPE_VertexDecl_Chunk				0xF7FC6B2D

#endif