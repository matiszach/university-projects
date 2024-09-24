import java.util.HashMap;
import java.util.Scanner;
import java.util.Vector;

import exchanges.Exchange;
import instruments.Stock;
import investors.Investor;
import investors.RANDOM;
import investors.SMA;
import systems.Portfolio;
import utils.Pair;
import java.io.File;
import java.io.FileNotFoundException;

public class Main {
    public static void main(String[] args){
        Exchange GPW = new Exchange(args[0]);
        GPW.simulate(Integer.parseInt(args[1]));

        System.out.println(GPW);
    }
}