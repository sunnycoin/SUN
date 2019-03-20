//------------------------------------------------------------------------------
/*
    This file is part of sunnycoind: https://github.com/sunnycoin/sunnycoind
    Copyright (c) 2012, 2013 SUNNYCoin Labs Inc.

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


// This has to be included early to prevent an obscure MSVC compile error
#include <boost/asio/deadline_timer.hpp>

#include <sunnycoin/protocol/JsonFields.h>
#include <sunnycoin/rpc/RPCHandler.h>
#include <sunnycoin/rpc/handlers/Handlers.h>

#include <sunnycoin/rpc/handlers/PathFind.cpp>
#include <sunnycoin/rpc/handlers/PayChanClaim.cpp>
#include <sunnycoin/rpc/handlers/Peers.cpp>
#include <sunnycoin/rpc/handlers/Ping.cpp>
#include <sunnycoin/rpc/handlers/Print.cpp>
#include <sunnycoin/rpc/handlers/Random.cpp>
#include <sunnycoin/rpc/handlers/SUNNYCoinPathFind.cpp>
#include <sunnycoin/rpc/handlers/ServerInfo.cpp>
#include <sunnycoin/rpc/handlers/ServerState.cpp>
#include <sunnycoin/rpc/handlers/SignFor.cpp>
#include <sunnycoin/rpc/handlers/SignHandler.cpp>
#include <sunnycoin/rpc/handlers/Stop.cpp>
#include <sunnycoin/rpc/handlers/Submit.cpp>
#include <sunnycoin/rpc/handlers/SubmitMultiSigned.cpp>
#include <sunnycoin/rpc/handlers/Subscribe.cpp>
#include <sunnycoin/rpc/handlers/TransactionEntry.cpp>
#include <sunnycoin/rpc/handlers/Tx.cpp>
#include <sunnycoin/rpc/handlers/TxHistory.cpp>
#include <sunnycoin/rpc/handlers/UnlList.cpp>
#include <sunnycoin/rpc/handlers/Unsubscribe.cpp>
#include <sunnycoin/rpc/handlers/ValidationCreate.cpp>
#include <sunnycoin/rpc/handlers/ValidationSeed.cpp>
#include <sunnycoin/rpc/handlers/Validators.cpp>
#include <sunnycoin/rpc/handlers/ValidatorListSites.cpp>
#include <sunnycoin/rpc/handlers/WalletPropose.cpp>

#include <sunnycoin/rpc/impl/Handler.cpp>
#include <sunnycoin/rpc/impl/LegacyPathFind.cpp>
#include <sunnycoin/rpc/impl/Role.cpp>
#include <sunnycoin/rpc/impl/RPCHandler.cpp>
#include <sunnycoin/rpc/impl/RPCHelpers.cpp>
#include <sunnycoin/rpc/impl/ServerHandlerImp.cpp>
#include <sunnycoin/rpc/impl/ShardArchiveHandler.cpp>
#include <sunnycoin/rpc/impl/Status.cpp>
#include <sunnycoin/rpc/impl/TransactionSign.cpp>


