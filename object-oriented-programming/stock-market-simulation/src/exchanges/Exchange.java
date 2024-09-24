package exchanges;

import instruments.Stock;
import investors.Investor;
import investors.RANDOM;
import investors.SMA;
import orders.ImmidiateOrder;
import orders.NoExpirationOrder;
import orders.Order;
import orders.ValidUntilOrder;
import systems.OrderBook;
import systems.Portfolio;
import utils.Pair;
import utils.Randomizer;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.HashMap;
import java.util.Scanner;
import java.util.Vector;

public class Exchange {
    // data
    private Stock[] stocks;
    private Investor[] investors;
    private HashMap<Stock, OrderBook> orderBooks;
    private int currentTurn;
    private int number;
    // constructor
    public Exchange(Stock[] stocks, Investor[] investors){
        this.stocks = stocks;
        this.orderBooks = new HashMap<Stock, OrderBook>();
        for (int i = 0; i < stocks.length; ++i) {
            orderBooks.put(stocks[i], new OrderBook(stocks[i]));
        }
        this.investors = investors;
        this.currentTurn = 0;
        this.number = 0;
    }

    // constructor from file
    public Exchange(String fileName){
        Investor[] _investors = new Investor[0];
        Stock[] _stocks = new Stock[0];

        HashMap<String, Stock> availableStocks = new HashMap<String, Stock>();
        System.out.println();
        try(Scanner scanner = new Scanner(new File(fileName))){
            String line;

            // investor types
            do{
                line = scanner.nextLine();
            }while(line.charAt(0) == '#');

            Scanner lineScanner = new Scanner(line);
            Vector<String> types = new Vector<String>();
            while(lineScanner.hasNext()){
                String type = lineScanner.next();
                System.out.print(type + " ");
                types.add(type);
            }

            System.out.println();

            // stock types
            do{
                line = scanner.nextLine();
            }while(line.charAt(0) == '#');

            lineScanner = new Scanner(line);
            Vector<String> tickers = new Vector<String>();
            Vector<Integer> prices = new Vector<Integer>();
            while(lineScanner.hasNext()){
                String stock = lineScanner.next();
                int price = Integer.parseInt(stock.substring(stock.indexOf(":") + 1, stock.length()));
                String ticker = stock.substring(0, stock.indexOf(":"));
                System.out.print(ticker + ":" + price + " ");
                tickers.add(ticker);
                prices.add(price);
            }

            System.out.println();

            lineScanner.close();
            _stocks = new Stock[tickers.size()];

            for(int i = 0; i < _stocks.length; ++i){
                _stocks[i] = new Stock(tickers.elementAt(i), prices.elementAt(i));
                availableStocks.put(tickers.elementAt(i), _stocks[i]);
            }

            // portfolio
            do{
                line = scanner.nextLine();
            }while(line.charAt(0) == '#');

            lineScanner = new Scanner(line);

            int cash = lineScanner.nextInt();
            System.out.print(cash + " ");

            Vector<Stock> stockParts = new Vector<Stock>();
            Vector<Integer> amounts = new Vector<Integer>();

            while(lineScanner.hasNext()){
                String stock = lineScanner.next();
                int amount = Integer.parseInt(stock.substring(stock.indexOf(":") + 1, stock.length()));
                String ticker = stock.substring(0, stock.indexOf(":"));

                System.out.print(ticker + " " + amount + " ");

                assert availableStocks.containsKey(ticker) : "Nieznany ticker [" + ticker + "]! (nie ma go w systemie)";

                stockParts.add(availableStocks.get(ticker));
                amounts.add(amount);
            }

            System.out.println();

            lineScanner.close();

            Pair<Stock, Integer>[] assets = new Pair[stockParts.size()];
            for(int i = 0; i < stockParts.size(); ++i){
                assets[i] = new Pair<Stock, Integer>(stockParts.elementAt(i), amounts.elementAt(i));
            }

            // investors
            _investors = new Investor[types.size()];
            for(int i = 0; i < types.size(); ++i){
                if(types.elementAt(i).equals("R")){
                    _investors[i] = new RANDOM(Integer.toString(i), new Portfolio(cash, assets));
                }
                if(types.elementAt(i).equals("S")){
                    _investors[i] = new SMA(Integer.toString(i), new Portfolio(cash, assets));
                }
            }

            lineScanner.close();
            scanner.close();
        } catch(FileNotFoundException e){
            e.printStackTrace();
        }

        System.out.println();

        this.stocks = _stocks;
        this.orderBooks = new HashMap<Stock, OrderBook>();
        for (int i = 0; i < _stocks.length; ++i) {
            orderBooks.put(_stocks[i], new OrderBook(_stocks[i]));
        }
        this.investors = _investors;
        this.currentTurn = 0;
        this.number = 0;
    }
    // methods
    public int getCurrentTurn(){
        return currentTurn;
    }

