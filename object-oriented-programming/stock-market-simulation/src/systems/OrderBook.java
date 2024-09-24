package systems;

import instruments.Stock;
import orders.Order;

import java.util.Comparator;
import java.util.TreeSet;

public class OrderBook {
    // data
    private TreeSet<Order> sellOrders;
    private TreeSet<Order> buyOrders;
    private Stock stock;

    // constructor
    public OrderBook(Stock stock) {
        this.stock = stock;
        this.sellOrders = new TreeSet<Order>(new Comparator<Order>(){
            @Override
            public int compare(Order o1, Order o2){
                if(o1.getPrice() > o2.getPrice()) return -1;
                if(o1.getPrice() < o2.getPrice()) return 1;
                if(o1.getTurn() < o2.getTurn()) return -1;
                if(o1.getTurn() > o2.getTurn()) return 1;
                return Integer.compare(o1.getNumber(), o2.getNumber());
            }
        });
        this.buyOrders = new TreeSet<Order>(new Comparator<Order>(){
            @Override
            public int compare(Order o1, Order o2){
                if(o1.getPrice() > o2.getPrice()) return 1;
                if(o1.getPrice() < o2.getPrice()) return -1;
                if(o1.getTurn() < o2.getTurn()) return -1;
                if(o1.getTurn() > o2.getTurn()) return 1;
                return Integer.compare(o1.getNumber(), o2.getNumber());
            }
        });
    }

    // methods
    public void placeBuyOrder(Order order){
        buyOrders.add(order);
    }

    public void placeSellOrder(Order order){
        sellOrders.add(order);
    }

    // process all the orders in the order book
    public void processOrders(int currentTurn){
        while(true){
            if(buyOrders.isEmpty()) break;
            Order order = buyOrders.first();
            buyOrders.removeFirst();
            if(!order.isValid(currentTurn)) continue;

            int amount = order.getAmount();
            if(order.getInvestor().getPortfolio().getFunds() < amount * order.getPrice()) continue;

            while(amount > 0){
                if(sellOrders.isEmpty()) break;
                Order sOrder = sellOrders.first();
                sellOrders.removeFirst();
                if(!sOrder.isValid(currentTurn)) continue;
                if(sOrder.getPrice() > order.getPrice()) break;

                int volume = Math.min(amount, sOrder.getAmount());
                int price = sOrder.getPrice();
                if(order.comparePriority(sOrder)) price = order.getPrice();

                if(sOrder.getInvestor().getPortfolio().quantity(stock) < volume) continue;

                stock.setLastPrice(price, currentTurn);
                order.getInvestor().getPortfolio().buy(order.getInvestor(), stock, volume, price);
                sOrder.getInvestor().getPortfolio().sell(sOrder.getInvestor(), stock, volume, price);

                if(volume < sOrder.getAmount()){
                    sOrder.setAmount(sOrder.getAmount() - volume);
                    sellOrders.add(sOrder);
                }

                amount -= volume;
            }
            // amount > 0 to dodać z powrotem
            if(amount > 0){
                order.setAmount(amount);
                buyOrders.add(order);
                break;
            }
        }
    }
}
