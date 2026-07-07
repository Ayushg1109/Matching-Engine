#include "../engine/order_book.h"

#include <cassert>
#include <iostream>

int main() {
    using engine::OrderBook;
    using engine::Side;

    {
        OrderBook book;

        auto trades = book.add_limit_order(1, Side::Sell, 100, 10);

        assert(trades.empty());
        assert(!book.empty());
    }

    {
        OrderBook book;

        auto trades = book.add_limit_order(1, Side::Buy, 100, 10);

        assert(trades.empty());
        assert(!book.empty());
    }

    {
        OrderBook book;

        book.add_limit_order(1, Side::Sell, 100, 10);
        auto trades = book.add_limit_order(2, Side::Buy, 100, 10);

        assert(trades.size() == 1);
        assert(trades[0].buy_order_id == 2);
        assert(trades[0].sell_order_id == 1);
        assert(trades[0].price == 100);
        assert(trades[0].quantity == 10);

        assert(book.empty());
    }

    {
        OrderBook book;

        book.add_limit_order(1, Side::Buy, 100, 10);
        auto trades = book.add_limit_order(2, Side::Sell, 100, 10);

        assert(trades.size() == 1);
        assert(trades[0].buy_order_id == 1);
        assert(trades[0].sell_order_id == 2);
        assert(trades[0].price == 100);
        assert(trades[0].quantity == 10);

        assert(book.empty());
    }

    {
        OrderBook book;

        book.add_limit_order(1, Side::Sell, 100, 10);
        auto trades = book.add_limit_order(2, Side::Buy, 100, 4);

        assert(trades.size() == 1);
        assert(trades[0].buy_order_id == 2);
        assert(trades[0].sell_order_id == 1);
        assert(trades[0].price == 100);
        assert(trades[0].quantity == 4);

        assert(!book.empty());
    }

    {
        OrderBook book;

        book.add_limit_order(1, Side::Sell, 100, 4);
        auto trades = book.add_limit_order(2, Side::Buy, 100, 10);

        assert(trades.size() == 1);
        assert(trades[0].buy_order_id == 2);
        assert(trades[0].sell_order_id == 1);
        assert(trades[0].price == 100);
        assert(trades[0].quantity == 4);

        assert(!book.empty());
    }

    {
        OrderBook book;

        book.add_limit_order(1, Side::Sell, 100, 10);
        book.add_limit_order(2, Side::Sell, 100, 20);

        auto trades = book.add_limit_order(3, Side::Buy, 100, 25);

        assert(trades.size() == 2);

        assert(trades[0].buy_order_id == 3);
        assert(trades[0].sell_order_id == 1);
        assert(trades[0].price == 100);
        assert(trades[0].quantity == 10);

        assert(trades[1].buy_order_id == 3);
        assert(trades[1].sell_order_id == 2);
        assert(trades[1].price == 100);
        assert(trades[1].quantity == 15);

        assert(!book.empty());
    }

    {
        OrderBook book;

        book.add_limit_order(1, Side::Sell, 101, 10);
        auto trades = book.add_limit_order(2, Side::Buy, 100, 10);

        assert(trades.empty());
        assert(!book.empty());
    }

    std::cout << "Day 2 matching test passed\n";
}