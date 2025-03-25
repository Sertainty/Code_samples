package org.Sertainty;

public class ByteArray {
    static {
        System.loadLibrary(Library.SertaintyCore());
    }

    public ByteArray() {
        handle = newHandle();
    }

    public ByteArray(long handle)
    {
        this.handle = handle;
    }

    public void setString(String data) {
        setString(handle, data);
    }

    public void appendString(String data) { appendString(handle, data);}

    public String getString() {
        return getString(handle);
    }

    public void setData(byte[] data)
    {
        setData(handle, data);
    }

    public void appendData(byte[] data)
    {
        appendData(handle, data);
    }

    public byte[] getData()
    {
        return getData(handle);
    }

    public void clearData() {
        clearData(handle);
    }

    public long getSize(){
        return getSize(handle);
    }

    public long getHandle() {
        return handle;
    }

    @Override
    protected void finalize() throws Throwable {
        freeHandle(handle);
        super.finalize();
    }

    private native long newHandle();
    private native void freeHandle(long handle);
    private native void clearData(long handle);
    private native byte[] getData(long handle);
    private native void appendData(long handle, byte[] data);
    private native void setData(long handle, byte[] data);
    private native String getString(long handle);
    private native void appendString(long handle, String data);
    private native void setString(long handle, String data);
    private native long getSize(long handle);

    private long handle;
}
