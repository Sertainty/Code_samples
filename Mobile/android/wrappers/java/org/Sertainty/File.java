package org.Sertainty;

public class File {
    static {
        System.loadLibrary(Library.SertaintyCore());
    }

    public static final int StatusAuthorized = 0x00010;
    public static final int StatusNotAuthorized = 0x00001;
    public static final int StatusChallenged = 0x00200;

    private File()
    {
        this.handle = newHandle();
    }

    public File(long handle)
    {
        this.handle = handle;
    }

    public static File create(String fileName, String governance, int govType, int mods, int flags)
    {
        long handle = newHandle();
        openNewFile(handle,fileName,governance,govType,mods,flags);
        return new File(handle);
    }

    public void addVirtualFile(String vfName, String fileSpec, long pageSize, long cacheSize, int mods)
    {
        addVirtualFile(handle, vfName, fileSpec, pageSize, cacheSize, mods);
    }

    public VirtualFile openVirtualFile(String vfName, int mode)
    {
        return new VirtualFile(handle, openVirtualFile(handle, vfName, mode));
    }

    public boolean compareExternalFile(String vfName, String fileName)
    {
        return compareExternalFile(handle, vfName, fileName);
    }

    public void close()
    {
        close(handle);
        freeHandle(handle);
    }

    public static File open(String source, int mode)
    {
        long handle = newHandle();
        open(handle,source,mode);
        return new File(handle);
    }

    public int authenticate()
    {
        status = authenticate(handle, false);
        return status;
    }

    public int getChallengeCount()
    {
        return getChallengeCount(handle);
    }

    public Challenge getChallenge(int offset)
    {
        return new Challenge(getChallenge(handle,offset));
    }

    public void addResponse(Challenge challenge)
    {
        addResponse(handle, challenge.getHandle());
    }

    public boolean isOpen()
    {
        return isOpen(handle, true);
    }

    public boolean isReadOnly()
    {
        return isReadOnly(handle, true);
    }

    public int getStatus() {
        return status;
    }

    public boolean isAuthorized()
    {
        return status == StatusAuthorized;
    }

    public VirtualFileInfo getVirtualFileInfo(String name)
    {
        return new VirtualFileInfo(getVirtualFileInfo(handle,name));
    }

    public VirtualFileInfoList getVirtualDirectory(String name, int mods)
    {
        return new VirtualFileInfoList(getVirtualDirectory(handle,name,mods));
    }

    public ByteArray readAllVirtualFile(String name)
    {
        ByteArray outbuf = new ByteArray();
        readAllVirtualFile(handle,name,outbuf.getHandle());
        return outbuf;
    }

    public String name()
    {
        return getName(handle);
    }

    public String fileSpecification()
    {
        return getFileSpecification(handle);
    }

    public Sys.CallStatus getCallStatus()
    {
        return new Sys.CallStatus(handle);
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        freeHandle(handle);
    }

    private native static long newHandle();
    private native static void openNewFile(long fileHandle, String fileName, String governance,
                                           int govType, int mods, int flags);
    private native void addVirtualFile(long fileHandle, String vfName, String fileSpec,
                                       long pageSize, long cacheSize, int mods);
    private native long openVirtualFile(long fileHandle, String vfName, int mode);
    private native boolean compareExternalFile(long fileHandle, String vfName, String fileName);
    private native void close(long fileHandle);
    private native static void open(long fileHandle, String source, int mode);
    private native int authenticate(long fileHandle, boolean useSSO);
    private native int getChallengeCount(long fileHandle);
    private native long getChallenge(long fileHandle, int offset);
    private native void addResponse(long fileHandle, long challengeHandle);
    private native void freeHandle(long fileHandle);
    private native boolean isOpen(long fileHandle, boolean bumpTimer);
    private native boolean isReadOnly(long fileHandle, boolean bumpTimer);
    private native long getChallenges(long fileHandle);
    private native void setResponses(long fileHandle, long responseList);
    private native long getVirtualFileInfo(long fileHandle, String name);
    private native long getVirtualDirectory(long fileHandle, String name, int mods);
    private native void readAllVirtualFile(long fileHandle, String name, long outbuf);
    private native String getName(long fileHandle);
    private native String getFileSpecification(long fileHandle);
    private long handle;



    private int status;
}
