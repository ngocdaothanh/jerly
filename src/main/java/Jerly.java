class Jerly {
    static {
        System.loadLibrary("jerly");
    }

    protected long cherly;  // Set by init

    public Jerly(long maxSize) {
        init(maxSize);
    }
    
    protected void finalize() {
        destroy();
    }
  
    protected native void   init(long maxSize);
    protected native void   destroy();
              native void   put(String key, byte[] value);
              native byte[] get(String key);
              native void   remove(String key);
}
