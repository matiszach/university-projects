# Stock Market Simulation

## Description

This is a simplified simulation of the Stock Exchange featuring some of its core mechanisms:
- **Investors**: There are investors participating in the martket following one of 2 trading strategies (SMA and Random)
- **Tradable Instruments**: Every instrument is identified by its tickers (e.g. APL, GOOGL) and can be traded between investors participating in the market. Instrument's current price is the price of its last transaction.
- **Buy & Sell Orders**: Investors can post buy and sell orders of 3 types (Immediate Order, Valid Until Order, No Expiration Order).
- **Fullfil System**: Exchange system tries to fullfil the orders based on the rules from [GPW](https://www.gpw.pl/pub/images/prezentacje/system_obrotu.pdf) website.

Entire simulation takes place in turn based system where every turn all the investors take post buy and sell orders based on the behaviour specified by their stategies. At the end of each turn Exchange fullfils the orders.
The Project is fully following the OOP principles such as Polymorphism, Inheritance and Encapsulation which makes it very easy to add new features like investment strategies, order types or financial instruments. 

## Input Format

First line of input consists of letters identifying the investment strategies used by each of the investors participating in the market (R - Random, S - SMA).

That's how to add 5 investors, where 1st, 3rd and 4th follow Random stategy whereas 2nd and 5th follow SMA stategy.
```
R S R R S
```

Second line of input specifies the list of available instruments with their initial last transaction price.
```
APL:145 MSFT:300 GOOGL:2700
```

Third line of input sets the initial state of the investor's portfolio (identical for every investor).

First number specifies the initial amount of money, and then the list of the instruments with their quantities.
```
5000 APL:5 MSFT:15 GOOGL:3
```

## Usage

