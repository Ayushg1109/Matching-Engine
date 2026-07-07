#include "order_book.h"

namespace engine {

std::vector<Trade> OrderBook::add_limit_order(
    OrderId order_id,
    Side side,
    Price price,
    Quantity quantity) {
    std::vector<Trade> trades;
    Order incoming{
        .id = order_id,
        .side = side,
        .price = price,
        .remaining_quantity = quantity,
        .sequence_number = next_sequence_number_++
    };

    if (side == Side::Buy) {
        while (incoming.remaining_quantity > 0 && !asks_.empty()) {
            auto best_ask = asks_.begin();
            if (price < best_ask->first) {
                break;
            }

            auto& resting = best_ask->second.front();
            const Quantity traded_quantity =
                incoming.remaining_quantity < resting.remaining_quantity
                    ? incoming.remaining_quantity
                    : resting.remaining_quantity;

            trades.push_back(Trade{
                .buy_order_id = incoming.id,
                .sell_order_id = resting.id,
                .price = resting.price,
                .quantity = traded_quantity
            });

            incoming.remaining_quantity -= traded_quantity;
            resting.remaining_quantity -= traded_quantity;

            if (resting.remaining_quantity == 0) {
                best_ask->second.pop_front();
                if (best_ask->second.empty()) {
                    asks_.erase(best_ask);
                }
            }
        }

        if (incoming.remaining_quantity > 0) {
            bids_[incoming.price].push_back(incoming);
        }
    } else {
        while (incoming.remaining_quantity > 0 && !bids_.empty()) {
            auto best_bid = bids_.begin();
            if (price > best_bid->first) {
                break;
            }

            auto& resting = best_bid->second.front();
            const Quantity traded_quantity =
                incoming.remaining_quantity < resting.remaining_quantity
                    ? incoming.remaining_quantity
                    : resting.remaining_quantity;

            trades.push_back(Trade{
                .buy_order_id = resting.id,
                .sell_order_id = incoming.id,
                .price = resting.price,
                .quantity = traded_quantity
            });

            incoming.remaining_quantity -= traded_quantity;
            resting.remaining_quantity -= traded_quantity;

            if (resting.remaining_quantity == 0) {
                best_bid->second.pop_front();
                if (best_bid->second.empty()) {
                    bids_.erase(best_bid);
                }
            }
        }

        if (incoming.remaining_quantity > 0) {
            asks_[incoming.price].push_back(incoming);
        }
    }

    return trades;
}

bool OrderBook::empty() const {
    return bids_.empty() && asks_.empty();
}

}  // namespace engine
