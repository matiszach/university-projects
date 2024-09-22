package systems;

import instruments.Stock;
import investors.Investor;
import utils.Pair;

import java.util.HashMap;

public class Portfolio {
    // data
    private int freeFunds;
    private HashMap<Stock, Integer> assets;

    // constructor
    public Portfolio(int freeFunds, Pair<Stock, Integer>[] assets){
        this.freeFunds = freeFunds;
        this.assets = new HashMap<Stock, Integer>();
        for(int i = 0; i < assets.length; ++i){
            this.assets.put(assets[i].getFirst(), assets[i].getSecond());
        }
    }
    // methods
    public int quantity(Stock stock){
        if(assets.containsKey(stock)) return assets.get(stock);
        return 0;
    }

    public int getFunds(){
        return freeFunds;
    }

    public void addStock(Stock stock, int quantity){
        assets.merge(stock, quantity, Integer::sum);
        if(assets.get(stock) == 0){
            assets.remove(stock);
        }
    }

    public void addFunds(int amount){
        freeFunds += amount;
    }

    public void buy(Investor investor, Stock stock, int quantity, int price){
        System.out.println(investor.getName() + " bought " + quantity + " " + stock.getTicker() + " for $" + price);
        freeFunds -= price * quantity;
        addStock(stock, quantity);
    }

    public void sell(Investor investor, Stock stock, int quantity, int price){
        System.out.println(investor.getName() + " sold " + quantity + " " + stock.getTicker() + " for $" + price);
        freeFunds += price * quantity;
        addStock(stock, -quantity);
    }

    @Override
    public String toString(){
        String res = "";
        for(Stock key : assets.keySet()){
            res += key.getTicker() + " : " + assets.get(key) + "\n";
        }
        return res;
    }
}
