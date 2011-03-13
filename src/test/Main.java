public class Main {
  private static int CACHE_SIZE = 502;
  private static int N          = 1000000;
  
  public static void main(String[] args) throws Exception {
    testTTL();
    //benchmark();
  }
  
  private static void testTTL() throws Exception {
    Jerly j = new Jerly(CACHE_SIZE*100);

    j.put("1s", "value", 1);
    Thread.sleep(2000);
    System.out.println(j.get("1s"));

    j.put("2s", "value".getBytes(), 2);
    Thread.sleep(1000);
    System.out.println(new String(j.get("2s")));

    j.put("3s", "value", 3);
    Thread.sleep(1000);
    System.out.println(j.<String>getAs("3s"));
  }
  
  private static void benchmark() {
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

      if (v != null) System.out.println(k + ": " + v);
    }
    
    long t3 = System.currentTimeMillis();
    System.out.println((t3 - t2) + " [ms]");
  }
}
