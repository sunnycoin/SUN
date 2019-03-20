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

#include <sunnycoin/json/json_value.h>
#include <sunnycoin/protocol/JsonFields.h>
#include <sunnycoin/rpc/Role.h>
#include <sunnycoin/rpc/Context.h>

namespace sunnycoin {

namespace RPC {
struct Context;
} // RPC

Json::Value doPing (RPC::Context& context)
{
    // For testing connection privileges.
    if (isUnlimited(context.role))
    {
        Json::Value ret;

        switch (context.role)
        {
            case Role::ADMIN:
                ret[jss::role] = "admin";
                break;
            case Role::IDENTIFIED:
                ret[jss::role] = "identified";
                break;
            default:
                ;
        }

        return ret;
    }
    else
    {
        return Json::Value (Json::objectValue);
    }
}

} // sunnycoin
