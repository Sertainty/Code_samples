package org.Sertainty;

public class VirtualFileInfoList extends List {
    static {
        System.loadLibrary(Library.SertaintyCore());
    }

    public VirtualFileInfoList()
    {
        handle = newHandle();
    }

    public VirtualFileInfoList(long handle)
    {
        this.handle = handle;
    }

    public void add(VirtualFileInfo virtualFileInfo)
    {
        add(handle,virtualFileInfo.getHandle());
    }

    public VirtualFileInfo get(int element)
    {
        return new VirtualFileInfo(get(handle,element));
    }

    private native long newHandle();
    private native void add(long listHandle, long challengeHandle);
    private native long get(long listHandle, int element);
}
