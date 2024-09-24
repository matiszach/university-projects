package exchanges;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

class ExchangeTest {

    private void test(Exchange exchange){
        int initCash = exchange.totalCash();
        int[] initAmounts = new int[exchange.stocksAmount()];

        for(int i = 0; i < exchange.stocksAmount(); ++i){
            initAmounts[i] = exchange.totalStockAmount(exchange.stockAt(i));
        }

        exchange.simulate(10000);
        assertEquals(initCash, exchange.totalCash());

        for(int i = 0; i < exchange.stocksAmount(); ++i){
            assertEquals(initAmounts[i], exchange.totalStockAmount(exchange.stockAt(i)));
        }
    }

    @Test
    void test1() {
        Exchange exchange = new Exchange("tests/test1");
        test(exchange);
    }

    @Test
    void test2() {
        Exchange exchange = new Exchange("tests/test2");
        test(exchange);
    }

    @Test
    void test3() {
        Exchange exchange = new Exchange("tests/test3");
        test(exchange);
    }
}