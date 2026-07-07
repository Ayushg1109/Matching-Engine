#include "../engine/order_book.h"

#include <cassert>
#include <iostream>
#include <vector>

namespace {

void sell_rests_when_no_bids_exist() {
    engine::OrderBook book;

    const std::vector<engine::Trade> trades =
        book.add_limit_order(1, engine::Side::Sell, 100, 10);

    assert(trades.empty());
    assert(!book.empty());
}

void buy_rests_when_no_asks_exist() {
    engine::OrderBook book;

    const std::vector<engine::Trade> trades =
        book.add_limit_order(1, engine::Side::Buy, 100, 10);

    assert(trades.empty());
    assert(!book.empty());
}

void buy_matches_resting_sell() {
    engine::OrderBook book;

    assert(book.add_limit_order(1, engine::Side::Sell, 100, 10).empty());
    const std::vector<engine::Trade> trades =
        book.add_limit_order(2, engine::Side::Buy, 100, 10);

    assert(trades.size() == 1);
    assert(trades[0].buy_order_id == 2);
    assert(trades[0].sell_order_id == 1);
    assert(trades[0].price == 100);
    assert(trades[0].quantity == 10);
    assert(book.empty());
}

void sell_matches_resting_buy() {
    engine::OrderBook book;

    assert(book.add_limit_order(1, engine::Side::Buy, 100, 10).empty());
    const std::vector<engine::Trade> trades =
        book.add_limit_order(2, engine::Side::Sell, 100, 10);

    assert(trades.size() == 1);
    assert(trades[0].buy_order_id == 1);
    assert(trades[0].sell_order_id == 2);
    assert(trades[0].price == 100);
    assert(trades[0].quantity == 10);
    assert(book.empty());
}

void buy_matches_lowest_ask_first() {
    engine::OrderBook book;

    assert(book.add_limit_order(1, engine::Side::Sell, 105, 5).empty());
    assert(book.add_limit_order(2, engine::Side::Sell, 100, 5).empty());

    const std::vector<engine::Trade> trades =
        book.add_limit_order(3, engine::Side::Buy, 105, 10);

    assert(trades.size() == 2);
    assert(trades[0].buy_order_id == 3);
    assert(trades[0].sell_order_id == 2);
    assert(trades[0].price == 100);
    assert(trades[0].quantity == 5);
    assert(trades[1].buy_order_id == 3);
    assert(trades[1].sell_order_id == 1);
    assert(trades[1].price == 105);
    assert(trades[1].quantity == 5);
    assert(book.empty());
}

void sell_matches_highest_bid_first() {
    engine::OrderBook book;

    assert(book.add_limit_order(1, engine::Side::Buy, 100, 5).empty());
    assert(book.add_limit_order(2, engine::Side::Buy, 105, 5).empty());

    const std::vector<engine::Trade> trades =
        book.add_limit_order(3, engine::Side::Sell, 100, 10);

    assert(trades.size() == 2);
    assert(trades[0].buy_order_id == 2);
    assert(trades[0].sell_order_id == 3);
    assert(trades[0].price == 105);
    assert(trades[0].quantity == 5);
    assert(trades[1].buy_order_id == 1);
    assert(trades[1].sell_order_id == 3);
    assert(trades[1].price == 100);
    assert(trades[1].quantity == 5);
    assert(book.empty());
}

void partial_fill_of_incoming_order() {
    engine::OrderBook book;

    assert(book.add_limit_order(1, engine::Side::Sell, 100, 5).empty());
    std::vector<engine::Trade> trades =
        book.add_limit_order(2, engine::Side::Buy, 101, 10);

    assert(trades.size() == 1);
    assert(trades[0].buy_order_id == 2);
    assert(trades[0].sell_order_id == 1);
    assert(trades[0].price == 100);
    assert(trades[0].quantity == 5);
    assert(!book.empty());

    trades = book.add_limit_order(3, engine::Side::Sell, 101, 5);
    assert(trades.size() == 1);
    assert(trades[0].buy_order_id == 2);
    assert(trades[0].sell_order_id == 3);
    assert(trades[0].price == 101);
    assert(trades[0].quantity == 5);
    assert(book.empty());
}

void partial_fill_of_resting_order() {
    engine::OrderBook book;

    assert(book.add_limit_order(1, engine::Side::Sell, 100, 10).empty());
    std::vector<engine::Trade> trades =
        book.add_limit_order(2, engine::Side::Buy, 100, 4);

    assert(trades.size() == 1);
    assert(trades[0].buy_order_id == 2);
    assert(trades[0].sell_order_id == 1);
    assert(trades[0].price == 100);
    assert(trades[0].quantity == 4);
    assert(!book.empty());

    trades = book.add_limit_order(3, engine::Side::Buy, 100, 6);
    assert(trades.size() == 1);
    assert(trades[0].buy_order_id == 3);
    assert(trades[0].sell_order_id == 1);
    assert(trades[0].price == 100);
    assert(trades[0].quantity == 6);
    assert(book.empty());
}

void fifo_at_same_price() {
    engine::OrderBook book;

    assert(book.add_limit_order(1, engine::Side::Sell, 100, 5).empty());
    assert(book.add_limit_order(2, engine::Side::Sell, 100, 5).empty());

    std::vector<engine::Trade> trades =
        book.add_limit_order(3, engine::Side::Buy, 101, 7);

    assert(trades.size() == 2);
    assert(trades[0].buy_order_id == 3);
    assert(trades[0].sell_order_id == 1);
    assert(trades[0].price == 100);
    assert(trades[0].quantity == 5);
    assert(trades[1].buy_order_id == 3);
    assert(trades[1].sell_order_id == 2);
    assert(trades[1].price == 100);
    assert(trades[1].quantity == 2);
    assert(!book.empty());

    trades = book.add_limit_order(4, engine::Side::Buy, 100, 3);
    assert(trades.size() == 1);
    assert(trades[0].buy_order_id == 4);
    assert(trades[0].sell_order_id == 2);
    assert(trades[0].price == 100);
    assert(trades[0].quantity == 3);
    assert(book.empty());
}

}  // namespace

int main() {
    sell_rests_when_no_bids_exist();
    buy_rests_when_no_asks_exist();
    buy_matches_resting_sell();
    sell_matches_resting_buy();
    buy_matches_lowest_ask_first();
    sell_matches_highest_bid_first();
    partial_fill_of_incoming_order();
    partial_fill_of_resting_order();
    fifo_at_same_price();

    std::cout << "Day 2 matching test passed\n";
    return 0;
}
