#pragma once

#include "types.h"

#include <deque>
#include <functional>
#include <map>
#include <vector>

namespace engine {

class OrderBook {
public:
    std::vector<Trade> add_limit_order(
        OrderId order_id,
        Side side,
        Price price,
        Quantity quantity);

    bool empty() const;

private:
    using BidBook = std::map<Price, std::deque<Order>, std::greater<Price>>;
    using AskBook = std::map<Price, std::deque<Order>, std::less<Price>>;

    BidBook bids_;
    AskBook asks_;
    SequenceNumber next_sequence_number_{1};
};

}  // namespace engine
