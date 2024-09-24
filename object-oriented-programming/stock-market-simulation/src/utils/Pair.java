package utils;

public class Pair<T1, T2> {
    // data
    private T1 first;
    private T2 second;
    // constructor
    public Pair(T1 first, T2 second){
        this.first = first;
        this.second = second;
    }
    // methods
    public T1 getFirst() {
        return first;
    }
    public T2 getSecond(){
        return second;
    }
}
