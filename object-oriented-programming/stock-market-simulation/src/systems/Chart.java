package systems;

import java.util.Vector;

public class Chart {
    // data
    Vector<Integer> prices;
    // constructor
    public Chart(int initialPrice){
        this.prices = new Vector<Integer>();
        prices.add(initialPrice);
    }
    // methods
    public void setLastPrice(int price, int turn){
        while(prices.size() <= turn){
            prices.add(prices.lastElement());
        }
        prices.set(prices.size() - 1, price);
    }

    public double getSma(int from, int k){
        assert k <= from && k > 0 && from <= prices.size() : " Nie można policzyć SMA " + k + " dla " + from + " tury";
        int sum = 0;
        for(int i = 0; i < k; ++i){
            sum += prices.elementAt(from - 1 - i);
        }
        return (double)sum / k;
    }

    public int getLastPrice(){
        return prices.lastElement();
    }

    public int priceAt(int turn){
        assert turn >= 0 && turn < prices.size() : "invalid turn";
        return prices.elementAt(turn);
    }
}
