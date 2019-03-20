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

#ifndef SUNNYCOIN_APP_PATHS_PATHREQUESTS_H_INCLUDED
#define SUNNYCOIN_APP_PATHS_PATHREQUESTS_H_INCLUDED

#include <sunnycoin/app/main/Application.h>
#include <sunnycoin/app/paths/PathRequest.h>
#include <sunnycoin/app/paths/SUNNYCoinLineCache.h>
#include <sunnycoin/core/Job.h>
#include <atomic>
#include <mutex>
#include <vector>

namespace sunnycoin {

class PathRequests
{
public:
    /** A collection of all PathRequest instances. */
    PathRequests (Application& app,
            beast::Journal journal, beast::insight::Collector::ptr const& collector)
        : app_ (app)
        , mJournal (journal)
        , mLastIdentifier (0)
    {
        mFast = collector->make_event ("pathfind_fast");
        mFull = collector->make_event ("pathfind_full");
    }

    /** Update all of the contained PathRequest instances.

        @param ledger Ledger we are pathfinding in.
        @param shouldCancel Invocable that returns whether to cancel.
     */
    void updateAll (std::shared_ptr<ReadView const> const& ledger,
                    Job::CancelCallback shouldCancel);

    std::shared_ptr<SUNNYCoinLineCache> getLineCache (
        std::shared_ptr <ReadView const> const& ledger, bool authoritative);

    // Create a new-style path request that pushes
    // updates to a subscriber
    Json::Value makePathRequest (
        std::shared_ptr <InfoSub> const& subscriber,
        std::shared_ptr<ReadView const> const& ledger,
        Json::Value const& request);

    // Create an old-style path request that is
    // managed by a coroutine and updated by
    // the path engine
    Json::Value makeLegacyPathRequest (
        PathRequest::pointer& req,
        std::function <void (void)> completion,
        Resource::Consumer& consumer,
        std::shared_ptr<ReadView const> const& inLedger,
        Json::Value const& request);

    // Execute an old-style path request immediately
    // with the ledger specified by the caller
    Json::Value doLegacyPathRequest (
        Resource::Consumer& consumer,
        std::shared_ptr<ReadView const> const& inLedger,
        Json::Value const& request);

    void reportFast (std::chrono::milliseconds ms)
    {
        mFast.notify (ms);
    }

    void reportFull (std::chrono::milliseconds ms)
    {
        mFull.notify (ms);
    }

private:
    void insertPathRequest (PathRequest::pointer const&);

    Application& app_;
    beast::Journal                   mJournal;

    beast::insight::Event            mFast;
    beast::insight::Event            mFull;

    // Track all requests
    std::vector<PathRequest::wptr> requests_;

    // Use a SUNNYCoinLineCache
    std::shared_ptr<SUNNYCoinLineCache>         mLineCache;

    std::atomic<int>                 mLastIdentifier;

    using ScopedLockType = std::lock_guard <std::recursive_mutex>;
    std::recursive_mutex mLock;

};

} // sunnycoin

#endif