    // simulation of trading for certain number of turns
    public void simulate(int turns){
        for(int i = 1; i <= turns; ++i){
            ++currentTurn;
            number = 0;
            simulateTurn();
        }
    }

    private void simulateTurn(){
        System.out.println("[            TURN " + currentTurn + "            ]");
        // add last price for every stock
        for(Stock stock : stocks){
            stock.setLastPrice(stock.getLastPrice(), currentTurn);
        }
        System.out.println("\nInvestors are making decisions:\n");
        // make investing decisions
        for(Investor investor : investors){
            investor.makeDecision(this);
        }
        System.out.println("\nOrders are being processed:\n");
        // process orders
        for(Stock stock : stocks){
            orderBooks.get(stock).processOrders(currentTurn);
        }
        System.out.println("\n################################################################\n");
    }

    public int totalCash(){
        int res = 0;
        for(Investor investor : investors){
            res += investor.getPortfolio().getFunds();
        }
        return res;
    }

    public int totalStockAmount(Stock stock){
        int res = 0;
        for(Investor investor : investors) {
            res += investor.getPortfolio().quantity(stock);
        }
        return res;
    }

    public int stocksAmount(){
        return stocks.length;
    }

    public Stock stockAt(int k){
        assert k >= 0 && k < stocks.length;
        return stocks[k];
    }

    public void placeBuyOrder(Stock stock, Order order){
        System.out.println("#" + number + " > " + order.getInvestor().getName() + " placed " + order + " Order [BUY] "
                           + order.getAmount() + " " + stock.getTicker() + " $" + order.getPrice());
        order.setNumber(number++);
        orderBooks.get(stock).placeBuyOrder(order);
    }

    public void placeSellOrder(Stock stock, Order order){
        System.out.println("#" + number + " > " + order.getInvestor().getName() + " placed " + order + " Order [SELL] "
                           + order.getAmount() + " " + stock.getTicker() + " $" + order.getPrice());
        order.setNumber(number++);
        orderBooks.get(stock).placeSellOrder(order);
    }

    public Order randomOrder(Investor investor, int activationLimit, int amount, int turn, int number){
        int orderType = Randomizer.randRange(0, 3);

        if(orderType == 0){
            return new ImmidiateOrder(investor, activationLimit, amount, turn, number);
        }
        if(orderType == 1){
            return new NoExpirationOrder(investor, activationLimit, amount, turn, number);
        }

        int validUntil = turn + Randomizer.randRange(0, 1000);
        return new ValidUntilOrder(investor, activationLimit, amount, turn, number, validUntil);
    }

    @Override
    public String toString(){
        String res = "\n################################################\n\n";
        res += "Welcome to the exchange!\n\nThere are " + stocks.length + " instruments available:\n\n";
        for(int i = 0; i < stocks.length; ++i){
            res += "#" + (i + 1) + " " + stocks[i].getTicker() + " : $" + stocks[i].getLastPrice() + "\n";
        }
        res += "\nThere are " + investors.length + " investors participating in the trading:\n";
        for(int i = 0; i < investors.length; ++i){
            res += "\n#" + (i + 1) + " (" + investors[i].getName() + ") owning $" + investors[i].getPortfolio().getFunds() +
            " and following instruments:\n\n";
            res += investors[i].getPortfolio();
        }
        res += "\n################################################\n";
        return res;
    }
}
