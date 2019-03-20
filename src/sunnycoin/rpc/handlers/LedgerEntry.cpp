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

#include <sunnycoin/app/main/Application.h>
#include <sunnycoin/basics/strHex.h>
#include <sunnycoin/basics/StringUtilities.h>
#include <sunnycoin/ledger/ReadView.h>
#include <sunnycoin/net/RPCErr.h>
#include <sunnycoin/protocol/ErrorCodes.h>
#include <sunnycoin/protocol/Indexes.h>
#include <sunnycoin/protocol/JsonFields.h>
#include <sunnycoin/rpc/Context.h>
#include <sunnycoin/rpc/impl/RPCHelpers.h>

namespace sunnycoin {

// {
//   ledger_hash : <ledger>
//   ledger_index : <ledger_index>
//   ...
// }
Json::Value doLedgerEntry (RPC::Context& context)
{
    std::shared_ptr<ReadView const> lpLedger;
    auto jvResult = RPC::lookupLedger (lpLedger, context);

    if (!lpLedger)
        return jvResult;

    uint256         uNodeIndex;
    bool            bNodeBinary = false;
    LedgerEntryType expectedType = ltANY;

    if (context.params.isMember (jss::index))
    {
        uNodeIndex.SetHex (context.params[jss::index].asString());
        bNodeBinary = true;
    }
    else if (context.params.isMember (jss::account_root))
    {
        expectedType = ltACCOUNT_ROOT;
        auto const account = parseBase58<AccountID>(
            context.params[jss::account_root].asString());
        if (! account || account->isZero())
            jvResult[jss::error] = "malformedAddress";
        else
            uNodeIndex = keylet::account(*account).key;
    }
    else if (context.params.isMember (jss::check))
    {
        expectedType = ltCHECK;
        uNodeIndex.SetHex (context.params[jss::check].asString());
    }
    else if (context.params.isMember (jss::deposit_preauth))
    {
        expectedType = ltDEPOSIT_PREAUTH;

        if (!context.params[jss::deposit_preauth].isObject())
        {
            if (! context.params[jss::deposit_preauth].isString() ||
                ! uNodeIndex.SetHex (
                    context.params[jss::deposit_preauth].asString()))
            {
                uNodeIndex = beast::zero;
                jvResult[jss::error] = "malformedRequest";
            }
        }
        else if (!context.params[jss::deposit_preauth].isMember (jss::owner)
            || !context.params[jss::deposit_preauth][jss::owner].isString()
            || !context.params[jss::deposit_preauth].isMember (jss::authorized)
            || !context.params[jss::deposit_preauth][jss::authorized].isString())
        {
            jvResult[jss::error] = "malformedRequest";
        }
        else
        {
            auto const owner = parseBase58<AccountID>(context.params[
                jss::deposit_preauth][jss::owner].asString());

            auto const authorized = parseBase58<AccountID>(context.params[
                jss::deposit_preauth][jss::authorized].asString());

            if (! owner)
                jvResult[jss::error] = "malformedOwner";
            else if (! authorized)
                jvResult[jss::error] = "malformedAuthorized";
            else
                uNodeIndex = keylet::depositPreauth (*owner, *authorized).key;
        }
    }
    else if (context.params.isMember (jss::directory))
    {
        expectedType = ltDIR_NODE;
        if (context.params[jss::directory].isNull())
        {
            jvResult[jss::error]   = "malformedRequest";
        }
        else if (!context.params[jss::directory].isObject())
        {
            uNodeIndex.SetHex (context.params[jss::directory].asString ());
        }
        else if (context.params[jss::directory].isMember (jss::sub_index)
            && !context.params[jss::directory][jss::sub_index].isIntegral ())
        {
            jvResult[jss::error] = "malformedRequest";
        }
        else
        {
            std::uint64_t  uSubIndex
                = context.params[jss::directory].isMember (jss::sub_index)
                ? context.params[jss::directory][jss::sub_index].asUInt () : 0;

            if (context.params[jss::directory].isMember (jss::dir_root))
            {
                if (context.params[jss::directory].isMember (jss::owner))
                {
                    // May not specify both dir_root and owner.
                    jvResult[jss::error] = "malformedRequest";
                }
                else
                {
                    uint256 uDirRoot;
                    uDirRoot.SetHex (
                        context.params[jss::directory][jss::dir_root].asString());

                    uNodeIndex = getDirNodeIndex (uDirRoot, uSubIndex);
                }
            }
            else if (context.params[jss::directory].isMember (jss::owner))
            {
                auto const ownerID = parseBase58<AccountID>(
                    context.params[jss::directory][jss::owner].asString());

                if (! ownerID)
                {
                    jvResult[jss::error] = "malformedAddress";
                }
                else
                {
                    uint256 uDirRoot = getOwnerDirIndex (*ownerID);
                    uNodeIndex = getDirNodeIndex (uDirRoot, uSubIndex);
                }
            }
            else
            {
                jvResult[jss::error] = "malformedRequest";
            }
        }
    }
    else if (context.params.isMember (jss::escrow))
    {
        expectedType = ltESCROW;
        if (!context.params[jss::escrow].isObject ())
        {
            uNodeIndex.SetHex (context.params[jss::escrow].asString ());
        }
        else if (!context.params[jss::escrow].isMember (jss::owner)
            || !context.params[jss::escrow].isMember (jss::seq)
            || !context.params[jss::escrow][jss::seq].isIntegral ())
        {
            jvResult[jss::error] = "malformedRequest";
        }
        else
        {
            auto const id = parseBase58<AccountID>(
                context.params[jss::escrow][jss::owner].asString());
            if (! id)
                jvResult[jss::error] = "malformedOwner";
            else
                uNodeIndex = keylet::escrow (*id,
                    context.params[jss::escrow][jss::seq].asUInt()).key;
        }
    }
    else if (context.params.isMember (jss::generator))
    {
        jvResult[jss::error] = "deprecatedFeature";
    }
    else if (context.params.isMember (jss::offer))
    {
        expectedType = ltOFFER;
        if (!context.params[jss::offer].isObject())
        {
            uNodeIndex.SetHex (context.params[jss::offer].asString ());
        }
        else if (!context.params[jss::offer].isMember (jss::account)
            || !context.params[jss::offer].isMember (jss::seq)
            || !context.params[jss::offer][jss::seq].isIntegral ())
        {
            jvResult[jss::error] = "malformedRequest";
        }
        else
        {
            auto const id = parseBase58<AccountID>(
                context.params[jss::offer][jss::account].asString());
            if (! id)
                jvResult[jss::error] = "malformedAddress";
            else
                uNodeIndex = getOfferIndex (*id,
                    context.params[jss::offer][jss::seq].asUInt ());
        }
    }
    else if (context.params.isMember (jss::payment_channel))
    {
        expectedType = ltPAYCHAN;
        uNodeIndex.SetHex (context.params[jss::payment_channel].asString ());
    }
    else if (context.params.isMember (jss::sunnycoin_state))
    {
        expectedType = ltSUNNYCOIN_STATE;
        Currency         uCurrency;
        Json::Value     jvSUNNYCoinState   = context.params[jss::sunnycoin_state];

        if (!jvSUNNYCoinState.isObject()
            || !jvSUNNYCoinState.isMember (jss::currency)
            || !jvSUNNYCoinState.isMember (jss::accounts)
            || !jvSUNNYCoinState[jss::accounts].isArray()
            || 2 != jvSUNNYCoinState[jss::accounts].size ()
            || !jvSUNNYCoinState[jss::accounts][0u].isString ()
            || !jvSUNNYCoinState[jss::accounts][1u].isString ()
            || (jvSUNNYCoinState[jss::accounts][0u].asString ()
                == jvSUNNYCoinState[jss::accounts][1u].asString ())
           )
        {
            jvResult[jss::error] = "malformedRequest";
        }
        else
        {
            auto const id1 = parseBase58<AccountID>(
                jvSUNNYCoinState[jss::accounts][0u].asString());
            auto const id2 = parseBase58<AccountID>(
                jvSUNNYCoinState[jss::accounts][1u].asString());
            if (! id1 || ! id2)
            {
                jvResult[jss::error] = "malformedAddress";
            }
            else if (!to_currency (uCurrency,
                jvSUNNYCoinState[jss::currency].asString()))
            {
                jvResult[jss::error] = "malformedCurrency";
            }
            else
            {
                uNodeIndex = getSUNNYCoinStateIndex(
                    *id1, *id2, uCurrency);
            }
        }
    }
    else
    {
        jvResult[jss::error] = "unknownOption";
    }

    if (uNodeIndex.isNonZero ())
    {
        auto const sleNode = lpLedger->read(keylet::unchecked(uNodeIndex));
        if (context.params.isMember(jss::binary))
            bNodeBinary = context.params[jss::binary].asBool();

        if (!sleNode)
        {
            // Not found.
            jvResult[jss::error] = "entryNotFound";
        }
        else if ((expectedType != ltANY) &&
            (expectedType != sleNode->getType()))
        {
            jvResult[jss::error] = "malformedRequest";
        }
        else if (bNodeBinary)
        {
            Serializer s;

            sleNode->add (s);

            jvResult[jss::node_binary] = strHex (s.peekData ());
            jvResult[jss::index]       = to_string (uNodeIndex);
        }
        else
        {
            jvResult[jss::node]        = sleNode->getJson (0);
            jvResult[jss::index]       = to_string (uNodeIndex);
        }
    }

    return jvResult;
}

} // sunnycoin
