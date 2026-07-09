#pragma once

#include "types.h"

#include <functional>
#include <list>
#include <map>
#include <unordered_map>
#include <vector>

namespace engine {

class OrderBook {
public:
    std::vector<Trade> add_limit_order(
        OrderId order_id,
        Side side,
        Price price,
        Quantity quantity
    );

    bool empty() const;
    bool contains_order(OrderId order_id) const;

private:
    using OrderQueue = std::list<Order>;

    using BidBook = std::map<Price, OrderQueue, std::greater<Price>>;
    using AskBook = std::map<Price, OrderQueue, std::less<Price>>;

    struct OrderLocation {
        Side side{};
        Price price{};
        OrderQueue::iterator iterator{};
    };

    BidBook bids_;
    AskBook asks_;

    std::unordered_map<OrderId, OrderLocation> order_lookup_;

    SequenceNumber next_sequence_number_{1};

private:
    void match_buy_order(Order& incoming_order, std::vector<Trade>& trades);
    void match_sell_order(Order& incoming_order, std::vector<Trade>& trades);
    void add_resting_order(const Order& order);
};

} // namespace engine
