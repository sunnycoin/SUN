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

#include <sunnycoin/app/tx/impl/OfferStream.h>
#include <sunnycoin/beast/unit_test.h>

namespace sunnycoin {

class OfferStream_test : public beast::unit_test::suite
{
public:
    void
    test()
    {
        pass();
    }

    void
    run() override
    {
        test();
    }
};

BEAST_DEFINE_TESTSUITE(OfferStream,tx,sunnycoin);

}