package org.Sertainty;

public class VirtualFileInfo {
    static {
        System.loadLibrary(Library.SertaintyCore());
    }

    public VirtualFileInfo()
    {
        handle = newHandle();
    }

    public VirtualFileInfo(long handle)
    {
        this.handle = handle;
    }

    public String name()
    {
        return getName(handle);
    }

    public String fullName()
    {
        return getFullName(handle);
    }

    public int permissions()
    {
        return getPermissions(handle);
    }

    public int type()
    {
        return getType(handle);
    }

    public String typeName()
    {
        return getTypeName(handle);
    }

    public String reality()
    {
        return getReality(handle);
    }

    public static String getTypeName(int type)
    {
        return getTypeNameStatic(type);
    }

    public VirtualFileInfoList members()
    {
        return new VirtualFileInfoList(getMembers(handle));
    }

    public long getHandle()
    {
        return handle;
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        freeHandle(handle);
    }

    private long handle;

    private native long newHandle();
    private native void freeHandle(long vfInfoHandle);
    private native String getName(long vfInfoHandle);
    private native String getFullName(long vInfoHandle);
    private native int getPermissions(long vInfoHandle);
    private native int getType(long vInfoHandle);
    private native String getTypeName(long vInfoHandle);
    private native String getReality(long vInfoHandle);
    private native static String getTypeNameStatic(int type);
    private native long getCreDate(long vInfoHandle);
    private native long getModDate(long vInfoHandle);
    private native long getVirtualCreDate(long vInfoHandle);
    private native long getVirtualModDate(long vInfoHandle);
    private native long getSize(long vInfoHandle);
    private native long getVirtualSize(long vInfoHandle);
    private native long getPageSize(long vInfoHandle);
    private native long getCacheSize(long vInfoHandle);
    private native String getFileName(long vInfoHandle);
    private native String getChecksum(long vInfoHandle);
    private native short getFlags(long vInfoHandle);
    private native long getMembers(long vInfoHandle);
    private native String getACL(long vInfoHandle);
}
