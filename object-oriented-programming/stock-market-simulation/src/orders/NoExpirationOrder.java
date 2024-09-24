package orders;

import investors.Investor;

public class NoExpirationOrder extends Order{
    // data
    // constructor
    public NoExpirationOrder(Investor investor, int activationLimit, int amount, int turn, int number){
        super(investor, activationLimit, amount, turn, number);
    }
    // methods
    public int getAmount(){
        return amount;
    }

    public int getPrice() {
        return activationLimit;
    }

    public int getTurn() {
        return turn;
    }

    public int getNumber() {
        return number;
    }

    public boolean isValid(int turn){
        return true;
    }

    public Investor getInvestor(){
        return investor;
    }

    public boolean comparePriority(Order order){
        if(turn < order.getTurn()) return true;
        if(turn > order.getTurn()) return false;
        return number < order.getNumber();
    }

    public void setAmount(int amount){
        this.amount = amount;
    }

    public void setNumber(int number){
        this.number = number;
    }

    public String toString(){
        return "NE";
    }
}
