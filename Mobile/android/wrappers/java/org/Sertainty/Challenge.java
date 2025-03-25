package org.Sertainty;

public class Challenge {
    static {
        System.loadLibrary(Library.SertaintyCore());
    }

    public Challenge()
    {
        this.handle = newHandle();
    }

    public Challenge(long handle)
    {
        this.handle = handle;
    }

    public void setPrompt(String prompt)
    {
        setPrompt(handle,prompt);
    }

    public String getPrompt()
    {
        return getPrompt(handle);
    }

    public void setName(String name)
    {
        setName(handle,name);
    }

    public String getName()
    {
        return getName(handle);
    }

    public void startTimer()
    {
        startTimer(handle);
    }

    public void endTimer()
    {
        endTimer(handle);
    }

    public void setValue(String value)
    {
        setValueString(handle, value);
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
    private native void freeHandle(long challengeHandle);
    private native String getPrompt(long challengeHandle);
    private native void startTimer(long challengeHandle);
    private native void endTimer(long challengeHandle);
    private native void setValueString(long challengeHandle, String value);
    private native void setPrompt(long challengeHandle, String prompt);
    private native void setName(long challengeHandle, String name);
    private native String getName(long challengeHandle);
}
