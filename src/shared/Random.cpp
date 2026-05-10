/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "Random.h"
#include "Errors.h"

#include "SFMT.h"

#include <cmath>
#include <ctime>

namespace
{
    struct SFMTRand
    {
        sfmt_t sfmt;
        SFMTRand()
        {
            sfmt_init_gen_rand(&sfmt,
                static_cast<uint32_t>(time(nullptr)) ^
                static_cast<uint32_t>(clock()) ^
                static_cast<uint32_t>(reinterpret_cast<uintptr_t>(this)));
        }
    };
}

thread_local SFMTRand sfmtRand;

// Generate a uniform random integer in [0, n] without modulo bias.
static uint32_t sfmt_randInt_n(uint32_t n)
{
    uint32_t used = n;
    used |= used >> 1;
    used |= used >> 2;
    used |= used >> 4;
    used |= used >> 8;
    used |= used >> 16;
    uint32_t i;
    do
        i = sfmt_genrand_uint32(&sfmtRand.sfmt) & used;
    while (i > n);
    return i;
}

int32 irand(int32 minInclusive, int32 maxInclusive)
{
    return int32(sfmt_randInt_n(maxInclusive - minInclusive)) + minInclusive;
}

uint32 urand(uint32 minInclusive, uint32 maxInclusive)
{
    return sfmt_randInt_n(maxInclusive - minInclusive) + minInclusive;
}

float frand(float minInclusive, float maxExclusive)
{
    return static_cast<float>(sfmt_genrand_real2(&sfmtRand.sfmt)) * (maxExclusive - minInclusive) + minInclusive;
}

int32 rand32()
{
    return static_cast<int32>(sfmt_genrand_uint32(&sfmtRand.sfmt));
}

double rand_norm()
{
    return sfmt_genrand_real2(&sfmtRand.sfmt);
}

float rand_norm_f()
{
    return static_cast<float>(sfmt_genrand_real2(&sfmtRand.sfmt));
}

double rand_chance()
{
    return sfmt_genrand_real2(&sfmtRand.sfmt) * 100.0;
}

float rand_chance_f()
{
    return static_cast<float>(sfmt_genrand_real2(&sfmtRand.sfmt) * 100.0);
}

Milliseconds randtime(Milliseconds const& minInclusive, Milliseconds const& maxInclusive)
{
    long long diff = maxInclusive.count() - minInclusive.count();
    MANGOS_ASSERT(diff >= 0);
    MANGOS_ASSERT(diff <= (uint32)-1);
    return minInclusive + Milliseconds(urand(0, diff));
}

int32 rand_dither(float v)
{
    return std::copysign(std::floor(std::abs(v) + frand(0, 1)), v);
}

uint32 rand_ditheru(float v)
{
    return static_cast<uint32>(rand_dither(std::max(v, 0.f)));
}
