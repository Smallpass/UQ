public class FibonacciString {
    static String fibString(int n) {
        if (n==1){
            return "a";
        }
        if (n==2) {
            return "b";
        }
        return fibString(n-1) + fibString(n-2);
    }
    static void main(String[] args) {
        System.out.println(fibString(3));
    }
}
