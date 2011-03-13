import java.io.*;

class Jerly {
    static {
        System.loadLibrary("jerly");
    }

    private long cherly;  // Set by init

    private native void   init(long maxSize);
    private native void   destroy();
    private native void   put(String key, byte[] value, int ttl);
    private native byte[] get(String key);
    private native void   remove(String key);

    //--------------------------------------------------------------------------
    
    public         Jerly(long maxSize) { init(maxSize); }
    protected void finalize()          { destroy();     }    

    //--------------------------------------------------------------------------
    
    public synchronized void   put   (Object key, byte[] value, int ttl) {        put   (key.toString(), value, ttl); }
    public synchronized void   put   (Object key, byte[] value)          {        put   (key.toString(), value, 0);   }
    public synchronized byte[] get   (Object key)                        { return get   (key.toString());             }
    public synchronized void   remove(Object key)                        {        remove(key.toString());             }

    //--------------------------------------------------------------------------

    public synchronized void put(Object key, Serializable value, int ttl) {
        try {            
            ByteArrayOutputStream bos = new ByteArrayOutputStream();
            ObjectOutputStream    out = new ObjectOutputStream(bos);

            out.writeObject(value);
            out.close();
            byte[] bytes = bos.toByteArray();
            bos.close();

            put(key.toString(), bytes, ttl);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    public synchronized void put(Object key, Serializable value) { put(key, value, 0); }

    //--------------------------------------------------------------------------
    
    public synchronized <T> T getAs(Object key) {
        try {
            byte[] bytes = get(key.toString());
            if (bytes == null) return null;

            ByteArrayInputStream bis = new ByteArrayInputStream(bytes);
            ObjectInputStream    in  = new ObjectInputStream(bis);

            @SuppressWarnings("unchecked")
            T t = (T) (in.readObject());
            
            bis.close();
            in.close();

            return t;
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }
}
