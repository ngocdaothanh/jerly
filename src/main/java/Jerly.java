class Jerly {
    native void   init(long maxSize);
    native void   put(String key, byte[] value);
    native byte[] get(String key);
    native void   remove(String key);
    native void   destroy();

    static {
        System.loadLibrary("jerly");
    }
}
