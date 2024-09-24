# Stock Market Simulation

## Description

This is a simplified simulation of the Stock Exchange featuring some of its core mechanisms:
- **Investors**: There are investors participating in the martket following one of 2 trading strategies (SMA and Random)
- **Tradable Instruments**: Every instrument is identified by its tickers (e.g. APL, GOOGL) and can be traded between investors participating in the market. Instrument's current price is the price of its last transaction.
- **Buy & Sell Orders**: Investors can post buy and sell orders of 3 types (Immediate Order, Valid Until Order, No Expiration Order).
- **Fullfil System**: Exchange system tries to fullfil the orders based on the rules from [GPW](https://www.gpw.pl/pub/images/prezentacje/system_obrotu.pdf) website.
The Project is fully following the OOP principles such as Polymorphism, Inheritance and Encapsulation which makes it very easy to add new functionalities like new investment strategies, order types or financial instruments. 
