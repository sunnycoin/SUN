//------------------------------------------------------------------------------
/*
    This file is part of sunnycoind: https://github.com/sunnycoin/sunnycoind
    Copyright (c) 2016 SUNNYCoin Labs Inc.

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

#ifndef SUNNYCOIN_NET_REGISTER_SSL_CERTS_H_INCLUDED
#define SUNNYCOIN_NET_REGISTER_SSL_CERTS_H_INCLUDED

#include <boost/asio/ssl/context.hpp>
#include <sunnycoin/basics/Log.h>

namespace sunnycoin {
/** Register default SSL certificates.

    Register the system default SSL root certificates. On linux/mac,
    this just calls asio's `set_default_verify_paths` to look in standard
    operating system locations. On windows, it uses the OS certificate
    store accessible via CryptoAPI.
*/
void
registerSSLCerts(
    boost::asio::ssl::context&,
    boost::system::error_code&,
    beast::Journal j);

}  // namespace sunnycoin

#endif
