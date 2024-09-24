package instruments;

public interface Tradable {
    public String getTicker();
    public double getSma(int from, int k);
    public int getLastPrice();
    public void setLastPrice(int price, int turn);
}
