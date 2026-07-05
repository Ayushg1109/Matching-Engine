#include "../engine/order_book.h"

#include <cassert>
#include <iostream>

int main() {
    Order order{
        .id = 1,
        .side = Side::Buy,
        .price = 10000,
        .remaining_quantity = 25,
        .sequence_number = 7
    };

    assert(order.id == 1);
    assert(order.side == Side::Buy);
    assert(order.price == 10000);
    assert(order.remaining_quantity == 25);
    assert(order.sequence_number == 7);

    Trade trade{
        .buy_order_id = 1,
        .sell_order_id = 2,
        .price = 10005,
        .quantity = 10
    };

    assert(trade.buy_order_id == 1);
    assert(trade.sell_order_id == 2);
    assert(trade.price == 10005);
    assert(trade.quantity == 10);

    OrderBook book;
    assert(book.empty());

    std::cout << "Day 1 basic test passed\n";
    return 0;
}
