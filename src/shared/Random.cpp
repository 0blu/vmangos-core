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
#include "SFMTRand.h"

#include <cmath>
#include <random>

static thread_local SFMTRand sfmtRand;
static RandomEngine engine;

int32 irand(int32 minInclusive, int32 maxInclusive)
{
    MANGOS_ASSERT(maxInclusive >= minInclusive);
    std::uniform_int_distribution<int32> uid(minInclusive, maxInclusive);
    return uid(engine);
}

uint32 urand(uint32 minInclusive, uint32 maxInclusive)
{
    MANGOS_ASSERT(maxInclusive >= minInclusive);
    std::uniform_int_distribution<uint32> uid(minInclusive, maxInclusive);
    return uid(engine);
}

float frand(float minInclusive, float maxExclusive)
{
    MANGOS_ASSERT(maxExclusive >= minInclusive);
    std::uniform_real_distribution<float> urd(minInclusive, maxExclusive);
    return urd(engine);
}

uint32 rand32()
{
    return sfmtRand.RandomUInt32();
}

double rand_norm()
{
    std::uniform_real_distribution<double> urd;
    return urd(engine);
}

float rand_norm_f()
{
    std::uniform_real_distribution<float> urd;
    return urd(engine);
}

double rand_chance()
{
    std::uniform_real_distribution<double> urd(0.0, 100.0);
    return urd(engine);
}

float rand_chance_f()
{
    std::uniform_real_distribution<float> urd(0.0f, 100.0f);
    return urd(engine);
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

RandomEngine& RandomEngine::Instance()
{
    return engine;
}
