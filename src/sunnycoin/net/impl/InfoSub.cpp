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

#include <sunnycoin/net/InfoSub.h>
#include <atomic>

namespace sunnycoin {

// This is the primary interface into the "client" portion of the program.
// Code that wants to do normal operations on the network such as
// creating and monitoring accounts, creating transactions, and so on
// should use this interface. The RPC code will primarily be a light wrapper
// over this code.

// Eventually, it will check the node's operating mode (synched, unsynched,
// etectera) and defer to the correct means of processing. The current
// code assumes this node is synched (and will continue to do so until
// there's a functional network.

//------------------------------------------------------------------------------

InfoSub::Source::Source (char const* name, Stoppable& parent)
    : Stoppable (name, parent)
{
}

//------------------------------------------------------------------------------

InfoSub::InfoSub(Source& source)
    : m_source(source)
    , mSeq(assign_id())
{
}

InfoSub::InfoSub(Source& source, Consumer consumer)
    : m_consumer(consumer)
    , m_source(source)
    , mSeq(assign_id())
{
}

InfoSub::~InfoSub ()
{
    m_source.unsubTransactions (mSeq);
    m_source.unsubRTTransactions (mSeq);
    m_source.unsubLedger (mSeq);
    m_source.unsubManifests (mSeq);
    m_source.unsubServer (mSeq);
    m_source.unsubValidations (mSeq);
    m_source.unsubPeerStatus (mSeq);

    // Use the internal unsubscribe so that it won't call
    // back to us and modify its own parameter
    if (! realTimeSubscriptions_.empty ())
        m_source.unsubAccountInternal
            (mSeq, realTimeSubscriptions_, true);

    if (! normalSubscriptions_.empty ())
        m_source.unsubAccountInternal
            (mSeq, normalSubscriptions_, false);
}

Resource::Consumer& InfoSub::getConsumer()
{
    return m_consumer;
}

std::uint64_t InfoSub::getSeq ()
{
    return mSeq;
}

void InfoSub::onSendEmpty ()
{
}

void InfoSub::insertSubAccountInfo (AccountID const& account, bool rt)
{
    ScopedLockType sl (mLock);

    if (rt)
        realTimeSubscriptions_.insert (account);
    else
        normalSubscriptions_.insert (account);
}

void InfoSub::deleteSubAccountInfo (AccountID const& account, bool rt)
{
    ScopedLockType sl (mLock);

    if (rt)
        realTimeSubscriptions_.erase (account);
    else
        normalSubscriptions_.erase (account);
}

void InfoSub::clearPathRequest ()
{
    mPathRequest.reset ();
}

void InfoSub::setPathRequest (const std::shared_ptr<PathRequest>& req)
{
    mPathRequest = req;
}

const std::shared_ptr<PathRequest>& InfoSub::getPathRequest ()
{
    return mPathRequest;
}

} // sunnycoin
