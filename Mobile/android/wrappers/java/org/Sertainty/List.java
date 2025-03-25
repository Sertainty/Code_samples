package org.Sertainty;

public abstract class List {
    static {
        System.loadLibrary(Library.SertaintyCore());
    }

    public List(){}

    public void clear()
    {
        clear(handle);
    }

    public int count()
    {
        return count(handle);
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        freeList(handle);
    }

    protected long handle;

    private native void freeList(long listHandle);
    private native void clear(long listHandle);
    private native int count(long listHandle);
}
