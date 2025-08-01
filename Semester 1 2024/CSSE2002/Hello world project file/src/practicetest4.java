import java.util.Arrays;

public class practicetest4 {

    static int[] d(int n, int b) {
        if (n < b) {
            int[] r = new int[]{n};
            return r;
        }
        int[] rs = d(n / b, b);
        int[] r = new int[rs.length + 1];
        r[0] = n % b;
        for (int i = 0; i < rs.length; i++) {
            r[i + 1] = rs[i];
        }
        return r;
    }

    public static void main(String[] args) {
        int n = 1234;
        int b = 10;
        int[] result = d(n, b);
        System.out.println(Arrays.toString(result));
    }

}
