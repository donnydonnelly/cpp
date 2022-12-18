# cpp

This repository contains an aggregation of some finance-related applications utilizing C++ as completed during Fordham's Quantitative Finance program.

## Implied Volatility
An implied volatility curve generator. The code was written with the assistance of Fordham and Columbia professor Manuel Balsera.
#### Date.cpp / Date.h
Defines a Date object that is easier to work with.
#### Discount.cpp / Discount.h
Defines a Discount object that stores vectors of discount rates and dates.
#### Forward.h
Defines a Forward object that stores spot rates, discount rates, divdend yields, and dates.
#### Option.cpp / Option.h
Defines an Option object that holds an option's underlyer, currency, notional, option type, expiry date, and strike price.
Defines an OptionPrice object that is made up of an Option object and a price.
#### bisect.h
Implements an array bisection algorithm.
#### black_scholes.cpp / black_scholes.h
Implements the Black-Scholes option pricing formula and an implied volatility calculator that uses the Black-Scholes option pricing formula and bisection algorithm.
#### bootstrap_volatility.cpp
Reads market option price, forward, and discount data and outputs an implied volatility curve.
#### interpolate.cpp / interpolate.h
A simple interpolation function implementation.
#### parse.cpp / parse.h / parse_forwards.cpp
Helper files for reading data from CSV files.
