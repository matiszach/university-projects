package investors;

import exchanges.Exchange;
import instruments.Stock;
import orders.Order;
import systems.Portfolio;
import utils.Randomizer;

public class RANDOM extends Investor{
    // data
    // constructor
    public RANDOM(String name, Portfolio portfolio){
        super(name, portfolio);
    }
    // methods
    public void makeDecision(Exchange exchange){
        Stock stock = exchange.stockAt(Randomizer.randRange(0, exchange.stocksAmount() - 1));
        int operationType = Randomizer.randRange(0, 1);
        int turn = exchange.getCurrentTurn();

        // buy
        if(operationType == 0){
            int price = Math.max(1, stock.getLastPrice() + Randomizer.randRange(-2, 10));
            int amount = Randomizer.randRange(portfolio.getFunds() / 10, portfolio.getFunds()) / price;
            if(amount == 0){
                amount = 1;
                if(price > portfolio.getFunds()) return;
            }

            Order order = exchange.randomOrder(this, price, amount, turn, 0);
            exchange.placeBuyOrder(stock, order);
        }
        // sell
        else{
            if(portfolio.quantity(stock) == 0) return;
            int price = Math.max(1, stock.getLastPrice() + Randomizer.randRange(-5, 2));
            int amount = Randomizer.randRange(1, portfolio.quantity(stock));

            Order order = exchange.randomOrder(this, price, amount, turn, 0);
            exchange.placeSellOrder(stock, order);
        }
    }

    public Portfolio getPortfolio(){
        return portfolio;
    }

    public String getName(){
        return name;
    }
}
