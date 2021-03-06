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

#ifndef SUNNYCOIN_RPC_SERVERHANDLER_H_INCLUDED
#define SUNNYCOIN_RPC_SERVERHANDLER_H_INCLUDED

#include <sunnycoin/basics/BasicConfig.h>
#include <sunnycoin/core/Config.h>
#include <sunnycoin/core/JobQueue.h>
#include <sunnycoin/core/Stoppable.h>
#include <sunnycoin/server/Port.h>
#include <sunnycoin/resource/ResourceManager.h>
#include <sunnycoin/rpc/impl/ServerHandlerImp.h>
#include <sunnycoin/beast/utility/Journal.h>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/address.hpp>
#include <memory>
#include <vector>

namespace sunnycoin {

using ServerHandler = ServerHandlerImp;

ServerHandler::Setup
setup_ServerHandler (
    Config const& c,
    std::ostream&& log);

std::unique_ptr <ServerHandler>
make_ServerHandler (Application& app, Stoppable& parent, boost::asio::io_service&,
    JobQueue&, NetworkOPs&, Resource::Manager&, CollectorManager& cm);

} // sunnycoin

#endif
