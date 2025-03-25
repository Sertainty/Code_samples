package org.Sertainty;

public class VirtualFile {
    static {
        System.loadLibrary(Library.SertaintyCore());
    }

    public VirtualFile(long fileHandle, long vfHandle)
    {
        this.fileHandle = fileHandle;
        this.vfHandle = vfHandle;
    }

    public long read(ByteArray buffer, long mx)
    {
        return read(fileHandle, vfHandle, buffer.getHandle(), mx);
    }

    public void close()
    {
        close(fileHandle, vfHandle);
    }


    private native long read(long fileHandle, long vfHandle, long baBuffer, long mx);
    private native void close(long fileHandle, long vfHandle);

    private long fileHandle;
    private long vfHandle;


}
