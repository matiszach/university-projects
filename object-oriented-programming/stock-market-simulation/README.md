# Stock Market Simulation

## Description

This is a simplified simulation of the Stock Exchange featuring some of its core mechanisms:
- **Investors**: Investors are participating in the market following one of 2 trading strategies (SMA and Random).
- **Tradable Instruments**: Every instrument is identified by its ticker (e.g. APL, GOOGL) and can be traded between investors participating in the market. The instrument's current price is the price of its last transaction.
- **Buy & Sell Orders**: Investors can post buy and sell orders of 3 types (Immediate Order, Valid Until Order, No Expiration Order).
- **Fullfilling System**: The exchange system tries to fulfill orders based on the rules on the [GPW](https://www.gpw.pl/pub/images/prezentacje/system_obrotu.pdf) website.

The entire simulation takes place in a turn-based system where every turn all the investors post buy and sell orders based on the behavior specified by their strategies. At the end of each turn, Exchange fulfills the orders.

The Project fully follows the OOP principles such as Polymorphism, Inheritance, and Encapsulation which makes it very easy to add new features like investment strategies, order types, or financial instruments.

## Input Format

The first line of input consists of letters identifying the investment strategies used by each investor participating in the market (R - Random, S - SMA).

That's how to add 5 investors, where 1st, 3rd, and 4th follow the Random strategy whereas 2nd and 5th follow the SMA strategy.
```
R S R R S
```

The second line of input specifies the list of available instruments with their initial last transaction price.
```
APL:145 MSFT:300 GOOGL:2700
```

The third line of input sets the initial state of the investor's portfolio (identical for every investor).

The first number specifies the initial amount of money, and then the list of the instruments with their quantities.
```
5000 APL:5 MSFT:15 GOOGL:3
```

Everything that comes after **#** symbol in its line is ignored so it can be used for comments.

There's an example.txt provided which is an example input file and sample tests in **tests** folder.

## Usage

Download the project folder.
```
git clone --no-checkout https://github.com/matiszach/university-projects.git
cd university-projects
git sparse-checkout init --cone
git sparse-checkout set object-oriented-programming/stock-market-simulation
git checkout
cd object-oriented-programming/stock-market-simulation
```

Use JetBrains IntelliJ IDEA (or any preferred Java build tool/system) to build the project.

Run Main with 2 arguments; the first being the input file's name and the second number of turns of simulation.
```
java Main ../example.txt 1000
```
