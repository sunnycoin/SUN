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

#ifndef SUNNYCOIN_APP_PATHS_PATHFINDER_H_INCLUDED
#define SUNNYCOIN_APP_PATHS_PATHFINDER_H_INCLUDED

#include <sunnycoin/app/ledger/Ledger.h>
#include <sunnycoin/app/paths/SUNNYCoinLineCache.h>
#include <sunnycoin/core/LoadEvent.h>
#include <sunnycoin/protocol/STAmount.h>
#include <sunnycoin/protocol/STPathSet.h>

namespace sunnycoin {

/** Calculates payment paths.

    The @ref SUNNYCoinCalc determines the quality of the found paths.

    @see SUNNYCoinCalc
*/
class Pathfinder
{
public:
    /** Construct a pathfinder without an issuer.*/
    Pathfinder (
        std::shared_ptr<SUNNYCoinLineCache> const& cache,
        AccountID const& srcAccount,
        AccountID const& dstAccount,
        Currency const& uSrcCurrency,
        boost::optional<AccountID> const& uSrcIssuer,
        STAmount const& dstAmount,
        boost::optional<STAmount> const& srcAmount,
        Application& app);
    Pathfinder (Pathfinder const&) = delete;
    Pathfinder& operator= (Pathfinder const&) = delete;
    ~Pathfinder() = default;

    static void initPathTable ();

    bool findPaths (int searchLevel);

    /** Compute the rankings of the paths. */
    void computePathRanks (int maxPaths);

    /* Get the best paths, up to maxPaths in number, from mCompletePaths.

       On return, if fullLiquidityPath is not empty, then it contains the best
       additional single path which can consume all the liquidity.
    */
    STPathSet
    getBestPaths (
        int maxPaths,
        STPath& fullLiquidityPath,
        STPathSet const& extraPaths,
        AccountID const& srcIssuer);

    enum NodeType
    {
        nt_SOURCE,     // The source account: with an issuer account, if needed.
        nt_ACCOUNTS,   // Accounts that connect from this source/currency.
        nt_BOOKS,      // Order books that connect to this currency.
        nt_SUN_BOOK,   // The order book from this currency to SUN.
        nt_DEST_BOOK,  // The order book to the destination currency/issuer.
        nt_DESTINATION // The destination account only.
    };

    // The PathType is a list of the NodeTypes for a path.
    using PathType = std::vector <NodeType>;

    // PaymentType represents the types of the source and destination currencies
    // in a path request.
    enum PaymentType
    {
        pt_SUN_to_SUN,
        pt_SUN_to_nonSUN,
        pt_nonSUN_to_SUN,
        pt_nonSUN_to_same,   // Destination currency is the same as source.
        pt_nonSUN_to_nonSUN  // Destination currency is NOT the same as source.
    };

    struct PathRank
    {
        std::uint64_t quality;
        std::uint64_t length;
        STAmount liquidity;
        int index;
    };

private:
    /*
      Call graph of Pathfinder methods.

      findPaths:
          addPathsForType:
              addLinks:
                  addLink:
                      getPathsOut
                      issueMatchesOrigin
                      isNoSUNNYCoinOut:
                          isNoSUNNYCoin

      computePathRanks:
          sunnycoinCalculate
          getPathLiquidity:
              sunnycoinCalculate

      getBestPaths
     */


    // Add all paths of one type to mCompletePaths.
    STPathSet& addPathsForType (PathType const& type);

    bool issueMatchesOrigin (Issue const&);

    int getPathsOut (
        Currency const& currency,
        AccountID const& account,
        bool isDestCurrency,
        AccountID const& dest);

    void addLink (
        STPath const& currentPath,
        STPathSet& incompletePaths,
        int addFlags);

    // Call addLink() for each path in currentPaths.
    void addLinks (
        STPathSet const& currentPaths,
        STPathSet& incompletePaths,
        int addFlags);

    // Compute the liquidity for a path.  Return tesSUCCESS if it has has enough
    // liquidity to be worth keeping, otherwise an error.
    TER getPathLiquidity (
        STPath const& path,            // IN:  The path to check.
        STAmount const& minDstAmount,  // IN:  The minimum output this path must
                                       //      deliver to be worth keeping.
        STAmount& amountOut,           // OUT: The actual liquidity on the path.
        uint64_t& qualityOut) const;   // OUT: The returned initial quality

    // Does this path end on an account-to-account link whose last account has
    // set the "no sunnycoin" flag on the link?
    bool isNoSUNNYCoinOut (STPath const& currentPath);

    // Is the "no sunnycoin" flag set from one account to another?
    bool isNoSUNNYCoin (
        AccountID const& fromAccount,
        AccountID const& toAccount,
        Currency const& currency);

    void rankPaths (
        int maxPaths,
        STPathSet const& paths,
        std::vector <PathRank>& rankedPaths);

    AccountID mSrcAccount;
    AccountID mDstAccount;
    AccountID mEffectiveDst; // The account the paths need to end at
    STAmount mDstAmount;
    Currency mSrcCurrency;
    boost::optional<AccountID> mSrcIssuer;
    STAmount mSrcAmount;
    /** The amount remaining from mSrcAccount after the default liquidity has
        been removed. */
    STAmount mRemainingAmount;
    bool convert_all_;

    std::shared_ptr <ReadView const> mLedger;
    std::unique_ptr<LoadEvent> m_loadEvent;
    std::shared_ptr<SUNNYCoinLineCache> mRLCache;

    STPathElement mSource;
    STPathSet mCompletePaths;
    std::vector<PathRank> mPathRanks;
    std::map<PathType, STPathSet> mPaths;

    hash_map<Issue, int> mPathsOutCountMap;

    Application& app_;
    beast::Journal j_;

    // Add sunnycoin paths
    static std::uint32_t const afADD_ACCOUNTS = 0x001;

    // Add order books
    static std::uint32_t const afADD_BOOKS = 0x002;

    // Add order book to SUN only
    static std::uint32_t const afOB_SUN = 0x010;

    // Must link to destination currency
    static std::uint32_t const afOB_LAST = 0x040;

    // Destination account only
    static std::uint32_t const afAC_LAST = 0x080;
};

} // sunnycoin

#endif
