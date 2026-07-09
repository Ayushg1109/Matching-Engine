#include "../engine/order_book.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

int main() {
    using engine::OrderBook;
    using engine::Side;

    {
        OrderBook book;

        book.add_limit_order(1, Side::Sell, 100, 10);

        assert(book.contains_order(1));
        assert(!book.empty());
    }

    {
        OrderBook book;

        book.add_limit_order(1, Side::Sell, 100, 10);
        auto trades = book.add_limit_order(2, Side::Buy, 100, 10);

        assert(trades.size() == 1);
        assert(!book.contains_order(1));
        assert(!book.contains_order(2));
        assert(book.empty());
    }

    {
        OrderBook book;

        book.add_limit_order(1, Side::Sell, 100, 10);
        auto trades = book.add_limit_order(2, Side::Buy, 100, 4);

        assert(trades.size() == 1);
        assert(book.contains_order(1));
        assert(!book.contains_order(2));
        assert(!book.empty());
    }

    {
        OrderBook book;

        book.add_limit_order(1, Side::Sell, 100, 4);
        auto trades = book.add_limit_order(2, Side::Buy, 100, 10);

        assert(trades.size() == 1);
        assert(!book.contains_order(1));
        assert(book.contains_order(2));
        assert(!book.empty());
    }

    {
        OrderBook book;

        book.add_limit_order(1, Side::Buy, 100, 10);

        bool duplicate_rejected = false;

        try {
            book.add_limit_order(1, Side::Buy, 99, 5);
        } catch (const std::invalid_argument&) {
            duplicate_rejected = true;
        }

        assert(duplicate_rejected);
    }

    std::cout << "Day 3 order lookup test passed\n";
}
