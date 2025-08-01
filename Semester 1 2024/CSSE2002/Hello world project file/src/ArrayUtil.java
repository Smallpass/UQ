public class ArrayUtil {
    static int sum(int[] numbers) {
        int result = 0;
        for (int number: numbers){
            result += number;
        }
        return result;
    }
    static int max(int[] numbers){
        int result = Integer.MIN_VALUE;
        for (int number: numbers) {
            if (number > result){
                result = number;
            }
        }
        return result;
    }

    static int[] concat(int[] first, int[] second) {
        int[] result = new int[first.length + second.length];
        for (int i = 0; i < result.length; i++) {
            if (i < first.length) {
                result[i] = first[i];
            } else{
                result[i] = second[i - first.length];
            }
        }
        return result;
    }
}
