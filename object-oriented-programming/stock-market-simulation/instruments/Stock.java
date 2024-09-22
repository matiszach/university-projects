package instruments;

import systems.Chart;

public class Stock implements Tradable{
    // data
    private final String ticker;
    private Chart chart;
    // constructor
    public Stock(String ticker, int initialPrice){
        this.ticker = ticker;
        this.chart = new Chart(initialPrice);
    }
    // methods
    public String getTicker(){
        return ticker;
    }
    public double getSma(int from, int k){
        return chart.getSma(from, k);
    }
    public int getLastPrice(){
        return chart.getLastPrice();
    }

    public void setLastPrice(int price, int turn){
        chart.setLastPrice(price, turn);
    }
}
