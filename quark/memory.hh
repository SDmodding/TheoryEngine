#pragma once

namespace UFG
{
	template <typename T>
	THEORY_INLINE T qAlignUp(T n, T align) { return (n + (align - 1)) & ~(align - 1); }

    template <typename T>
    THEORY_INLINE T qAlignDown(T n, T align) { return n & ~(align - 1); }

    void qEndianSwapBytes(u8* v, u32 num_bytes);

#ifdef THEORY_IMPL

    void qEndianSwapBytes(u8* v, u32 num_bytes)
    {
        if (2 > num_bytes) {
            return;
        }

        u8* start = v;
        u8* end = v + num_bytes - 1;

        while (end > start)
        {
            u8 tmp = *end;
            *end = *start;
            *start = tmp;

            ++start;
            --end;
        }
    }

#endif
}