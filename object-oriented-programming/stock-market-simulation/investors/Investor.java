package investors;

import exchanges.Exchange;
import systems.Portfolio;

public abstract class Investor {
    // data
    protected Portfolio portfolio;
    protected String name;
    // constructor
    public Investor(String name, Portfolio portfolio){
        this.name = name;
        this.portfolio = portfolio;
    }
    // methods
    public abstract void makeDecision(Exchange exchange);
    public abstract Portfolio getPortfolio();
    public abstract String getName();
}
