#include "order_book.h"

std::vector<Trade> OrderBook::add_limit_order(
    OrderId order_id,
    Side side,
    Price price,
    Quantity quantity) {
    static_cast<void>(order_id);
    static_cast<void>(side);
    static_cast<void>(price);
    static_cast<void>(quantity);

    return {};
}

bool OrderBook::empty() const {
    return bids_.empty() && asks_.empty();
}
