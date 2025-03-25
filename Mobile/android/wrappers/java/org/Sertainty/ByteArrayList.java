package org.Sertainty;

public class ByteArrayList extends List {
    static {
        System.loadLibrary(Library.SertaintyCore());
    }

    public ByteArrayList()
    {
        handle = newHandle();
    }

    public ByteArrayList(long handle)
    {
        this.handle = handle;
    }

    public void add(ByteArray data)
    {
        add(handle,data.getHandle());
    }

    public ByteArray get(int element) { return new ByteArray(get(handle,element));}

    private native long newHandle();
    private native void add(long listHandle, long buffer);
    private native long get(long listHandle, int element);
}
