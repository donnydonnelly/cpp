# cpp

This repository contains an aggregation of some finance-related applications utilizing C++ as completed during Fordham's Quantitative Finance program.

## Implied Volatility
Implied volatility curve generator using market data.
#### Date.cpp / Date.h
Defines a Date object that is easier to work with.
#### Discount
Defines a Discount object that stores vectors of discount rates and dates.
#### Forward
Defines a Forward object that stores spot rates, discount rates, divdend yields, and dates.
#### Option
Defines an Option object that holds an option's underlyer, currency, notional, option type, expiry date, and strike price.
Defines an OptionPrice object that is made up of an Option object and a price.
#### bisect
Impliments an array bisection algorithm.
#### black_scholes
Impliments the black-scholes option pricing formula and an implied volatility calculator that uses the black-scholes option pricing formula and bisection algorithm.
#### bootstrap_volatility.cpp
Reads market option price, forward, and discount data and outputs an implied volatility curve.
#### interpolate.cpp
A simple interpolation function implementation.
#### parse.cpp / parse.h / parse_forwards.cpp
Helper files for reading data from CSV files.
