# Matching Engine

This is a production-inspired C++ matching engine project.

Day 1 contains the core types and the `OrderBook` skeleton.

Day 2 adds normal limit-order matching:

- Buy orders match against the lowest resting ask.
- Sell orders match against the highest resting bid.
- Orders at the same price match FIFO.
- Leftover quantity rests on the book.

Prices are stored as integers, not `double`, to avoid floating-point precision issues in price comparisons and accounting.
