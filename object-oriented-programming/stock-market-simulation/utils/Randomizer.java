package utils;

import java.util.Random;

public class Randomizer {
    // data
    private static final Random rand = new Random();
    // constructor
    public Randomizer(){
        
    }
    // methods
    public static int randRange(int first, int last){
        return rand.nextInt(last - first + 1) + first;
    }
}
