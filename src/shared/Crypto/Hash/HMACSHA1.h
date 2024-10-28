/*
 * Copyright (C) 2005-2012 MaNGOS <http://getmangos.com/>
 *
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

#ifndef _AUTH_HMACSHA1_H
#define _AUTH_HMACSHA1_H

#include "Common.h"

#include <openssl/opensslv.h>

class BigNumber;

class HMACSHA1
{
    public:
        HMACSHA1(const uint8* seed, size_t len);
        ~HMACSHA1();

        void UpdateBigNumber(BigNumber* bn);
        void UpdateData(std::vector<uint8> const& data);
        void UpdateData(std::string const& str);
        void UpdateData(uint8 const* data, size_t length);

        void Finalize();

        uint8* GetDigest() { return m_digest; }
        int constexpr GetLength() { return sizeof(m_digest); }

    private:
#if defined(OPENSSL_VERSION_MAJOR) && (OPENSSL_VERSION_MAJOR >= 3)
        typedef struct evp_mac_st EVP_MAC;
        typedef struct evp_mac_ctx_st EVP_MAC_CTX;
        EVP_MAC* m_mac;
        EVP_MAC_CTX* m_ctx;
#else
        typedef struct hmac_ctx_st HMAC_CTX;
        HMAC_CTX* m_ctx;
#endif
        uint8 m_digest[20]; // SHA_DIGEST_LENGTH
};
#endif
