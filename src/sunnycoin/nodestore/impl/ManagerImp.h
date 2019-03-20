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

#ifndef SUNNYCOIN_NODESTORE_MANAGERIMP_H_INCLUDED
#define SUNNYCOIN_NODESTORE_MANAGERIMP_H_INCLUDED

#include <sunnycoin/nodestore/Manager.h>

namespace sunnycoin {
namespace NodeStore {

class ManagerImp : public Manager
{
private:
    std::mutex mutex_;
    std::vector<Factory*> list_;

public:
    static
    ManagerImp&
    instance();

    static
    void
    missing_backend();

    ManagerImp() = default;

    ~ManagerImp() = default;

    Factory*
    find (std::string const& name) override;

    void
    insert (Factory& factory) override;

    void
    erase (Factory& factory) override;

    std::unique_ptr <Backend>
    make_Backend (
        Section const& parameters,
        Scheduler& scheduler,
        beast::Journal journal) override;

    std::unique_ptr <Database>
    make_Database (
        std::string const& name,
        Scheduler& scheduler,
        int readThreads,
        Stoppable& parent,
        Section const& config,
        beast::Journal journal) override;
};

}
}

#endif