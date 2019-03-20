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

#include <sunnycoin/app/paths/Credit.h>
#include <sunnycoin/app/paths/Flow.h>
#include <sunnycoin/app/paths/impl/AmountSpec.h>
#include <sunnycoin/app/paths/impl/StrandFlow.h>
#include <sunnycoin/app/paths/impl/Steps.h>
#include <sunnycoin/basics/Log.h>
#include <sunnycoin/protocol/IOUAmount.h>
#include <sunnycoin/protocol/SUNAmount.h>

#include <boost/container/flat_set.hpp>

#include <numeric>
#include <sstream>

namespace sunnycoin {

template<class FlowResult>
static
auto finishFlow (PaymentSandbox& sb,
    Issue const& srcIssue, Issue const& dstIssue,
    FlowResult&& f)
{
    path::SUNNYCoinCalc::Output result;
    if (f.ter == tesSUCCESS)
        f.sandbox->apply (sb);
    else
        result.removableOffers = std::move (f.removableOffers);

    result.setResult (f.ter);
    result.actualAmountIn = toSTAmount (f.in, srcIssue);
    result.actualAmountOut = toSTAmount (f.out, dstIssue);

    return result;
};

path::SUNNYCoinCalc::Output
flow (
    PaymentSandbox& sb,
    STAmount const& deliver,
    AccountID const& src,
    AccountID const& dst,
    STPathSet const& paths,
    bool defaultPaths,
    bool partialPayment,
    bool ownerPaysTransferFee,
    bool offerCrossing,
    boost::optional<Quality> const& limitQuality,
    boost::optional<STAmount> const& sendMax,
    beast::Journal j,
    path::detail::FlowDebugInfo* flowDebugInfo)
{
    Issue const srcIssue = [&] {
        if (sendMax)
            return sendMax->issue ();
        if (!isSUN (deliver.issue ().currency))
            return Issue (deliver.issue ().currency, src);
        return sunIssue ();
    }();

    Issue const dstIssue = deliver.issue ();

    boost::optional<Issue> sendMaxIssue;
    if (sendMax)
        sendMaxIssue = sendMax->issue ();

    // convert the paths to a collection of strands. Each strand is the collection
    // of account->account steps and book steps that may be used in this payment.
    auto sr = toStrands (sb, src, dst, dstIssue, limitQuality, sendMaxIssue,
        paths, defaultPaths, ownerPaysTransferFee, offerCrossing, j);

    if (sr.first != tesSUCCESS)
    {
        path::SUNNYCoinCalc::Output result;
        result.setResult (sr.first);
        return result;
    }

    auto& strands = sr.second;

    if (j.trace())
    {
        j.trace() << "\nsrc: " << src << "\ndst: " << dst
            << "\nsrcIssue: " << srcIssue << "\ndstIssue: " << dstIssue;
        j.trace() << "\nNumStrands: " << strands.size ();
        for (auto const& curStrand : strands)
        {
            j.trace() << "NumSteps: " << curStrand.size ();
            for (auto const& step : curStrand)
            {
                j.trace() << '\n' << *step << '\n';
            }
        }
    }

    const bool srcIsSUN = isSUN (srcIssue.currency);
    const bool dstIsSUN = isSUN (dstIssue.currency);

    auto const asDeliver = toAmountSpec (deliver);

    // The src account may send either sun or iou. The dst account may receive
    // either sun or iou. Since SUN and IOU amounts are represented by different
    // types, use templates to tell `flow` about the amount types.
    if (srcIsSUN && dstIsSUN)
    {
        return finishFlow (sb, srcIssue, dstIssue,
            flow<SUNAmount, SUNAmount> (
                sb, strands, asDeliver.sun, partialPayment, offerCrossing,
                limitQuality, sendMax, j, flowDebugInfo));
    }

    if (srcIsSUN && !dstIsSUN)
    {
        return finishFlow (sb, srcIssue, dstIssue,
            flow<SUNAmount, IOUAmount> (
                sb, strands, asDeliver.iou, partialPayment, offerCrossing,
                limitQuality, sendMax, j, flowDebugInfo));
    }

    if (!srcIsSUN && dstIsSUN)
    {
        return finishFlow (sb, srcIssue, dstIssue,
            flow<IOUAmount, SUNAmount> (
                sb, strands, asDeliver.sun, partialPayment, offerCrossing,
                limitQuality, sendMax, j, flowDebugInfo));
    }

    assert (!srcIsSUN && !dstIsSUN);
    return finishFlow (sb, srcIssue, dstIssue,
        flow<IOUAmount, IOUAmount> (
            sb, strands, asDeliver.iou, partialPayment, offerCrossing,
            limitQuality, sendMax, j, flowDebugInfo));

}

} // sunnycoin
