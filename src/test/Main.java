public class Main {
  public static void main(String[] args) {
    Jerly j = new Jerly(30);
 
    for (int i = 1; i < 10; i++) {
      String k = "" + i;
      j.put(k, k.getBytes());
    }

    for (int i = 1; i < 10; i++) {
      String k = "" + i;
      byte[] bytes = j.get(k);
      if (bytes != null) System.out.println(k + ": " + new String(bytes));
    }
  }
}
