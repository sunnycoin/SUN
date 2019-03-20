//------------------------------------------------------------------------------
/*
    This file is part of sunnycoind: https://github.com/sunnycoin/sunnycoind
    Copyright (c) 2012-2014 SUNNYCoin Labs Inc.

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#include <sunnycoin/crypto/csprng.h>
#include <sunnycoin/json/json_value.h>
#include <sunnycoin/net/RPCErr.h>
#include <sunnycoin/protocol/ErrorCodes.h>
#include <sunnycoin/protocol/JsonFields.h>
#include <sunnycoin/basics/base_uint.h>
#include <sunnycoin/beast/utility/rngfill.h>

namespace sunnycoin {

namespace RPC {
struct Context;
}

// Result:
// {
//   random: <uint256>
// }
Json::Value doRandom (RPC::Context& context)
{
    // TODO(tom): the try/catch is almost certainly redundant, we catch at the
    // top level too.
    try
    {
        uint256 rand;
        beast::rngfill (
            rand.begin(),
            rand.size(),
            crypto_prng());

        Json::Value jvResult;
        jvResult[jss::random]  = to_string (rand);
        return jvResult;
    }
    catch (std::exception const&)
    {
        return rpcError (rpcINTERNAL);
    }
}

} // sunnycoin
