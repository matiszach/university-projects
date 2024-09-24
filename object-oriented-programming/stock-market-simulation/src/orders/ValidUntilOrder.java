package orders;

import investors.Investor;

public class ValidUntilOrder extends Order{
    // data
    private int validUntil;
    // constructor
    public ValidUntilOrder(Investor investor, int activationLimit, int amount, int turn, int number, int validUntil){
        super(investor, activationLimit, amount, turn, number);
        this.validUntil = validUntil;
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

    public Investor getInvestor(){
        return investor;
    }

    public boolean isValid(int turn){
        return turn <= validUntil;
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
        return "VU";
    }
}
