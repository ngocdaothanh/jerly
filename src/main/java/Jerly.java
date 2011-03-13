import java.io.*;

class Jerly {
    static {
        System.loadLibrary("jerly");
    }

    protected long cherly;  // Set by init

    protected native void   init(long maxSize);
    protected native void   destroy();
    protected native void   put(String key, byte[] value);
    protected native byte[] get(String key);
    protected native void   remove(String key);

    //--------------------------------------------------------------------------
    
    public         Jerly(long maxSize) { init(maxSize); }
    protected void finalize()          { destroy();     }    

    //--------------------------------------------------------------------------
    
    public synchronized void   put   (Object key, byte[] value) {        put   (key.toString(), value); }
    public synchronized byte[] get   (Object key)               { return get   (key.toString());        }
    public synchronized void   remove(Object key)               {        remove(key.toString());        }

    //--------------------------------------------------------------------------
    
    public synchronized void put(Object key, Serializable value) {
        try {
            ByteArrayOutputStream bos = new ByteArrayOutputStream();
            ObjectOutputStream    out = new ObjectOutputStream(bos);
            out.writeObject(value);
            out.close();

            byte[] bytes = bos.toByteArray();
            put(key, bytes);
            bos.close();
        } catch (Exception e) {
        }
    }
    
    public synchronized <T> T getAs(Object key) {
        try {
            byte[] bytes = get(key);
            ByteArrayInputStream bis = new ByteArrayInputStream(bytes);
            ObjectInputStream in = new ObjectInputStream(bis);

            @SuppressWarnings("unchecked")
            T t = (T) in.readObject();

            in.close();
            bis.close();
            return t;
        } catch (Exception e) {
            return null;
        }
    }
}
