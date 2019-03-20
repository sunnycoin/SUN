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


#include <sunnycoin/nodestore/backend/MemoryFactory.cpp>
#include <sunnycoin/nodestore/backend/NuDBFactory.cpp>
#include <sunnycoin/nodestore/backend/NullFactory.cpp>
#include <sunnycoin/nodestore/backend/RocksDBFactory.cpp>
#include <sunnycoin/nodestore/backend/RocksDBQuickFactory.cpp>

#include <sunnycoin/nodestore/impl/BatchWriter.cpp>
#include <sunnycoin/nodestore/impl/Database.cpp>
#include <sunnycoin/nodestore/impl/DatabaseNodeImp.cpp>
#include <sunnycoin/nodestore/impl/DatabaseRotatingImp.cpp>
#include <sunnycoin/nodestore/impl/DatabaseShardImp.cpp>
#include <sunnycoin/nodestore/impl/DummyScheduler.cpp>
#include <sunnycoin/nodestore/impl/DecodedBlob.cpp>
#include <sunnycoin/nodestore/impl/EncodedBlob.cpp>
#include <sunnycoin/nodestore/impl/ManagerImp.cpp>
#include <sunnycoin/nodestore/impl/NodeObject.cpp>
#include <sunnycoin/nodestore/impl/Shard.cpp>
