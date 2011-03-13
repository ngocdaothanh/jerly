public class Main {
  private static int CACHE_SIZE = 502;
  private static int N          = 1000000;
  
  public static void main(String[] args) {
    Jerly j = new Jerly(CACHE_SIZE);

    long t1 = System.currentTimeMillis();

    System.out.println("Put"); 
    for (int i = 1; i < N; i++) {
      String k = "" + i;
      String v = k;
      j.put(k, v);
    }

    long t2 = System.currentTimeMillis();
    System.out.println((t2 - t1) + " [ms]");
    
    System.out.println("Get");
    for (int i = 1; i < N; i++) {
      String k = "" + i;
      String v = j.getAs(k);

      if (v != null) {
        String s = new String(v);
        if (!s.equals(k)) System.out.println(k + ": " + s);
      }
    }
    
    long t3 = System.currentTimeMillis();
    System.out.println((t3 - t2) + " [ms]");
  }
}
