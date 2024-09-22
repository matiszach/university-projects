package orders;

import investors.Investor;

public class FulfillOrCancelOrder extends Order{
    // data
    // constructor
    public FulfillOrCancelOrder(Investor investor, int activationLimit, int amount, int turn, int number){
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

    public Investor getInvestor(){
        return investor;
    }

    // this type of order is not implemented and is never valid
    public boolean isValid(int turn){
        return false;
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

    @Override
    public String toString(){
        return "FC";
    }
}
