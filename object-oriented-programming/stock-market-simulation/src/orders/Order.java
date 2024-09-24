package orders;

import investors.Investor;

public abstract class Order {
    // data
    protected int activationLimit;
    protected int amount;
    protected int turn;
    protected int number;
    protected Investor investor;
    // constructor
    public Order(Investor investor, int activationLimit, int amount, int turn, int number){
        this.investor = investor;
        this.activationLimit = activationLimit;
        this.amount = amount;
        this.turn = turn;
        this.number = number;
    }
    // methods
    public abstract int getAmount();
    public abstract int getPrice();
    public abstract int getTurn();
    public abstract int getNumber();
    public abstract Investor getInvestor();
    public abstract boolean isValid(int turn);
    public abstract boolean comparePriority(Order order);
    public abstract void setAmount(int amount);
    public abstract void setNumber(int number);
    public abstract String toString();
}
