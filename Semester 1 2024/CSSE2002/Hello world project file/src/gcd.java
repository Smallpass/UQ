public class gcd {
    public static int gcdrecursion(int x, int y) {
//        if (y !=0) {
//            int temp = y;
//            y = x % y;
//            x = temp;
//            gcdrecursion(x, y);
//        }
//        return x;
//    }   

    while (y != 0) {
        int temp = y;
        y = x % y;
        x = temp;        
    }
    return x;
}

    public static void main(String[] args) {
        if (args.length != 2) {
            System.out.println("Please provide exactly two integers.");
            return;
        }
        
        try {
            int x = Integer.parseInt(args[0]);
            int y = Integer.parseInt(args[1]);
            int result = gcdrecursion(x, y);
            System.out.println("GCD of " + x + " and " + y + " is " + result);
        } catch (NumberFormatException e) {
            System.out.println("Both arguments must be valid integers.");
        }
    }
}
