#include "order_book.h"

#include <algorithm>
#include <iterator>
#include <stdexcept>

namespace engine {

std::vector<Trade> OrderBook::add_limit_order(
    OrderId order_id,
    Side side,
    Price price,
    Quantity quantity
) {
    if (price <= 0) {
        throw std::invalid_argument("price must be positive");
    }

    if (quantity == 0) {
        throw std::invalid_argument("quantity must be positive");
    }

    if (order_lookup_.contains(order_id)) {
        throw std::invalid_argument("duplicate active order id");
    }

    Order incoming_order{
        .id = order_id,
        .side = side,
        .price = price,
        .remaining_quantity = quantity,
        .sequence_number = next_sequence_number_++
    };

    std::vector<Trade> trades;

    if (side == Side::Buy) {
        match_buy_order(incoming_order, trades);
    } else {
        match_sell_order(incoming_order, trades);
    }

    if (incoming_order.remaining_quantity > 0) {
        add_resting_order(incoming_order);
    }

    return trades;
}

void OrderBook::match_buy_order(Order& incoming_order, std::vector<Trade>& trades) {
    while (incoming_order.remaining_quantity > 0 && !asks_.empty()) {
        auto best_ask_it = asks_.begin();
        Price best_ask_price = best_ask_it->first;

        if (incoming_order.price < best_ask_price) {
            break;
        }

        auto& resting_orders = best_ask_it->second;

        while (incoming_order.remaining_quantity > 0 && !resting_orders.empty()) {
            Order& resting_order = resting_orders.front();

            Quantity trade_quantity = std::min(
                incoming_order.remaining_quantity,
                resting_order.remaining_quantity
            );

            trades.push_back(Trade{
                .buy_order_id = incoming_order.id,
                .sell_order_id = resting_order.id,
                .price = resting_order.price,
                .quantity = trade_quantity
            });

            incoming_order.remaining_quantity -= trade_quantity;
            resting_order.remaining_quantity -= trade_quantity;

            if (resting_order.remaining_quantity == 0) {
                order_lookup_.erase(resting_order.id);
                resting_orders.pop_front();
            }
        }

        if (resting_orders.empty()) {
            asks_.erase(best_ask_it);
        }
    }
}

void OrderBook::match_sell_order(Order& incoming_order, std::vector<Trade>& trades) {
    while (incoming_order.remaining_quantity > 0 && !bids_.empty()) {
        auto best_bid_it = bids_.begin();
        Price best_bid_price = best_bid_it->first;

        if (incoming_order.price > best_bid_price) {
            break;
        }

        auto& resting_orders = best_bid_it->second;

        while (incoming_order.remaining_quantity > 0 && !resting_orders.empty()) {
            Order& resting_order = resting_orders.front();

            Quantity trade_quantity = std::min(
                incoming_order.remaining_quantity,
                resting_order.remaining_quantity
            );

            trades.push_back(Trade{
                .buy_order_id = resting_order.id,
                .sell_order_id = incoming_order.id,
                .price = resting_order.price,
                .quantity = trade_quantity
            });

            incoming_order.remaining_quantity -= trade_quantity;
            resting_order.remaining_quantity -= trade_quantity;

            if (resting_order.remaining_quantity == 0) {
                order_lookup_.erase(resting_order.id);
                resting_orders.pop_front();
            }
        }

        if (resting_orders.empty()) {
            bids_.erase(best_bid_it);
        }
    }
}

void OrderBook::add_resting_order(const Order& order) {
    if (order.side == Side::Buy) {
        auto& level = bids_[order.price];
        level.push_back(order);

        auto order_it = std::prev(level.end());

        order_lookup_[order.id] = OrderLocation{
            .side = order.side,
            .price = order.price,
            .iterator = order_it
        };
    } else {
        auto& level = asks_[order.price];
        level.push_back(order);

        auto order_it = std::prev(level.end());

        order_lookup_[order.id] = OrderLocation{
            .side = order.side,
            .price = order.price,
            .iterator = order_it
        };
    }
}

bool OrderBook::empty() const {
    return bids_.empty() && asks_.empty();
}

bool OrderBook::contains_order(OrderId order_id) const {
    return order_lookup_.contains(order_id);
}

} // namespace engine
