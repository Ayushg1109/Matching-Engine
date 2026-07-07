#include "../engine/order_book.h"

#include <cassert>
#include <iostream>

int main() {
    engine::Order order{
        .id = 1,
        .side = engine::Side::Buy,
        .price = 100,
        .remaining_quantity = 10,
        .sequence_number = 1
    };

    assert(order.id == 1);
    assert(order.side == engine::Side::Buy);
    assert(order.price == 100);
    assert(order.remaining_quantity == 10);
    assert(order.sequence_number == 1);

    engine::Trade trade{
        .buy_order_id = 1,
        .sell_order_id = 2,
        .price = 100,
        .quantity = 5
    };

    assert(trade.buy_order_id == 1);
    assert(trade.sell_order_id == 2);
    assert(trade.price == 100);
    assert(trade.quantity == 5);

    engine::OrderBook book;
    assert(book.empty());

    std::cout << "Day 1 basic test passed\n";
}