public class Factorials {
    public static long calculator(int c){   
            long x;    
        if (c==1){
            return 1;} 
            
            x = calculator(c-1) * c;
            return x;
    }


    public static void main(String[] args){
        System.out.print(calculator(23));
    }
}