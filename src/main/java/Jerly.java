import java.io.*;

class Jerly {
    static {
        System.loadLibrary("jerly");
    }

    private long cherly;  // Set by init

    private native void   init(long maxSize);
    private native void   destroy();
    private native void   put(String key, byte[] value);
    private native byte[] get(String key);
    private native void   remove(String key);

    //--------------------------------------------------------------------------
    // Avoid having to create these objects all the time

    private ByteArrayOutputStream bos = new ByteArrayOutputStream();
    private ObjectOutputStream    out;

    private ByteArrayInputStream bis = new ByteArrayInputStream(new byte[0]);
    private ObjectInputStream    in;

    //--------------------------------------------------------------------------
    
    public Jerly(long maxSize) {
        init(maxSize);
        
        try { out = new ObjectOutputStream(bos); } catch (Exception e) {}
        try { in  = new ObjectInputStream(bis);  } catch (Exception e) {}
    }

    protected void finalize()          { destroy();     }    

    //--------------------------------------------------------------------------
    
    public synchronized void   put   (Object key, byte[] value) {        put   (key.toString(), value); }
    public synchronized byte[] get   (Object key)               { return get   (key.toString());        }
    public synchronized void   remove(Object key)               {        remove(key.toString());        }

    //--------------------------------------------------------------------------
    
    public synchronized void put(Object key, Serializable value) {
        try {            
            out.writeObject(value);
            byte[] bytes = bos.toByteArray();
            put(key, bytes);

            bos.reset();
            out.reset();
        } catch (Exception e) {
        }
    }

    //--------------------------------------------------------------------------
    
    public synchronized <T> T getAs(Object key) {
        try {          
            byte[] bytes = get(key);
            bis.read(bytes);
            
            @SuppressWarnings("unchecked")
            T t = (T) in.readObject();

            bis.reset();
            in.reset();

            return t;
        } catch (Exception e) {
            return null;
        }
    }
}
