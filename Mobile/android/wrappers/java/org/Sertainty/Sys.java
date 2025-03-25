package org.Sertainty;

public class Sys {
    static {
        System.loadLibrary(Library.SertaintyCore());
    }

    public static class CallStatus{
        public CallStatus(){
            handle = newCallStatusHandle();
        }

        public CallStatus(long handle)
        {
            this.handle = handle;
            this.derived = true;
        }

        public boolean hasError()
        {
            return Sys.hasError(handle);
        }

        public String getErrorMessage()
        {
            return Sys.getErrorMessage(handle);
        }

        public long getHandle()
        {
            return handle;
        }



        @Override
        protected void finalize() throws Throwable {
            super.finalize();
            if(!derived) {
                freeCallStatusHandle(handle);
            }
        }

        private long handle;
        private boolean derived = false;
    }

    public static boolean initLibrary(ByteArray errBuffer, String appName, String prefix, String version)
    {
        return initLibrary(errBuffer.getHandle(),appName,prefix,version);
    }

    public static boolean installLicense(ByteArray errBuffer, String licenseFile)
    {
        return installLicense(errBuffer.getHandle(),licenseFile);
    }

    public static boolean releaseLibrary(ByteArray errBuffer)
    {
        return releaseLibrary(errBuffer.getHandle());
    }

    public static void fileReadAll(CallStatus callStatus, String fileSpec, ByteArray outbuf)
    {
        fileReadAll(callStatus.getHandle(),fileSpec,outbuf.getHandle());
    }

    public static void fileWriteAll(CallStatus callStatus, String fileSpec, byte[] data, int mods)
    {
        fileWriteAll(callStatus.getHandle(),fileSpec,data,mods);
    }

    private native static long newCallStatusHandle();
    private native static void freeCallStatusHandle(long handle);
    public native static void setLogging(int level);
    public native static void setLogFile(String prefix, String version);
    private native static boolean initLibrary(long baerror, String appName, String prefix, String version);
    private native static boolean releaseLibrary(long baerror);
    private native static void fileReadAll(long callStatus, String fileSpec, long outbuf);
    private native static void fileWriteAll(long callStatus, String fileSpec, byte[] data, int mods);
    private native static boolean hasError(long handle);
    private native static String getErrorMessage(long handle);
    private native static boolean installLicense(long baerror, String licenseFile);
}
