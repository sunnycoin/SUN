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

#include <sunnycoin/rpc/handlers/AccountCurrenciesHandler.cpp>
#include <sunnycoin/rpc/handlers/AccountInfo.cpp>
#include <sunnycoin/rpc/handlers/AccountLines.cpp>
#include <sunnycoin/rpc/handlers/AccountChannels.cpp>
#include <sunnycoin/rpc/handlers/AccountObjects.cpp>
#include <sunnycoin/rpc/handlers/AccountOffers.cpp>
#include <sunnycoin/rpc/handlers/AccountTx.cpp>
#include <sunnycoin/rpc/handlers/AccountTxOld.cpp>
#include <sunnycoin/rpc/handlers/AccountTxSwitch.cpp>
#include <sunnycoin/rpc/handlers/BlackList.cpp>
#include <sunnycoin/rpc/handlers/BookOffers.cpp>
#include <sunnycoin/rpc/handlers/CanDelete.cpp>
#include <sunnycoin/rpc/handlers/Connect.cpp>
#include <sunnycoin/rpc/handlers/ConsensusInfo.cpp>
#include <sunnycoin/rpc/handlers/DepositAuthorized.cpp>
#include <sunnycoin/rpc/handlers/DownloadShard.cpp>
#include <sunnycoin/rpc/handlers/Feature1.cpp>
#include <sunnycoin/rpc/handlers/Fee1.cpp>
#include <sunnycoin/rpc/handlers/FetchInfo.cpp>
#include <sunnycoin/rpc/handlers/GatewayBalances.cpp>
#include <sunnycoin/rpc/handlers/GetCounts.cpp>
#include <sunnycoin/rpc/handlers/LedgerHandler.cpp>
#include <sunnycoin/rpc/handlers/LedgerAccept.cpp>
#include <sunnycoin/rpc/handlers/LedgerCleanerHandler.cpp>
#include <sunnycoin/rpc/handlers/LedgerClosed.cpp>
#include <sunnycoin/rpc/handlers/LedgerCurrent.cpp>
#include <sunnycoin/rpc/handlers/LedgerData.cpp>
#include <sunnycoin/rpc/handlers/LedgerEntry.cpp>
#include <sunnycoin/rpc/handlers/LedgerHeader.cpp>
#include <sunnycoin/rpc/handlers/LedgerRequest.cpp>
#include <sunnycoin/rpc/handlers/LogLevel.cpp>
#include <sunnycoin/rpc/handlers/LogRotate.cpp>
#include <sunnycoin/rpc/handlers/NoSUNNYCoinCheck.cpp>
#include <sunnycoin/rpc/handlers/OwnerInfo.cpp>
