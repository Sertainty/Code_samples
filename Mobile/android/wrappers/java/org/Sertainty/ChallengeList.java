package org.Sertainty;

public class ChallengeList extends List {
    static {
        System.loadLibrary(Library.SertaintyCore());
    }

    public ChallengeList()
    {
        handle = newHandle();
    }

    public ChallengeList(long handle)
    {
        this.handle = handle;
    }

    public void add(Challenge challenge)
    {
        add(handle,challenge.getHandle());
    }

    public Challenge get(int element)
    {
        return new Challenge(get(handle,element));
    }

    private native long newHandle();
    private native void add(long listHandle, long challengeHandle);
    private native long get(long listHandle, int element);

}
