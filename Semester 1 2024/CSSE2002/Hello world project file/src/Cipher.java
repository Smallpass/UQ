public class Cipher{
    static String cipher(String message){
        String result = "";
        for (int i =0; i<message.length(); i++){
            char next = (char) (((message.charAt(i)+3 - 'A')%26) +'A');
            result = result + next;
        }
    return result;
    }

    public static void main(String[] args){
        System.out.println(cipher("XYZ"));
    }

}