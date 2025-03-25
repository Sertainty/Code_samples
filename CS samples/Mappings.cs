using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace Sertainty
{
  using static MappingsCApi;

  public interface IUxpServices
  {
    void CloseDatabase();
    void InitializeDatabase(string spec, ByteArray id, string username, string dbmodule, string dbparams);
    void OpenDatabase(string spec, int readonlyInt);
    void Destroy();
    ServiceSession OpenSession(string username);
    ByteArray WebService(ByteArray request, string callerIp);
    bool HasError { get; }
    string ErrorMessage { get; }
  }

  public class Delegate
  {
    /// <summary>
    /// Creates a new Delegate object with properties stored from GetDelegate() function.
    /// </summary>
    /// <param name="owner">User who owns the delegate.  If owner is not the current
    ///                     authenticated user, then the current user must have PRIV_ADMIN privileges.</param>
    /// <param name="name">Delegate name to to add.</param>
    /// <param name="description">Delegate description.</param>
    /// <param name="expiration">Expiration of subscription.  Zero indicates no expiration.</param>
    /// <param name="permit_offline">Permit offline validation. 1 or 0.</param>
    /// <param name="offline_duration">Number of hours since last server validation.  Must not be zero.</param>
    /// <param name="access_max">Maximum number of successful accesses for a subscriber.  A zero indicates no limit.</param>
    /// <param name="checksum">Checksum for delegate.</param>
    /// <param name="uxpid">Uxp ID for delegate selected.</param>
    public Delegate(string owner, string name, ByteArray description, DateTime expiration, bool permit_offline, int offline_duration,
                                  int access_max, ByteArray checksum, ByteArray uxpid)
    {
      Owner = owner;
      Name = name;
      Description = description;
      Expiration = expiration;
      PermitOffline = permit_offline;
      OfflineDuration = offline_duration;
      AccessMax = access_max;
      Checksum = checksum;
      Uxpid = uxpid;
    }

    public string Owner { get; private set; }
    public string Name { get; private set; }
    public ByteArray Description { get; private set; }
    public DateTime Expiration { get; private set; }
    public bool PermitOffline { get; private set; }
    public int OfflineDuration { get; private set; }
    public int AccessMax { get; private set; }
    public ByteArray Checksum { get; private set; }
    public ByteArray Uxpid { get; private set; }

    /// <summary>
    /// Returns delegate object properties to a string 
    /// </summary>
    /// <returns>String of properties</returns>
    public override string ToString()
    {
      return String.Format(
        @"Name: {0}
Owner: {1}
Description: {2}
Expiration: {3}
Permit Offline: {4}
Offline Duration: {5}
Access Max: {6}
Checksum: {7}",
        Name, Owner, Description, Expiration, PermitOffline,
        OfflineDuration, AccessMax, Checksum);
    }

  }

  public class Subscription
  {
    /// <summary>
    /// Creates a new Subscription object with properties stored from GetSubscription() function.
    /// </summary>
    /// <param name="owner">User who owns the delegate.  If owner is not the current
    ///                     authenticated user, then the current user must have PRIV_ADMIN privileges.</param>
    /// <param name="name">Delegate name</param>
    /// <param name="subscriber">User name to add to the delegate member list.</param>
    /// <param name="expiration">Expiration of subscription.  Zero indicates no expiration. A
    ///                          -1 indicates use delegate value.</param>
    /// <param name="permit_offline">Permit offline validation. A -1 indicates use delegate value.</param>
    /// <param name="offline_duration">Number of hours since last server validation.  Must not be zero. A
    ///                                -1 indicates use delegate value.</param>
    /// <param name="access_max">Maximum number of successful access for this subscriber.  A -1 indicates
    ///                          use delegate value.A zero indicates no limit.</param>
    /// <param name="access_count">Current number of successful access for this subscriber.</param>
    public Subscription(string owner, string name, string subscriber, DateTime expiration, bool permitOffline, int offlineDuration, int accessMax, int accessCount)
    {
      Owner = owner;
      DelegateName = name;
      Subscriber = subscriber;
      Expiration = expiration;
      PermitOffline = permitOffline;
      AccessMax = accessMax;
      AccessCount = accessCount;
    }
    public string Owner { get; private set; }
    public string DelegateName { get; private set; }
    public string Subscriber { get; private set; }
    public DateTime Expiration { get; private set; }
    public bool PermitOffline { get; private set; }
    public int OfflineDuration { get; private set; }
    public int AccessMax { get; private set; }
    public int AccessCount { get; private set; }

    /// <summary>
    /// Returns subscription object properties to a string 
    /// </summary>
    /// <returns>String of properties</returns>
    public override string ToString()
    {
      return String.Format(
        @"Delegate: {0}
Owner: {1}
Subscriber: {2}
Expiration: {3}
Permit Offline: {4}
Offline Duration: {5}
Access Max: {6}
Access Count: {7}",
        DelegateName, Owner, Subscriber, Expiration, PermitOffline,
        OfflineDuration, AccessMax, AccessCount);
    }

  }

  public class DelegateUser
  {
    /// <summary>
    /// Creates a new DelegateUser object with properties stored from GetUser() function
    /// </summary>
    /// <param name="username">User name for the new user. The user must exist in the required
    ///                        UXP ID objects.</param>
    /// <param name="formalname">User formal name.</param>
    /// <param name="description">Description of user.</param>
    /// <param name="email">Email address of user.</param>
    /// <param name="privileges">Privileges as a bitmask.  Possible values:
    ///                          @arg PRIV_NORMAL - No privileges.Standard user.
    ///                          @arg PRIV_ADMIN - User administrator.
    ///                          @arg PRIV_JOURNAL - Journal management.</param>
    /// <param name="app_data1">Optional application data that is passed back to the application
    ///                         during remote user validation.</param>
    /// <param name="app_data2">Optional application data that is passed back to the application
    ///                         during remote user validation.</param>
    public DelegateUser(string username, ByteArray formalName, ByteArray description, ByteArray email, int privileges, ByteArray appdata1, ByteArray appdata2)
    {
      Username = username;
      Formalname = formalName;
      Description = description;
      Email = email;
      Privileges = privileges;
      AppData1 = appdata1;
      AppData2 = appdata2;
    }

    public string Username { get; private set; }
    public ByteArray Formalname { get; private set; }
    public ByteArray Description { get; private set; }
    public ByteArray Email { get; private set; }
    public int Privileges { get; private set; }
    public ByteArray AppData1 { get; private set; }
    public ByteArray AppData2 { get; private set; }

    /// <summary>
    /// Returns delegate user object properties to a string 
    /// </summary>
    /// <returns>String of properties</returns>
    public override string ToString()
    {
      return String.Format(
        @"Username: {0}
Formalname: {1}
Description: {2}
Email: {3}
Privileges: {4}
AppData1: {5}
AppData2: {6}
", Username, Formalname, Description, Email, Privileges, AppData1, AppData2);
    }
  }

  public static class Sys
  {
    /// <summary>
    /// Determines if current handle has an error associated with it.
    /// </summary>
    /// <param name="handle">Call status handle.</param>
    /// <returns>True if error has occurred</returns>
    public static bool HasError(IntPtr handle)
    {
      return uxpsys_hasError(handle);
    }

    /// <summary>
    /// Gets the current error message for the handle
    /// </summary>
    /// <param name="handle">Call status handle.</param>
    /// <returns>Error message as null-terminated string</returns>
    public static string GetErrorMessage(IntPtr handle)
    {
      return uxpsys_getErrorMessage(handle);
    }

    /// <summary>
    /// Enables logging of execution information.  If the logging level
    /// is set to uxpLogNone, no logging will be set up.
    /// </summary>
    /// <param name="prefix">Prefix name used to construct the log file.</param>
    /// <param name="version">Application version</param>
    public static void SetLogFile(string prefix, string version)
    {
      uxpsys_setLogFile(prefix, version);
    }

    /// <summary>
    /// Reads the specified file into a buffer
    /// </summary>
    /// <param name="status">Call status</param>
    /// <param name="filespec">nput file specification</param>
    /// <param name="outbuf">Buffer to receive file contents</param>
    public static void FileReadAll(CallStatus status, string filespec, ByteArray outbuf)
    {
      uxpsys_fileReadAll(status, filespec, outbuf);
    }

    /// <summary>
    /// Writes a buffer to the specified file
    /// </summary>
    /// <param name="status">Call status</param>
    /// <param name="filespec">Output file specification</param>
    /// <param name="inbuf">Buffer to be written</param>
    /// <param name="modifiers">Operation modifiers.  Supported values are:
    ///     ModifierReplace: Replace existing output file</param>
    public static void FileWriteAll (CallStatus status, string filespec, ByteArray inbuf, Modifiers modifiers)
    {
      uxpsys_fileWriteAll (status, filespec, inbuf.GetDataPtr(), inbuf.Size, (long)modifiers);
    }

    /// <summary>
    /// Initializes the library.  This call must be made prior to any UXP
    /// operations.If the call returns a zero, then an error message
    ///  will be returned in the buffer.
    /// </summary>
    /// <param name="buffer">Buffer to receive any error messages</param>
    /// <param name="argc">Number of arguments on main command line</param>
    /// <param name="argv">Array of main command line arguments</param>
    /// <param name="licenseFile">License file name.  If the license file name is an asterisk,
    ///                           the system will search for sertainty.lic in the Sertainty home
    ///                           folder.If the file name is a complete specification, including
    ///                           the full volume specification, the system will attempt to find
    ///                           the license using that specification.If the license name is a
    ///                           relative path and name, then it will prepend the Sertainty home
    ///                           folder and attempt to search for the license.</param>
    /// <param name="appKey">Key required to load license.</param>
    /// <param name="prefix">Prefix name used to construct the log file.</param>
    /// <param name="version">Version of the application</param>
    /// <returns>Status of call: 1 = success, 0 = failure</returns>
    public static long InitializeLibrary(ByteArray buffer,long argc, string[] argv, string licenseFile, string appKey, string prefix, string version)
    {
      return uxpsys_initLibrary(buffer, argc, argv, licenseFile, appKey, prefix, version);
    }

    /// <summary>
    /// Converts Bytes to a character pointer for internal functions
    /// </summary>
    /// <param name="data">Data to be converted.</param>
    /// <returns>Character pointer with converted data.</returns>
    public static IntPtr BytesToCharPtr(byte[] data)
    {
      int size = Marshal.SizeOf(data[0]) * data.Length;
      IntPtr ptr = Marshal.AllocHGlobal(size);

      Marshal.Copy(data, 0, ptr, data.Length);
      
      return ptr;
    }

    /// <summary>
    /// Converts character pointer to Bytes for internal functions
    /// </summary>
    /// <param name="ptr">Character pointer that has data to be converted.</param>
    /// <param name="size">Size of character pointer.</param>
    /// <returns>Data as Bytes</returns>
    public static byte[] BytesFromCharPtr(IntPtr ptr, int size)
    {
      byte[] data = new byte[size];

      Marshal.Copy(ptr, data, 0, size);

      return data;
    }
    
    public class CallStatus
    {

      public CallStatus()
      {
        _handle = uxpsys_newCallStatusHandle();
      }

      
      public bool HasError
      {
        get
        {
          return Sys.HasError(_handle);
        }
      }
      
      public string ErrorMessage
      {
        get
        {
          return Sys.GetErrorMessage(_handle);
        }
      }
      
      public static implicit operator IntPtr(CallStatus cs)
      {
        return cs._handle;
      }
      
      ~CallStatus()
      {
        uxpsys_freeCallStatusHandle(_handle);
      }
      
      private IntPtr _handle;
    }
  }

  public class ByteArray
  {
    /// <summary>
    /// Creates a new byte array from pointer.
    /// </summary>
    /// <param name="handle">Pointer to data.</param>
    /// <returns></returns>
    public static ByteArray fromHandle(IntPtr handle)
    {
      return new ByteArray(handle);
    }

    /// <summary>
    /// Creates a new byte array
    /// </summary>
    public ByteArray()
    {
      _handle = uxpba_newHandle();
    }
    
    private ByteArray(IntPtr srcHandle)
    {
      _handle = srcHandle;
    }

    /// <summary>
    /// Gets the length of the data in the byte array.
    /// </summary>
    /// <returns>Length of data in bytes.</returns>
    public long Size
    {
      get
      {
        return uxpba_getSize(_handle);
      }
    }

    /// <summary>
    /// Erases all elements from the array. After this call, the array size is zero.
    /// </summary>
    public void Clear()
    {
      uxpba_clearData(_handle);
    }

    /// <summary>
    /// Gets the current data from the byte array and returns it in an array of bytes.
    /// </summary>
    /// <param name="len">Length of array</param>
    /// <returns>An array of bytes with the data from the ByteArray</returns>
    public byte[] GetBytes(int len)
    {
      IntPtr ptr = uxpba_getData(_handle);
      if (ptr == IntPtr.Zero)
      return new byte[0];
      if (len == 0)
      return new byte[0];
      byte[] array = new byte[len];
      Marshal.Copy(ptr, array, 0, len);
      return array;
    }

    /// <summary>
    /// Gets all the contents of the byte array and returns it in an array of bytes. 
    /// </summary>
    /// <returns>An array of bytes with the data from the ByteArray</returns>
    public byte[] GetAllBytes()
    {
      return GetBytes((int)this.Size);
    }

    /// <summary>
    /// Gets the pointer to the current data from the byte array.
    /// </summary>
    /// <returns>Pointer to data.  This pointer remains valid until the buffer
    ///          is either released or modified.For printable data, the buffer
    ///          is zero terminated.  For binary data, one must get the size
    ///          from the uxpba_getSize function.</returns>
    public IntPtr GetDataPtr ()
    {
      return uxpba_getData (_handle);
    }

    /// <summary>
    /// Copies the specified data to the array.
    /// </summary>
    /// <param name="data">Source array of data.</param>
    public void SetData(string data)
    {
      uxpba_setData(_handle, data, data.Length);
    }

    /// <summary>
    /// Gets the current data from the byte array and returns it as a string.
    /// </summary>
    /// <returns>A String with the data from the ByteArray.</returns>
    public override string ToString()
    {
      IntPtr ptr = uxpba_getData(_handle);
      if (ptr == IntPtr.Zero)
      return "";
      int len = 0;
      while (Marshal.ReadByte(ptr, len) != 0)
      len++;
      if (len == 0)
      return "";
      byte[] array = new byte[len];
      Marshal.Copy(ptr, array, 0, len);
      
      return Encoding.UTF8.GetString(array);
    }

    /// <summary>
    /// Returns internal handle for byte array 
    /// </summary>
    /// <param name="byteArray">ByteArray to to be used.</param>
    public static implicit operator IntPtr(ByteArray byteArray)
    {
      return byteArray._handle;
    }

    /// <summary>
    /// ByteArray class deconstructor
    /// </summary>
    ~ByteArray()
    {
      uxpba_freeHandle(_handle);
    }
    
    private IntPtr _handle;
  }

  public class Variable
  {
    /// <summary>
    /// Creates a new variable from a pointer.
    /// </summary>
    /// <param name="handle">Pointer to data.</param>
    /// <returns></returns>
    public static Variable fromHandle(IntPtr handle)
    {
      return new Variable(handle);
    }

    /// <summary>
    /// Internal variable pointer 
    /// </summary>
    /// <param name="handle">Pointer to copy</param>
    private Variable(IntPtr handle)
    {
      _handle = handle;
    }

    /// <summary>
    /// Gets the variable name.
    /// </summary>
    public string Name
    {
      get
      {
        ByteArray buffer = new ByteArray();
        uxpvar_getName(_handle, buffer);
        return buffer.ToString();
      }
    }

    /// <summary>
    /// Returns variable as a string 
    /// </summary>
    /// <returns>String containing variable data</returns>
    public override string ToString()
    {
      ByteArray buffer = new ByteArray();
      uxpvar_toString(_handle, buffer);
      return buffer.ToString();
    }

    /// <summary>
    /// Returns internal handle for variable for internal functions. 
    /// </summary>
    /// <param name="variable">Variable to be used.</param>
    public static implicit operator IntPtr(Variable variable)
    {
      return variable._handle;
    }

    private IntPtr _handle;
  }

  public abstract class HasCallStatus
  {
    /// <summary>
    /// Determines if current handle has an error associated with it.
    /// </summary>
    /// <returns>True if error has occurred.</returns>
    public bool HasError
    {
      get
      {
        return Sys.HasError(_handle);
      }
    }

    /// <summary>
    /// Gets the current error message for the object.
    /// </summary>
    /// <returns>String containing error message.</returns>
    public string ErrorMessage
    {
      get
      {
        return Sys.GetErrorMessage(_handle);
      }
    }


    protected IntPtr _handle;
  }

  public abstract class List<T> : IEnumerable<T>
  {
    /// <summary>
    /// Returns the number of elements in the list.
    /// </summary>
    public int Count
    {
      get
      {
        return uxplist_count(_handle);
      }
    }

    /// <summary>
    /// Clears a list.
    /// </summary>
    public void Clear()
    {
      uxplist_clear(_handle);
    }

    /// <summary>
    /// Gets the Enumerator for object in the List.
    /// </summary>
    /// <returns>Item in list</returns>
    public IEnumerator<T> GetEnumerator()
    {
      for(int i = 0; i < Count; i++)
      {
        yield return GetItem(i);
      }
    }

    /// <summary>
    /// Gets the Enumerator for object in the List.
    /// </summary>
    /// <returns>Enumerator item in list</returns>
    IEnumerator IEnumerable.GetEnumerator()
    {
      return GetEnumerator();
    }

    /// <summary>
    /// Gets Item from List object.
    /// </summary>
    /// <param name="index">Element number.  Zero is the first element.</param>
    /// <returns>Item from list object with properties.</returns>
    public T GetItem(int index)
    {
      if(index >= Count || index < 0)
      {
        throw new ArgumentOutOfRangeException();
      }

      Type listType = typeof(T);
      IntPtr ptr;
      object item;


      if (listType == typeof (Challenge))
      {
        ptr = uxplist_getChallenge(_handle, index);
        item = Challenge.fromHandle(ptr);

      }
      else if(listType == typeof(Variable))
      {
        ptr = uxplist_getVariable(_handle, index);
        item = Variable.fromHandle(ptr);
      }
      else if (listType == typeof (ByteArray))
      {
        ptr = uxplist_getByteArray(_handle, index);
        item = ByteArray.fromHandle(ptr);
      }
      else if (listType == typeof (VirtualFileInfo))
      {
        ptr = uxplist_getVirtualFile(_handle, index);
        item = VirtualFileInfo.fromHandle(ptr);
      }
      else
      {
        throw new NotImplementedException();
      }

      if (item is T)
      {
        return (T)item;
      }

      return (T)Convert.ChangeType(item, typeof(T));
      
    }

    /// <summary>
    /// Adds Item object to the current List object.
    /// </summary>
    /// <param name="item">Item to be added.</param>
    public void AddItem(T item)
    {
      Type listType = typeof(T);

      if (listType == typeof(Challenge))
      {
        uxplist_addChallenge(_handle, (item as Challenge));
      }
      else if (listType == typeof(Variable))
      {
        uxplist_addVariable(_handle, (item as Variable));
      }
      else if (listType == typeof(ByteArray))
      {
        uxplist_addByteArray(_handle, (item as ByteArray));
      }
      else
      {
        throw new NotImplementedException();
      }
    }

    /// <summary>
    /// List deconstructor
    /// </summary>
    ~List()
    {
      uxplist_freeList(_handle);
    }

    /// <summary>
    /// Returns internal handle for list object
    /// </summary>
    /// <param name="list">List to be used.</param>
    public static implicit operator IntPtr(List<T> list)
    {
      return list._handle;
    }

    protected IntPtr _handle;
    
  }

  public class UxpFile : HasCallStatus
  {
    /// <summary>
    /// Creates a new UXP handle.  A handle is not associated with
    /// an actual UXP until a create or open operation is performed.
    /// </summary>
    public UxpFile()
    {
      _handle = uxpfile_newHandle();
    }

    /// <summary>
    /// Creates a new UXP as a file.  If an existing UXP
    /// is open, it will be closed prior to opening the specified object.
    /// </summary>
    /// <param name="data">File name for the new UXP.</param>
    /// <param name="governance">Identity file used for authentication and governance control.</param>
    /// <param name="govtype">Specifies the identity code type.
    ///                       Possible values are:
    ///                       @arg<b> KclFile</b> File containing source or compiled kcl.
    ///                       @arg<b> IdFile</b> File containing a compiled UXP ID.</param>
    /// <param name="mods">Operation modifiers.
    ///                    Possible values:
    ///                    @arg<b> ModifierCompress</b> Compress data before saving using ZLIB.
    ///                    @arg<b>ModifierRecurse</b> Adds folders recursively, if data is a folder.
    ///                    @arg<b> ModifierReadWrite</b> Sets the UXP architecture to ReadWrite.
    ///                    @arg<b> ModifierReplace</b> Replaces the target UXP, if it exists.
    ///                    @arg<b> ModifierWriteOnce</b> Sets the UXP architecture to WriteOnce.</param>
    /// <param name="flags">Data flags.
    ///                     Possible values:
    ///                     @arg<b> DataFlagNone</b> No flags are used.
    ///                     @arg<b> DataFlagAnon</b> Allows anonymous writes to occur against UXP.</param>
    /// @note  The following preferences may be utilized to set up the UXP:
    ///        @arg<b> PreferencePageSize</b> Sets the default page size for virtual files.
    ///        @arg<b>PreferenceBuffers</b> Sets the default cache size for virtual file streaming.
    ///        @arg<b> PreferenceDefaultID</b> The default UXP ID if one is not provided.
    ///        @arg<b> PreferenceDefaultDomain</b> Sets the default default domain for the UXP.
    ///        @arg<b> PreferenceUseReadWriteArchitecture</b> Sets the default UXP architecture.  This
    ///                value will be used when an explicit modifier is not set.
    ///        @arg<b> PreferenceCompanyName</b> Sets the company name attribute in the UXP.
    ///        @arg<b> PreferenceCompanyWebPage</b> Sets the company web page attribute in the UXP. 
    public void OpenNewFile(string data, string governance, long govtype, long mods, long flags)
    {
      uxpfile_openNewFile(_handle, data, governance, govtype, mods, flags);
    }

    /// <summary>
    /// Adds a user data file to the UXP file system.
    /// </summary>
    /// <param name="virName">Name of the data.  Must be unique relative to the UXP.</param>
    /// <param name="filespec">Name of the file to be imported into the UXP.</param>
    /// <param name="pageSize">Sets the page size for the virtual file.  A value of -1
    ///                        lets the system set the size.Page size is used to optimize
    ///                        storage for typical access.</param>
    /// <param name="cacheSize">Sets the internal cache size.  The size is the number
    ///                         in-memory pages to maintain for frequently accessed data.
    ///                         A value of -1 lets the system set the size.A zero disables
    ///                         the cache.  Caching improves performance for streamed data.</param>
    /// <param name="mods">Operation modifiers as bit values.
    ///                    Support values are:
    ///                    @arg<b> ModifierRecurse</b> Recursively process input directories.
    ///                    @arg<b> ModifierCompress</b> Compress virtual files using ZLIB.
    ///                    @arg<b>ModifierDeleteSrc</b> Delete the source after successful import.</param>
    public void AddVirtualFile(string virName, string filespec, long pageSize, long cacheSize, long mods)
    {
      uxpfile_addVirtualFromFile(_handle, virName, filespec, pageSize, cacheSize, mods);
    }

    /// <summary>
    /// Replaces an existing virtual file with an external, updated copy.
    /// </summary>
    /// <param name="virName">Virtual file name of existing object.</param>
    /// <param name="filespec">Data file specification of updated file.</param>
    /// <param name="outname">New virtual file name.</param>
    public void EditVirtualFile(string virName, string filespec, ByteArray outname)
    {
      uxpfile_editVirtualFile(_handle, virName, outname, filespec);
    }

    /// <summary>
    /// Open a virtual file for direct access.  Data can be accessed
    /// randomly using seek and read routines.As with standard file
    /// access, zero is the start of the data.The read access pointer is set
    /// to zero by default.
    /// </summary>
    /// <param name="fileSpec">Virtual file name.</param>
    /// <param name="mode">Open mode.
    ///                    Supported values:
    ///                    @arg<b> ModeRead</b> Opens the file for read only.
    ///                    @arg<b> ModeReadWrite</b> Opens the file for read and write.
    ///                    @arg <b>ModeWrite</b> Opens the file for write.</param>
    /// <returns>Virtual file</returns>
    public VirtualFile OpenVirtualFile(string fileSpec, Mode mode)
    {
      return new VirtualFile(this, fileSpec, mode);
    }

    /// <summary>
    /// Creates and opens a virtual file for write.
    /// </summary>
    /// <param name="vf_name">Virtual file name.</param>
    /// <param name="pageSize">Sets the page size for the virtual file.  A value of -1
    ///                        lets the system set the size.Page size is used to optimize
    ///                        storage for typical access.</param>
    /// <param name="cacheSize">Sets the internal cache size.  The size is the number
    ///                         in-memory pages to maintain for frequently accessed data.
    ///                         A value of -1 lets the system set the size.A zero disables
    ///                         the cache.  Caching improves performance for streamed data.</param>
    /// <param name="mods">Operation modifiers as bit values.
    ///                    Support values are:
    ///                    @arg<b> ModifierCompress</b> Compress data before saving using ZLIB.</param>
    /// <returns>Virtual file</returns>
    public VirtualFile NewVirtualFile(string vf_name, long pageSize, long cacheSize, long mods)
    {
      return new VirtualFile(this, uxpfile_newVirtualFile(_handle, vf_name, pageSize, cacheSize, mods));
    }

    /// <summary>
    /// Compares a virtual file to an external file.
    /// </summary>
    /// <param name="vf_name">Virtual file name.</param>
    /// <param name="ext_file_name">External file name.</param>
    /// <returns>True if identical.</returns>
    public bool CompareExternalFle(string vf_name, string ext_file_name)
    {
      return uxpfile_compareExternalFile(_handle, vf_name, ext_file_name);
    }

    /// <summary>
    /// Copies the virtual file from the UXP to an external file.
    /// </summary>
    /// <param name="vf_name">Virtual file name.</param>
    /// <param name="ext_file_name">Output file specification.</param>
    /// <param name="mods">Operation modifiers as bit values.
    ///                    Support values are:
    ///                    @arg<b> ModifierMerge</b>     Merge files into existing directories.
    ///                    @arg <b>ModifierRecurse</b> Recursively process directories.
    ///                    @arg<b> ModifierReplace</b>   Replace existing files.</param>
    public void ExportVirtualFile(string vf_name, string ext_file_name, long mods)
    {
      uxpfile_exportVirtualFile(_handle, vf_name, ext_file_name, mods);
    }

    /// <summary>
    /// Closes the current UXP.
    /// </summary>
    public void Close()
    {
      uxpfile_close(_handle);
    }

    /// <summary>
    /// Opens a UXP from a source file.  If an existing UXP
    /// is open, it will be closed prior to opening the specified object.
    /// </summary>
    /// <param name="source">Source file specification.</param>
    /// <param name="mode">Sets the access mode.  If the UXP resides in a read-only file system,
    ///                    or the physical UXP file is locked, then the mode will be
    ///                    forced to ShareReadOnly.
    ///                    Possible values:
    ///                    @arg<b> ShareNone</b> Opens UXP for exclusive access.
    ///                    @arg<b> ShareAll</b> Opens UXP for sharing with others.
    ///                    @arg <b>ShareReadOnly</b> Opens UXP read-only.
    ///                    @arg <b>ShareAnon</b> Opens UXP in anonymous mode.</param>
    /// @note If the UXP was created using an older version of the API,
    ///       the UXP will be upgraded.This will render it incompatible with
    ///       prior versions of the API.To avoid the upgrade, open using ShareReadOnly
    ///       or physically set the UXP as read-only.
    public void Open(string source, UxpMode mode)
    {
      uxpfile_openFile(_handle, source, mode);
    }

    /// <summary>
    /// Authenticates the user after the UXP has been opened.  If
    /// the call returns<b> StatusChallenged</b>, then the challenge list will contain
    /// a set of questions that must be answered by the user.If an optional
    /// callback function is provided, challenges will be sent to it; otherwise, the
    /// caller must process the challenge list directly.
    /// </summary>
    /// <param name="useSSO">True if attempting to use an active UXP ID single sign-on session to
    ///                      provide authentication challenge responses.SSO will not be used if
    ///                      the UXP requires multi-factor authentication. SSO is also unavailable
    ///                      on Android, iOS, and embedded systems.</param>
    /// <returns>Status of authentication.
    ///          Possible values:
    ///          @arg<b> StatusAuthorized</b> Access has been authorized.
    ///          @arg<b> StatusCanceled</b> The user has canceled the authorization attempt.
    ///          @arg <b>StatusChallenged</b> Access requires more challenges to be met.
    ///          @arg<b> StatusNotAuthorized</b> Access has been denied.</returns>
    public AuthorizationStatus Authenticate(bool useSSO)
    {
      return uxpfile_authenticate(_handle, useSSO);
    }

    /// <summary>
    /// Gets the challenge count from a UXP handle.  Challenges are
    /// constructed by the UXP during the authentication process.If authentication
    /// sets the status to<b> StatusChallenged</b>, then the user must respond appropriately
    /// to the challenges.
    /// </summary>
    public int ChallengeCount
    {
      get
      {
        return uxpfile_getChallengeCount(_handle);
      }
    }

    /// <summary>
    /// Gets the specified challenge number from the UXP handle.  Challenges are
    /// constructed by the UXP during the authentication process.If authentication
    /// sets the status to<b> StatusChallenged</b>, then the user must respond appropriately
    /// to the challenges.Challenges start at offset zero and end at challenge count minus 1.
    /// </summary>
    /// <param name="ch_idx">Challenge offset</param>
    /// <returns>Challenge Object</returns>
    public Challenge GetChallenge(int ch_idx)
    {
      IntPtr ch_handle = uxpfile_getChallenge(_handle, ch_idx);
      return Challenge.fromHandle(ch_handle);
    }

    /// <summary>
    /// Adds a response to the challenge response list.
    /// </summary>
    /// <param name="ch">Challenge object</param>
    public void AddResponse(Challenge ch)
    {
      uxpfile_addResponse(_handle, ch);
    }

    /// <summary>
    /// Gets the specified virtual file information.  Virtual
    /// files are discrete storage entities within a UXP.Not
    /// only is user data stored in virtual files, but other
    //  ancillary data is as well.
    /// </summary>
    /// <param name="name">Virtual file name.
    ///                    File info:
    ///    @arg ACL - Access control information for the file.
    ///    @arg CacheSize - Number of cache buffers.
    ///    @arg Checksum - Checksum of the file contents.
    ///    @arg CreDate - Date/time when file was added to UXP.
    ///    @arg ExtCreDate - Creation date/time of file when added to UXP.
    ///    @arg ExtFilename - Original file name of virtual file.
    ///    @arg ExtModDate - Last modification date when added to UXP.
    ///    @arg ExtSize - Size of external file when added to UXP.
    ///    @arg MemberCount - If a folder, the number of child elements.
    ///    @arg ModDate - Last modification date within UXP.
    ///    @arg Name - Virtual file name.
    ///    @arg PageSize - Page size used for tuning access.
    ///    @arg Size - Current size of virtual file in bytes.
    ///    @arg Type - Type of virtual file.
    ///    @arg TypeName - Translated virtual file type.</param>
    /// <returns>Virtual file info object</returns>
    public VirtualFileInfo GetVirtualFileInfo(string name)
    {
      return VirtualFileInfo.fromHandle(uxpfile_getVirtualFileInfo(_handle, name));
    }

    /// <summary>
    /// Gets a directory list of the UXP.
    /// </summary>
    /// <param name="path">Starting path of directory.  An empty string or a value of
    ///                    "/" will get the objects at the top most level in the
    ///                    virtual directory.</param>
    /// <param name="mods">Operation modifiers as bit values.  Support values:
    ///                    @arg<b> ModifierRecurseDir</b> Recursively process sub-directories.</param>
    /// <returns>Virtual file list object of directory entries.</returns>
    public VirtualFileInfoList GetVirtualDirectory(string path, long mods)
    {
      IntPtr listHandle = uxpfile_getVirtualDirectory(_handle, path, mods);

      if (HasError)
      {
        return VirtualFileInfoList.emptyList();
      }

      return VirtualFileInfoList.fromHandle(listHandle);
    }

    /// <summary>
    /// Returns internal handle for UxpFile for internal functions.
    /// </summary>
    /// <param name="uxp">Uxp to be used.</param>
    public static implicit operator IntPtr(UxpFile uxp)
    {
      return uxp._handle;
    }

    /// <summary>
    /// UxpFile class deconstructor.
    /// </summary>
    ~UxpFile()
    {
      uxpfile_freeHandle(_handle);
    }
  }

  public class Challenge
  {
    private Challenge(IntPtr ch_handle)
    {
      _handle = ch_handle;
    }

    /// <summary>
    /// Creates a new challenge object
    /// </summary>
    public Challenge ()
    {
      _handle = uxpch_newHandle ();
    }

    /// <summary>
    /// Creates a new challenge object from a challenge pointer
    /// </summary>
    /// <param name="ch_handle">Pointer to challenge handle</param>
    /// <returns></returns>
    public static Challenge fromHandle(IntPtr ch_handle)
    {
      return new Challenge(ch_handle);
    }

    /// <summary>
    /// Starts a timer for user input
    /// </summary>
    public void StartTimer()
    {
      uxpch_startTimer(_handle);
    }

    /// <summary>
    /// Stops a user-input timer
    /// </summary>
    public void EndTimer()
    {
      uxpch_endTimer(_handle);
    }

    /// <summary>
    /// Gets the prompt for the challenge.
    /// </summary>
    /// <returns>Challenge prompt.</returns>
    public ByteArray GetPrompt()
    {
      ByteArray prompt = new ByteArray();
      uxpch_getPrompt(_handle, prompt);
      return prompt;
    }

    /// <summary>
    /// Sets the name for the challenge.
    /// </summary>
    /// <param name="name">Challenge name.</param>
    public void SetName (string name)
    {
      uxpch_setName (_handle, name);
    }

    /// <summary>
    /// Sets the prompt for the challenge.  The prompt should be a printable
    /// set of characters.
    /// </summary>
    /// <param name="prompt">Challenge prompt.</param>
    public void SetPrompt(string prompt)
    {
      uxpch_setPrompt(_handle, prompt);
    }

    /// <summary>
    /// Sets the challenge value
    /// </summary>
    /// <param name="value">Challenge response value.</param>
    public void SetValue(string value)
    {
      uxpch_setValueString(_handle, value);
    }

    /// <summary>
    /// Returns internal handle for chellenge object for internal functions.
    /// </summary>
    /// <param name="ch">Challenge to be used.</param>
    public static implicit operator IntPtr(Challenge ch)
    {
      return ch._handle;
    }

    /// <summary>
    /// Chellenge class deconstructor.
    /// </summary>
    ~Challenge()
    {
      uxpch_freeHandle(_handle);
    }
    
    private IntPtr _handle;
  }

  public class ChallengeList : List<Challenge>
  {
    /// <summary>
    /// Creates a new UXP challenge list from a handle.
    /// </summary>
    /// <param name="handle">Handle to be used.</param>
    /// <returns></returns>
    public static ChallengeList fromHandle(IntPtr handle)
    {
      return new ChallengeList(handle);
    }

    /// <summary>
    /// Creates a new UXP challenge list.
    /// </summary>
    public ChallengeList ()
    {
      _handle = uxplist_newChallengeList();
    }

    private ChallengeList(IntPtr handle)
    {
      _handle = handle;
    }

    /// <summary>
    /// Adds a challenge object to the current list
    /// </summary>
    /// <param name="ch">Challenge object to add. Object contents are copied to a new object
    ///                 within the list.</param>
    public void AddChallenge (Challenge ch)
    {
      uxplist_addChallenge (this._handle, ch);
    }
  }

  public class VariableList : List<Variable>
  {
    /// <summary>
    /// Creates a new UXP variable list from a handle.
    /// </summary>
    /// <param name="handle">Handle to be used</param>
    /// <returns></returns>
    public static VariableList fromHandle(IntPtr handle)
    {
      return new VariableList(handle);
    }

    /// <summary>
    /// Creates a new UXP variable list.
    /// </summary>
    public VariableList ()
    {
      _handle = uxplist_newVariableList();
    }

    private VariableList(IntPtr handle)
    {
      _handle = handle;
    }

  }

  public class ByteArrayList : List<ByteArray>
  {
    /// <summary>
    /// Creates a new UXP byte array list from a handle
    /// </summary>
    /// <param name="handle">handle to be used</param>
    /// <returns>Byte array list object</returns>
    public static ByteArrayList fromHandle(IntPtr handle)
    {
      return new ByteArrayList(handle);
    }

    private ByteArrayList(IntPtr handle)
    {
      _handle = handle;
    }

    /// <summary>
    /// Creates a new UXP byte array list.
    /// </summary>
    public ByteArrayList()
    {
       _handle = uxplist_newByteArrayList();
    }
  }

  public class VirtualFileInfoList : List<VirtualFileInfo>
  {
    /// <summary>
    /// Creates a virtual file info list from a handle.
    /// </summary>
    /// <param name="handle">Handle to be used.</param>
    /// <returns></returns>
    public static VirtualFileInfoList fromHandle(IntPtr handle)
    {
      return new VirtualFileInfoList(handle);
    }

    private VirtualFileInfoList(IntPtr handle)
    {
      _handle = handle;
    }

    /// <summary>
    /// Creates an empty virtual file info list.
    /// </summary>
    /// <returns>Virtual file info list object.</returns>
    public static VirtualFileInfoList emptyList()
    {
      return VirtualFileInfoList.fromHandle(uxplist_newVirtualFileList());
    }

  }

  public class VirtualFileInfo
  {

    /// <summary>
    /// Creates a new VirtualFileInfo object from a handle.
    /// </summary>
    /// <param name="handle">Handle to be used.</param>
    /// <returns></returns>
    public static VirtualFileInfo fromHandle(IntPtr handle)
    {
      return new VirtualFileInfo(handle);
    }

    private VirtualFileInfo(IntPtr handle)
    {
      _handle = handle;
      readProperties();
    }

    /// <summary>
    /// Reads and stores the virtual file properties into the VirtualFileInfo object.
    /// </summary>
    private void readProperties()
    {
      ByteArray ba = new ByteArray();

      uxpvf_getName(_handle, ba);
      Name = ba.ToString();

      ba.Clear();

      uxpvf_getFullName(_handle, ba);
      FullName = ba.ToString();

      ba.Clear();

      uxpvf_getReality(_handle, ba);
      Reality = ba.ToString();

      ba.Clear();

      Type = (VirtualFileType)uxpvf_getType(_handle);

    }

    public string Name { get; private set; }
    public string FullName { get; private set; }
    public VirtualFileType Type { get; private set; }
    public string Reality { get; private set; }

    private IntPtr _handle;
  }

  public class VirtualFile
  {
    /// <summary>
    /// Creates a new Virtual File object
    /// </summary>
    /// <param name="uxp"></param>
    /// <param name="handle"></param>
    public VirtualFile(UxpFile uxp, IntPtr handle)
    {
      _uxp = uxp;
      _handle = handle;
    }

    /// <summary>
    /// Open a virtual file for direct access.  Data can be accessed
    /// randomly using seek and read routines.As with standard file
    /// access, zero is the start of the data.The read access pointer is set
    /// to zero by default.
    /// </summary>
    /// <param name="uxp">UXP to be opened</param>
    /// <param name="fileSpec">Virtual file name.</param>
    /// <param name="mode">Open mode.  Supported values:
    ///                    @arg<b> ModeRead</b> Opens the file for read only.
    ///                    @arg<b> ModeReadWrite</b> Opens the file for read and write.
    ///                    @arg <b>ModeWrite</b> Opens the file for write.</param>
    ///                    
    ///@note  UXPs with write-one architecture limits write operations to writing
    ///       to the end of a newly created virtual file.Once the new file is
    ///       closed, the virtual file is restricted to read operations.
    public VirtualFile(UxpFile uxp, string fileSpec, Mode mode)
    {
      _uxp = uxp;
      _handle = uxpfile_openVirtualFile(uxp, fileSpec, mode);
    }

    /// <summary>
    /// Reads data from an open virtual file.  The position for the
    /// read must be set by the user or it will default to the current position
    /// after the last read.
    /// </summary>
    /// <param name="buffer">Buffer to receive data.</param>
    /// <param name="max_size">Maximum number of bytes to fetch (-1 indicates a virtual file's page size).</param>
    /// <returns>Number of bytes read.  Zero indicates EOF.</returns>
    public long Read(ByteArray buffer, long max_size)
    {
      return uxpfile_readVirtualFile(_uxp, this, buffer, max_size);
    }

    /// <summary>
    /// Writes a buffer to the open virtual file.  The virtual file
    /// must be open for write.
    /// </summary>
    /// <param name="buffer">Buffer to write.</param>
    /// <returns>Number of bytes written.</returns>
    public long Write(ByteArray buffer)
    {
      return uxpfile_writeVirtualFile(_uxp, this, buffer);
    }

    /// <summary>
    /// Closes an open virtual file.
    /// </summary>
    public void Close()
    {
      uxpfile_closeVirtualFile(_uxp, this);
    }

    /// <summary>
    /// Returns the virtual file handle for internal functions.
    /// </summary>
    /// <param name="vf">VirtualFile to be used.</param>
    public static implicit operator IntPtr(VirtualFile vf)
    {
      return vf._handle;
    }

    /// <summary>
    /// Object Deconstructor
    /// </summary>
    ~VirtualFile()
    {
      
    }
    
    private IntPtr _handle;
    private UxpFile _uxp;
  }

  public class SMEX
  {
    /// <summary>
    /// Create a new SMEX object.
    /// </summary>
    public SMEX ()
    {
      _handle = uxpsmex_newHandle ();
    }

    /// <summary>
    /// Initialize a SmartMessage Exchange Session on the client.
    /// This is the first step in initializing a uxpSMExchangeHandle
    /// for use.The localSessionToken buffer contains the Client's session token.
    /// The client's session token should then be sent to the server.
    /// </summary>
    /// <param name="localSessionToken">Local temporary SmartId to be sent to the server</param>
    /// <returns>status code
    ///  - UXPSMEX_SUCCESS : session initialized successfully
    ///  - UXPSMEX_FAILURE : failed to initialize session</returns>
    public bool Initialize (ByteArray localSessionToken)
    {
      return (SMEXStatus)uxpsmex_initSession (_handle, localSessionToken) == SMEXStatus.Success;
    }

    /// <summary>
    /// Initializes a uxpSMExchangeHandle on the server with a known ID file.
    /// To use this call with Workflow Machine IDs, you must provide
    /// all the challenge responses in the responseHandle.This is the first step in initializing a uxpSMExchangeHandle
    /// for use.The outClientToken buffer contains the Client's session token.
    /// The client's session token should then be sent to the server.
    /// </summary>
    /// <param name="localSmidFile">Known ID file specification.</param>
    /// <param name="challengeResponses">Chellenge lits to provide challenge responses</param>
    /// <param name="localSessionToken">Local ID file buffer to be sent to Remote peer.</param>
    /// <returns></returns>
    public bool InitializeWithKnownSMID (string localSmidFile, ChallengeList challengeResponses, ByteArray localSessionToken)
    {
      return (SMEXStatus)uxpsmex_initSessionKnownSMIDFile (_handle, localSmidFile, challengeResponses, localSessionToken) == SMEXStatus.Success;
    }

    /// <summary>
    /// Attempts to authenticate a uxpSMExchangeHandle
    /// for a known ID that requires interactive authentication.
    /// </summary>
    /// <param name="remoteSessionToken">File specification for known ID</param>
    /// <returns>status code
    ///  - UXPSMEX_SUCCESS : session successfully started on client
    ///  - UXPSMEX_FAILURE : failed to start session on client</returns>
    public bool StartSession (ByteArray remoteSessionToken)
    {
      return (SMEXStatus)uxpsmex_startSession (_handle, remoteSessionToken.GetDataPtr(), remoteSessionToken.Size) == SMEXStatus.Success;
    }

    /// <summary>
    /// Finishes initializing the uxpSMExchangeHandle and
    /// starts the exchange session on the client for a known ID.
    /// After a successfull call you can start using the
    /// uxpSMExchangeHandle to encode and decode messages.
    /// </summary>
    /// <param name="remoteSmidFile">File specification for known ID</param>
    /// <returns></returns>
    public bool StartSessionWithKnownSMIDFile (string remoteSmidFile)
    {
      return (SMEXStatus)uxpsmex_startSessionKnownSMIDFile (_handle, remoteSmidFile) == SMEXStatus.Success;
    }

    /// <summary>
    /// Encode a message using the specified SMEX object
    /// </summary>
    /// <param name="clearText">Test to encode</param>
    /// <param name="encoded">Buffer containing encoded data</param>
    /// <returns>status code
    ///  - UXPSMEX_SUCCESS : session successfully started on server
    ///  - UXPSMEX_FAILURE : failed to start session on server</returns>
    public bool Encode (string clearText, ByteArray encoded)
    {
      return (SMEXStatus)uxpsmex_encode (_handle, clearText, clearText.Length, encoded) == SMEXStatus.Success;
    }

    /// <summary>
    /// Decode a message using the specified SMEX object
    /// </summary>
    /// <param name="encoded">Input to decode</param>
    /// <param name="decoded">Buffer containing encoded data</param>
    /// <returns>status code
    ///  - UXPSMEX_SUCCESS : session successfully started on server
    ///  - UXPSMEX_FAILURE : failed to start session on server</returns>
    public bool Decode (string encoded, ByteArray decoded)
    {
      return (SMEXStatus)uxpsmex_decode (_handle, encoded, encoded.Length, decoded) == SMEXStatus.Success;
    }

    /// <summary>
    /// SMEX Deconstructor
    /// </summary>
    ~SMEX ()
    {
      uxpsmex_freeHandle (_handle);
    }
    private IntPtr _handle;
  }

  public class Id
  {

    /// <summary>
    /// Creates a new Id object.
    /// </summary>
    public Id()
    {
      _handle = uxpid_newHandle();
    }


    /// <summary>
    /// Publishes the current ID metadata as a published ID that
    /// can be used for protecting data, opening sessions, etc.  May
    /// also save the metadata to the ID if the ID has management
    /// privileges.
    /// </summary>
    /// <param name="status">Status of call</param> 
    /// <param name="id">Buffer to receive published ID.</param>
    /// <param name="doc">ID document</param>
    /// <param name="mods">Operation modifiers.  Possible values:
    ///    @arg ModifierCompress Compresses version2 ID to smallest possible size.  This will
    ///                          decrease session and UXP performance slightly.
    ///    @arg ModifierIdentityV2 Creates a version 2 ID.  It is not compatible with
    ///    older versions of Sertainty technology.</param>
    public static void PublishIdToBuffer(Sys.CallStatus status, ByteArray id, string doc)
    {
      uxpid_publishToBuffer(status, id, doc);
    }


    /// <summary>
    /// Publishes the current ID metadata as a published ID that
    /// can be used for protecting data, opening sessions, etc.May
    /// also save the metadata to the ID if the ID has management
    /// privileges.
    /// </summary>
    /// <param name="status">Status of call</param>
    /// <param name="id">File to receive published ID.</param>
    /// <param name="doc">ID document</param>
    /// <param name="mods">Operation modifiers.  Possible values:
    ///    @arg ModifierCompress Compresses version2 ID to smallest possible size.This will
    ///                          decrease session and UXP performance slightly.
    ///    @arg ModifierReplace
    ///    @arg ModifierIdentityV2 Creates a version 2 ID.It is not compatible with
    ///    older versions of Sertainty technology.</param>
    public static void PublishIdToFile(Sys.CallStatus status, string id, string doc, long mods)
    {
      uxpid_publishToFile(status, id, doc, mods);
    }

    /// <summary>
    /// Authenticates the user after the session has been opened.  If
    /// the call returns a status of<b>StatusChallenged</b>, then the challenge list will contain
    /// a set of questions that must be answered by the user.
    /// If an optional callback function is provided, challenges will be sent to it; otherwise, the
    /// caller must process the challenge list directly.
    /// </summary>
    /// <returns>Status of authentication. Possible values:
    ///   @arg<b> StatusAuthorized</b> Access has been authorized.
    ///   @arg<b> StatusCanceled</b> The user has canceled the authorization attempt.
    ///   @arg <b>StatusChallenged</b> Access requires more challenges to be met.
    ///   @arg<b> StatusNotAuthorized</b> Access has been denied.</returns>
    public int Authenticate()
    {
      return uxpid_authenticate(_handle);
    }

    /// <summary>
    /// Opens a UXP file using an ID session.  If an existing UXP
    /// is open, it will be closed prior to opening the specified object.
    /// </summary>
    /// <param name="uxpFile">UXP to open.</param>
    /// <param name="source">Source file specification.</param>
    /// <param name="mode">Sets the access mode.  If the UXP resides in a read-only file system,
    ///             or the physical UXP file is locked, then the mode will be
    /// forced to ShareReadOnly.Possible values:
    ///    @arg<b> ShareNone</b> Opens UXP for exclusive access.
    ///    @arg<b> ShareAll</b> Opens UXP for sharing with others.
    ///    @arg <b>ShareReadOnly</b> Opens UXP read-only.
    ///    @arg <b>ShareAnon</b> Opens UXP in anonymous mode.</param>
    /// <returns>Status of call
    ///   @arg<b> StatusAuthorized</b> Access has been authorized.
    ///   @arg<b> StatusNotAuthorized</b> Access has been denied.</returns>
    public int OpenUxpFromFile(UxpFile uxpFile, string source, int mode)
    {
      return uxpid_openUxpFromFile(_handle, uxpFile, source, mode);
    }

    /// <summary>
    /// Gets the challenge count from a ID handle.  Challenges are
    ///        constructed by the UXP ID during the authentication process.If authentication
    ///        sets the status to<b>StatusChallenged</b>, then the user must respond appropriately
    ///        to the challenges.
    /// </summary>
    /// <returns>Number of challenges found.</returns>
    public int GetChallengeCount()
    {
      return uxpid_getChallengeCount(_handle);
    }

    /// <summary>
    /// Adds a response to the challenge response list.
    /// </summary>
    /// <param name="ch_handle">Challenge handl. </param>
    public void AddResponse(IntPtr ch_handle)
    {
      uxpid_addResponse(_handle, ch_handle);
    }

    /// <summary>
    /// Gets the list of access challenges for session authentication.  Challenges are
    /// constructed by the UXP during the authentication process.If authentication
    /// sets the status to<b> StatusChallenged</b>, then the user must respond appropriately
    /// to the challenges within the list.
    /// </summary>
    /// <returns>List handle containing challenges.</returns>
    public IntPtr GetChallenges()
    {
      return uxpid_getChallenges(_handle);
    }

    /// <summary>
    /// Open a UXP ID session as an authenticated user to decode
    /// secure messages and/or verify identity.An authenticated session
    /// can encode and decode SmartMessages as well auto authenticate UXPs.
    ///
    /// To open a session, the caller must either set the challenge callback
    /// function or manually manage the challenge/response list.
    /// </summary>
    /// <param name="idFile">ID file to use for the session.</param>
    /// <param name="share">True if allowing this session to be a shared single sign-on session.  
    ///                     If false, the session is only valid for the current process.</param>
    public void OpenSessionFromFile(string idFile,bool share)
    {
      uxpid_openSessionFromFile(_handle,idFile,share,0);
    }

    /// <summary>
    /// Gets the specified challenge number from the ID handle.  Challenges are
    /// constructed by the ID during the authentication process.If authentication
    /// sets the status to<b> StatusChallenged</b>, then the user must respond appropriately
    /// to the challenges.Challenges start at offset zero and end at challenge count minus 1.
    /// </summary>
    /// <param name="offset">Challenge offset</param>
    /// <returns></returns>
    public Challenge GetChallenge(int offset)
    {
      IntPtr ch_handle = uxpid_getChallenge(_handle, offset);
      return Challenge.fromHandle(ch_handle);
    }

    /// <summary>
    /// Closes current UXP ID session
    /// </summary>
    public void CloseSession()
    {
      uxpid_closeSession(_handle);
    }

    /// <summary>
    /// Id object deconstructor.
    /// </summary>
    ~Id()
    {
      uxpid_freeHandle(_handle);
    }

    private IntPtr _handle;

    /// <summary>
    /// Returns internal handle for ID object.
    /// </summary>
    /// <param name="id">Id to be used.</param>
    public static explicit operator IntPtr(Id id)
    {
      return id._handle;
    }
  }

  public class Config : HasCallStatus
  {
    /// <summary>
    /// Config object constructor using a handle
    /// </summary>
    /// <param name="handle">Handle to be used</param>
    private Config(IntPtr handle)
    {
      _handle = handle;
    }

    /// <summary>
    /// Gets the configuration for the current application.
    /// </summary>
    /// <param name="useStale">0 if wait for valid configuration scan.</param>
    /// <returns>New Config object</returns>
    public static Config GetCurrentConfig(bool useStale)
    {
      return new Config(uxpcf_getCurrentConfig(useStale));
    }

    /// <summary>
    /// Get the location identifier for the configuration.
    /// </summary>
    /// <returns>Location object from configuration</returns>
    public Location GetLocation()
    {
      return new Location(uxpcf_getLocation(_handle));
    }

    /// <summary>
    /// Config object deconstructor
    /// </summary>
    ~Config()
    {
      uxpcf_freeHandle(_handle);
    }
  }

  public class Location : HasCallStatus
  {
    /// <summary>
    /// Creates a new Location object from a handle. 
    /// </summary>
    /// <param name="handle">Handle to be used.</param>
    public Location(IntPtr handle)
    {
      _handle = handle;
    }

    /// <summary>
    /// Gets the specified property
    /// </summary>
    /// <param name="name">Property name</param>
    /// <returns>Property value as a string.</returns>
    public string GetProperty(string name)
    {
      return uxploc_getProperty(_handle, name).ReadString();
    }

    /// <summary>
    /// Gets the location properties as a table of variables.
    /// </summary>
    /// <returns>Table of location properties.</returns>
    public VariableList GetTable()
    {
      return VariableList.fromHandle(uxploc_getTable(_handle));
    }
  }

  public class UxlMsg
  {
    /// <summary>
    /// Loads secure text strings.  Secure messages are construct using the
    /// <b>SertaintyString</b> utility.
    /// </summary>
    /// <param name="callStatus">Status handle</param>
    /// <param name="domain">Name of the secure message group.</param>
    /// <param name="fileName">File containing secure messages.</param>
    /// <param name="key">Security key</param>
    public static void LoadSecureString(Sys.CallStatus callStatus, string domain, string fileName, byte[] key)
    {
      IntPtr keyPtr = Sys.BytesToCharPtr(key);
      uxlmsg_loadSecureText(callStatus, domain, fileName, keyPtr, key.Length);
    }

    /// <summary>
    /// Gets a secure text string.  Secure strings are protected on disk
    /// and in memory, meaning that a memory dump will expose nothing.
    /// </summary>
    /// <param name="callStatus">Status handle.</param>
    /// <param name="domain">Secure string domain.</param>
    /// <param name="id">Message identifier.</param>
    /// <returns>String containing secure text.</returns>
    public static string GetSecureText(Sys.CallStatus callStatus, string domain, int id)
    {
      ByteArray ba = new ByteArray();
      uxlmsg_getSecureText(callStatus, ba, domain, id);
      return ba.ToString();
    }
  }

  public class UxpMsg : HasCallStatus
  {
    /// <summary>
    /// Creates a new UxpMsg object
    /// </summary>
    public UxpMsg()
    {
      _handle = uxpmsgid_newHandle();
    }

    /// <summary>
    /// Links the specified ID to the smartMsg object. The specified
    /// ID must remain active while using the smartMsg object.  If an existing
    /// ID is already linked to a smartMsg object, it will be deleted.
    /// </summary>
    /// <param name="id">User-specified ID</param>
    public void SetLinkedID(Id id)
    {
      uxpmsgid_setLinkedID(_handle, (IntPtr)id);
    }

    /// <summary>
    /// Creates and opens a protected message buffer for write
    /// </summary>
    /// <param name="outbuf">Buffer to receive protected data</param>
    /// <param name="exchange">Optional exchange session object.  If zero, then the current
    ///                        session will be used.</param>
    /// <param name="page_size">Sets the page size for the protected file.  A value of -1
    ///                         lets the system set the size.</param>
    /// <param name="cache_size">Sets the internal cache size.  The size is the number
    ///                          in-memory pages to maintain for frequently accessed data.
    ///                          A value of -1 lets the system set the size.A zero disables
    ///                          the cache.  Can be a value from -1 to 20.</param>
    /// <param name="mods">Operation modifiers as bit values.  Support values are:
    ///                    @arg ModifierCompress Compress data before protecting it
    ///                    @arg ModifierMinSize Include minimal header information to reduce size</param>
    /// <returns>Message buffer object</returns>
    public UxpMessageBuffer OpenNewBuffer(ByteArray outbuf, IntPtr exchange, int page_size, int cache_size, int mods)
    {
      IntPtr fileHandle = uxpmsgid_openNewBuffer(_handle, outbuf, exchange, page_size, cache_size, mods);
      return UxpMessageBuffer.fromHandle(_handle, fileHandle);
    }

    /// <summary>
    /// Open a protected data area for direct access.  Data can be accessed
    /// randomly using seek and read routines.As with standard file
    /// access, zero is the start of the data.The read access pointer is set
    /// to zero by default.
    /// </summary>
    /// <param name="buffer">Buffer containing protected data</param>
    /// <returns>Message buffer object</returns>
    public UxpMessageBuffer OpenBuffer(ByteArray buffer)
    {
      IntPtr ptrBuff = buffer;
      IntPtr ptrData = uxpba_getData(ptrBuff);

      IntPtr fileHandle = uxpmsgid_openBuffer(_handle, ptrData, buffer.Size);
      return UxpMessageBuffer.fromHandle(_handle, fileHandle);
    }

  }

  public class UxpMessageBuffer : HasCallStatus
  {
    /// <summary>
    /// Creates a new UxpMessageBuffer object.
    /// </summary>
    /// <param name="handle">handle to be used.</param>
    /// <param name="fileHandle">File handle to be used.</param>
    private UxpMessageBuffer(IntPtr handle, IntPtr fileHandle)
    {
      _handle = handle;
      _fileHandle = fileHandle;
    }

    /// <summary>
    /// Creates a new UxpMessageBuffer object from a handle. 
    /// </summary>
    /// <param name="handle">Handle to be used.</param>
    /// <param name="fileHandle">File handle to be used.</param>
    /// <returns>UxpMessageBuffer object</returns>
    public static UxpMessageBuffer fromHandle(IntPtr handle, IntPtr fileHandle)
    {
      return new UxpMessageBuffer(handle, fileHandle);
    }

    /// <summary>
    /// Gets properties for a protected message
    /// </summary>
    /// <param name="out_buffer">List of properties as an XML document.</param>
    public void GetProperties(ByteArray out_buffer)
    {
      if(_fileHandle == IntPtr.Zero)
      {
        throw new NullReferenceException(nullRefErrorMessage);
      }
      uxpmsgid_getProperties(_handle, _fileHandle, out_buffer);
    }

    /// <summary>
    /// Gets the next buffer for a protected data read.  The position for the
    /// read must be set by the user or it will default to the current position
    /// after the last read.
    /// </summary>
    /// <param name="buffer">Buffer to receive data</param>
    /// <param name="max">Maximum buffer to fetch (-1 indicates a default size)</param>
    /// <returns>Number of bytes read.  A -1 indicates EOF.</returns>
    public int Read(ByteArray buffer, int max)
    {
      if (_fileHandle == IntPtr.Zero)
      {
        throw new NullReferenceException(nullRefErrorMessage);
      }
      return uxpmsgid_read(_handle, _fileHandle, buffer, max);
    }

    /// <summary>
    /// Writes a buffer to the end of an open protected file
    /// </summary>
    /// <param name="in_buffer">Buffer to write</param>
    /// <param name="length">Buffer length</param>
    /// <returns>Number of bytes written.</returns>
    public int Append(string in_buffer, int length)
    {
      if (_fileHandle == IntPtr.Zero)
      {
        throw new NullReferenceException(nullRefErrorMessage);
      }
      return uxpmsgid_append(_handle, _fileHandle, in_buffer, length);
    }

    /// <summary>
    /// Closes protected data access.
    /// </summary>
    public void Close()
    {
      if (_fileHandle == IntPtr.Zero)
      {
        throw new NullReferenceException(nullRefErrorMessage);
      }
      uxpmsgid_close(_handle, _fileHandle);
      _fileHandle = IntPtr.Zero;
    }

    private IntPtr _fileHandle;
    private const string nullRefErrorMessage = "Message buffer not initialized or already closed.";
  }

  public class UxpServices : HasCallStatus, IUxpServices
  {
    public UxpServices()
    {
      _handle = uxpds_getService();
    }
    
    /// <summary>
    /// Creates a new UxpServices object and gets the default data services handle.
    /// </summary>
    public static UxpServices GetServices()
    {
      IntPtr handle = uxpds_getService();
      return new UxpServices(handle);
    }

    public void Destroy()
    {
      uxpds_freeHandle(_handle);
    }

    /// <summary>
    /// Creates an UxpServices object from a handle.
    /// </summary>
    /// <param name="handle">Handle to be used.</param>
    private UxpServices(IntPtr handle)
    {
       _handle = handle;
    }

    /// <summary>
    /// Creates an UxpServices object from a handle.
    /// </summary>
    /// <param name="handle">Handle to be used.</param>
    /// <returns>New UxpServices object</returns>
    public static UxpServices fromHandle(IntPtr handle)
    {
      return new UxpServices(handle);
    }

    /// <summary>
    /// Creates the required database for services management.  
    /// The initial user will be created from the provided Sertainty ID.
    /// The database is required by several system functions.
    ///
    /// The database remains open after initialization.
    /// </summary>
    /// <param name="spec">Database file specification.</param>
    /// <param name="id">Buffer containing system UXP ID for user.  The user defined in the
    ///                  UXP ID will be added to the local database.For custom
    ///                  management scenario, the user must be stored in the caller's
    ///                  non-Sertainty data service.</param>
    /// <param name="username">Username from UXP ID to use as user name.</param>
    /// <param name="dbmodule">Database module (Defaults to UXP).  Possible values:
    ///                        @arg UXP Use the native UXP SQL access module.
    ///                        @arg MySQL Use the MySQL database module(Under construction).
    ///                        @arg Oracle Use the Oracle database module(Under construction).
    ///                        @arg SQLServer Use the SQL Server database module(Windows-only) (Under construction).</param>
    /// <param name="dbparams">Optional parameters for database module.  Parameters are character
    ///                        string with each parameter seperated by a '|' character.The parameters must
    ///                        be specified in the correct order based on the database module:
    ///                        @arg UXP ignores the parameters.
    ///                        @arg MySQL uses the following parameters:
    ///                           - Database host name.
    ///                           - Database port number.
    ///                           - Database user password for user uxpds.
    ///                        @arg Oracle (Not implemented).
    ///                        @arg SQLServer (Not implemented).</param>
    public void InitializeDatabase(string spec, ByteArray id, string username, string dbmodule, string dbparams)
    {
      uxpds_initializeDatabase(_handle, spec, id, username, dbmodule, dbparams);
    }

    /// <summary>
    /// Opens the services database.
    /// </summary>
    /// <param name="spec">Database file specification.</param>
    /// <param name="readonlyInt">True if readonly access.</param>
    public void OpenDatabase(string spec, int readonlyInt)
    {
      uxpds_openDatabase(_handle, spec, readonlyInt);
    }

    /// <summary>
    /// Opens a services session.  To be usable, the current user must also
    /// be authenticated.
    /// </summary>
    /// <param name="username">User to authenticate</param>
    /// <returns>DSSession object.</returns>
    public ServiceSession OpenSession(string username)
    {
      long sessionId = uxpds_openSession(_handle, username);
      return ServiceSession.GetSession(this,sessionId);
    }

    /// <summary>
    /// Closes the current database.
    /// </summary>
    public void CloseDatabase()
    {
      uxpds_closeDatabase(_handle);
        }

    /// <summary>
    /// Drops the defined services elements from the database
    /// </summary>
    /// <param name="handle">services handle.</param>
    /// <param name="spec">Database file specification.</param>
    /// <param name="dbmodule">Database module (Defaults to UXP).  Possible values:
    ///     @arg UXP Use the native UXP SQL access module.
    ///     @arg MySQL Use the MySQL database module.
    ///     @param dbparams Optional parameters for database module.  Parameters are character
    ///         string with each parameter seperated by a '|' character.The parameters must
    ///         be specified in the correct order based on the database module:
    ///     @arg UXP ignores the parameters.

    ///     @arg MySQL uses the following parameters:
    ///             - Database host name.

    ///             - Database port number.

    ///             - Database user password for user uxpds.</param>
    public void DropDatabase(string spec, string dbmodule, string dbparams)
    {
     uxpds_dropDatabase(_handle, spec, dbmodule, dbparams);
    }

    /// <summary>
    /// Interface endpoint for external webservice calls
    /// </summary>
    /// <param name="request">Client request sent to the delegate service</param>
    /// <param name="callerIp">Client's IP address. Must be supplied by the hosting server.</param>
    /// <returns>Webservice response as a ByteArray</returns>
        public ByteArray WebService(ByteArray request, string callerIp)
    {
      ByteArray response = new ByteArray();
      uxpds_webService(this, request, response, callerIp);
      return response;
    }

    /// <summary>
    /// Returns internal handle for UxpServices object
    /// </summary>
    /// <param name="services">UxpServices object to be used.</param>
    public static implicit operator IntPtr(UxpServices services)
    {
      return services._handle;
    }

    /// <summary>
    /// UxpServices Deconstructor
    /// </summary>
    ~UxpServices()
    {
      uxpds_freeHandle(_handle);
    }

  }

  public class ServiceSession : HasCallStatus
  {
    /// <summary>
    /// Creates a new DSSession
    /// </summary>
    /// <param name="services">Services object to be used</param>
    /// <param name="sessionId">Session ID to be used during the session</param>
    private ServiceSession(UxpServices services,long sessionId)
    {
      _handle = services;
      _session = sessionId;
    }

    /// <summary>
    /// Creates a new DSSession object from the UxpServices object passed.
    /// </summary>
    /// <param name="services">UxpServices object to be used.</param>
    /// <param name="sessionId">Session ID to be used.</param>
    /// <returns></returns>
    public static ServiceSession GetSession(UxpServices services, long sessionId)
    {
      return new ServiceSession(services, sessionId);
    }

    /// <summary>
    /// Authenticates the user with the service.
    /// </summary>
    /// <returns>Status of call.  Possible values:
    ///          @arg<b> StatusAuthorized</b> User has been authenticated.
    ///          @arg<b> StatusCanceled</b> User has canceled current authentication attempt.
    ///          @arg<b> StatusChallenged</b> User must answer a set of challenges.
    ///          @arg <b>StatusNotAuthorized</b> User access has been denied.</returns>
    public int Authenticate()
    {
      if (_session == -1)
      {
        throw new NullReferenceException(nullRefErrorMsg);
      }
      return uxpds_authenticate(_handle, _session);
    }

    /// <summary>
    /// Gets the challenge count from the most recent authentication.  Challenges are
    /// constructed by the UXP during the authentication process.If authentication
    /// sets the status to<b> StatusChallenged</b>, then the user must respond appropriately
    /// to the challenges.
    /// </summary>
    /// <returns>Number of challenges found.</returns>
    public int GetChallengeCount()
    {
      if (_session == -1)
      {
        throw new NullReferenceException(nullRefErrorMsg);
      }
      return uxpds_getChallengeCount(_handle, _session);
    }

    /// <summary>
    /// Gets the specified challenge number from the most recent authentication.  Challenges are
    /// constructed by the UXP during the authentication process.If authentication
    /// sets the status to<b> StatusChallenged</b>, then the user must respond appropriately
    /// to the challenges.Challenges start at offset zero and end at challenge count minus 1.
    /// </summary>
    /// <param name="offset">Challenge offset</param>
    /// <returns>Challenge object</returns>
    public Challenge GetChallenge( int offset)
    {
      if (_session == -1)
      {
        throw new NullReferenceException(nullRefErrorMsg);
      }
      IntPtr ch_handle = uxpds_getChallenge(_handle, _session, offset);
      return Challenge.fromHandle(ch_handle);
    }

    /// <summary>
    /// Adds a response to the challenge response list.
    /// </summary>
    /// <param name="response">Challenge handle.</param>
    public void AddResponse(IntPtr response)
    {
      if (_session == -1)
      {
        throw new NullReferenceException(nullRefErrorMsg);
      }
      uxpds_addResponse(_handle, _session, response);
    }

    /// <summary>
    /// Sets the server URL.  The server URL will be linked to
    /// each deleget.Once activated by a UXP containing the delegate,
    /// the URL will be contacted.
    /// </summary>
    /// <param name="url">Server URL</param>
    /// @note When the server URL changes, all identity delegates will be
    ///       regenerated.This will invalidate any delegates already in use.
    public void SetServer(string url)
    {
      if (_session == -1)
      {
        throw new NullReferenceException(nullRefErrorMsg);
      }
      uxpds_setServer(_handle, _session, url);
    }

    /// <summary>
    /// Adds a user to the current service.  The current session user
    /// must have PRIV_ADMIN privileges.
    /// </summary>
    /// <param name="username">User name for the new user. The user must exist in the required
    ///                        UXP ID objects.</param>
    /// <param name="formalname">User formal name.</param>
    /// <param name="description">Description of user.</param>
    /// <param name="email">Email address of user.</param>
    /// <param name="privileges">Privileges as a bitmask.  Possible values:
    ///                          @arg PRIV_NORMAL - No privileges.Standard user.
    ///                          @arg PRIV_ADMIN - User administrator.
    ///                          @arg PRIV_JOURNAL - Journal management.</param>
    /// <param name="app_data1">Optional application data that is passed back to the application
    ///                         during remote user validation.</param>
    /// <param name="app_data2">Optional application data that is passed back to the application
    ///                         during remote user validation.</param>
    public void NewUser(string username, string formalname, string description, string email, int privileges, string app_data1, string app_data2)
    {
      if (_session == -1)
      {
        throw new NullReferenceException(nullRefErrorMsg);
      }
      uxpds_newUser(_handle, _session, username, formalname, description, email, privileges, app_data1, app_data2);
    }

    /// <summary>
    /// Adds a new UXP ID definition for the specified user.
    /// </summary>
    /// <param name="username">User name.  Must either be the current
    ///                        authenticated user, or the current user must have PRIV_ADMIN privileges.</param>
    /// <param name="idname">Name of the UXP ID definition.  An identity must be unique to the user.</param>
    /// <param name="flags">Identity flags as a bit mask.  Possible values:
    ///                     @arg ID_FLAGS_PRIVATE - Private identity.
    ///                     @arg ID_FLAGS_PUBLIC - Public identity.
    ///                     @arg ID_FLAGS_DELEGATE - Delegate identity.
    ///                     @arg ID_FLAGS_SESSION - Session identity.</param>
    /// <param name="description">Description of identity</param>
    /// <param name="uxpid">Compiled UXP ID.  Username must exist in ID.</param>
    /// <param name="source">Source identity in XML format. Can only be fetched by
    ///                      the owner, regardless of privilege.</param>
    /// @note To subscribe to a delegate, a user must have a UXP ID with the ID_FLAGS_DELEGATE flag set
    /// for delegate resolution. Additionally, at least one UXP ID must exist having
    /// the ID_FLAGS_SESSION flag set; otherwise, a user will not be able to manage their user settings
    /// on the service.
    public void NewUserId(string username, string idname, int flags, string description, ByteArray uxpid, ByteArray source)
    {
      if (_session == -1)
      {
        throw new NullReferenceException(nullRefErrorMsg);
      }
      uxpds_newUserId(_handle, _session, username, idname, flags, description, uxpid, source);
    }

    /// <summary>
    /// Created a new DelegateService object from the DSSession.
    /// </summary>
    /// <returns>DelegateService Object</returns>
    public DelegateService OpenDelegateService()
    {
      return DelegateService.CreateDelegateService(this);
    }

    /// <summary>
    /// Gets all users from the data service.  The current session must have PRIV_ADMIN
    /// privileges.
    /// </summary>
    /// <returns>ByteArrayList object of user names.</returns>
    public ByteArrayList GetUsers()
    {
      if (_session == -1)
      {
        throw new NullReferenceException(nullRefErrorMsg);
      }

      IntPtr userList = uxpds_getUsers(_handle, _session);
      if (HasError)
      {
        return new ByteArrayList();
      }
      return ByteArrayList.fromHandle(userList);
    }

    /// <summary>
    /// Gets user information.
    /// </summary>
    /// <param name="username">User name for search.  Must either be the current
    ///                        authenticated user, or the current user must have PRIV_ADMIN privileges.</param>
    /// <returns>DelegateUser object with user properties.</returns>
    public DelegateUser GetUser(string username)
    {
      ByteArray formalname = new ByteArray();
      ByteArray description = new ByteArray();
      ByteArray email = new ByteArray();
      int privileges = 0;
      ByteArray app_data1 = new ByteArray();
      ByteArray app_data2 = new ByteArray();

      if (_session == -1)
      {
        throw new NullReferenceException(nullRefErrorMsg);
      }

      uxpds_getUser(_handle, _session, username, formalname, description, email, ref privileges, app_data1, app_data2);

      return new DelegateUser(username, ByteArray.fromHandle(formalname), ByteArray.fromHandle(description), email, privileges, ByteArray.fromHandle(app_data1), ByteArray.fromHandle(app_data2));
    }

    /// <summary>
    /// Closes an open services session.
    /// </summary>
    public void CloseSession()
    {
      uxpds_closeSession(_handle, _session);
      _session = -1;
    }

    /// <summary>
    /// GCreates UXPServices object from a handle
    /// </summary>
    /// <returns>New UxpServices object.</returns>
    public UxpServices GetUxpServices()
    {
      return UxpServices.fromHandle(_handle);
    }

    /// <summary>
    /// Returns internal session handle for DSSession object.
    /// </summary>
    /// <param name="session">DSSession object to be used.</param>
    public static implicit operator long(ServiceSession session)
    {
      return session._session;
    }

    private long _session = -1;
    private const string nullRefErrorMsg = "Session is not initialized or already closed";
  }

  public class DelegateService : HasCallStatus
  {

    /// <summary>
    /// Creates a new DelegateService object.
    /// </summary>
    /// <param name="handle">Handle to be used</param>
    /// <param name="session">DSSession object to be used.</param>
    private DelegateService(IntPtr handle, ServiceSession session)
    {
      _handle = handle;
      _session = session;
    }

    /// <summary>
    /// Creates a new DelegateService object from a DSSession
    /// </summary>
    /// <param name="session">DSSession to be used.</param>
    /// <returns>New DelegateService object.</returns>
    public static DelegateService CreateDelegateService(ServiceSession session)
    {
      IntPtr handle = uxpdl_newHandle(session.GetUxpServices());
      return new DelegateService(handle, session);
    }

    /// <summary>
    /// Adds a new delegate for the specified user.  A delegate defines a user when added
    /// to a UXP ID will permit validated subscribers to access a UXP that was created by
    /// the UXP ID.
    /// </summary>
    /// <param name="owner">User who owns the delegate.  If owner is not the current
    ///                     authenticated user, then the current user must have PRIV_ADMIN privileges.</param>
    /// <param name="name">Delegate name to to add.</param>
    /// <param name="description">Delegate description.</param>
    /// <param name="expiration">Expiration of subscription.  Zero indicates no expiration.</param>
    /// <param name="permit_offline">Permit offline validation. 1 or 0.</param>
    /// <param name="offline_duration">Number of hours since last server validation.  Must not be zero.</param>
    /// <param name="access_max">Maximum number of successful accesses for a subscriber.  A zero indicates no limit.</param>
    /// <param name="flags">Flags to control delegate visibility and behavior.  Possible values:
    ///                     @arg DELEGATE_FLAGS_PUBLIC - Delegate ID can be fetched by anonymous user.</param>
    public void NewDelegate(string owner, string name, string description, long expiration, int permit_offline, int offline_duration, int access_max, int flags)
    {
      if (_session == -1)
      {
        throw new NullReferenceException(nullRefErrorMsg);
      }
      uxpdl_newDelegate(_handle, _session, owner, name, description, expiration, permit_offline, offline_duration, access_max, flags);
    }

    /// <summary>
    /// Subscribes the specified user to a delegate.
    /// </summary>
    /// <param name="owner">User who owns the delegate.  If owner is not the current
    ///                     authenticated user, then the current user must have PRIV_ADMIN privileges.</param>
    /// <param name="name">Delegate name</param>
    /// <param name="member">User name to add to the delegate member list.</param>
    /// <param name="expiration">Expiration of subscription.  Zero indicates no expiration. A
    ///                          -1 indicates use delegate value.</param>
    /// <param name="permit_offline">Permit offline validation. A -1 indicates use delegate value.</param>
    /// <param name="offline_duration">Number of hours since last server validation.  Must not be zero. A
    ///                                -1 indicates use delegate value.</param>
    /// <param name="access_max">Maximum number of successful access for this subscriber.  A -1 indicates
    ///                          use delegate value.A zero indicates no limit.</param>
    /// <param name="access_count">Current number of successful access for this subscriber.</param>
    public void Subscribe(string owner, string name, string member, long expiration, int permit_offline, int offline_duration, int access_max, int access_count)
    {
      if (_session == -1)
      {
        throw new NullReferenceException(nullRefErrorMsg);
      }
      uxpdl_subscribe(_handle, _session, owner, name, member, expiration, permit_offline, offline_duration, access_max, access_count);
    }

    /// <summary>
    /// Gets a list of the delegates for a user.
    /// </summary>
    /// <param name="owner">User who owns the delegate.  If owner is not the current
    ///                     authenticated user, then the current user must have PRIV_ADMIN privileges.</param>
    /// <returns>ByteArrayList of delegate names for the specified user.</returns>
    public ByteArrayList GetDelegates(string owner)
    {
      if (_session == -1)
      {
        throw new NullReferenceException(nullRefErrorMsg);
      }

      IntPtr delegateList = uxpdl_getDelegates(_handle, _session, owner);
      if (HasError)
      {
        return new ByteArrayList();
      }
      return ByteArrayList.fromHandle(delegateList);
    }

    /// <summary>
    /// Gets the specified delegate.
    /// </summary>
    /// <param name="owner">User who owns the delegate.  If owner is not the current
    ///                     authenticated user, then the current user must have PRIV_ADMIN privileges.</param>
    /// <param name="name">Delegate name to to fetch.</param>
    /// <returns>Delegate object specified by name with delegate properties</returns>
    public Delegate GetDelegate(string owner, string name)
    {
      ByteArray description = new ByteArray();
      long expiration = -1;
      int permit_offline = -1;
      int flags = -1;
      int offline_duration = -1;
      int access_max = -1;
      ByteArray checksum = new ByteArray();
      ByteArray uxpid = new ByteArray();

      if (_session == -1)
      {
        throw new NullReferenceException(nullRefErrorMsg);
      }

      uxpdl_getDelegate(_handle, _session, owner, name, description, ref expiration, ref permit_offline, ref offline_duration, ref access_max, ref flags, checksum, uxpid);
      DateTime dtExp = new DateTime(1970, 1, 1).AddSeconds(expiration);
      return new Delegate(owner, name, ByteArray.fromHandle(description), dtExp, permit_offline == 1, offline_duration, access_max, ByteArray.fromHandle(checksum), ByteArray.fromHandle(uxpid));
    }

    /// <summary>
    /// Gets a list of users who are subscribers to the specified delegate.
    /// </summary>
    /// <param name="owner">User who owns the delegate.  If owner is not the current
    ///                     authenticated user, then the current user must have PRIV_ADMIN privileges.</param>
    /// <param name="name">Delegate name.</param>
    /// <returns>ByteArrayList of member names for the specified delegate.</returns>
    public ByteArrayList GetSubscribers(string owner, string name)
    {
      if (_session == -1)
      {
        throw new NullReferenceException(nullRefErrorMsg);
      }

      IntPtr subscribersList = uxpdl_getSubscribers(_handle, _session, owner, name);

      if (HasError)
      {
        return new ByteArrayList();
      }

      return ByteArrayList.fromHandle(subscribersList);
    }

    /// <summary>
    /// Gets the specified delegate subscription.
    /// </summary>
    /// <param name="owner">User who owns the delegate.  If owner is not the current
    ///                     authenticated user, then the current user must have PRIV_ADMIN privileges.</param>
    /// <param name="name">Delegate name.</param>
    /// <param name="subscriber">Username of subscriber.</param>
    /// <returns>Subscription object specified by name with subscription properties</returns>
    public Subscription GetSubscription(string owner, string name, string subscriber)
    {
      long expiration = -1;
      int permitOffline = -1;
      int offlineDuration = -1;
      int accessMax = -1;
      int accessCount = -1;

      if (_session == -1)
      {
        throw new NullReferenceException(nullRefErrorMsg);
      }

      uxpdl_getSubscription(_handle, _session, owner, name, subscriber, ref expiration, ref permitOffline, ref offlineDuration, ref accessMax, ref accessCount);
      DateTime dtExp = new DateTime(1970, 1, 1).AddSeconds(expiration);
      return new Subscription(owner, name, subscriber, dtExp, permitOffline == 1, offlineDuration, accessMax, accessCount);
    }

    /// <summary>
    /// Gets a list of delegates to which the user is a subscriber.
    /// </summary>
    /// <param name="username">User to search.  If not the same as the authorized user,
    ///                        then the current user must have PRIV_ADMIN privileges.</param>
    /// <returns>ByteArrayList of subscription owner and name strings for the specified user.</returns>
    public ByteArrayList GetSubscriptions(string username)
    {
      if (_session == -1)
      {
        throw new NullReferenceException(nullRefErrorMsg);
      }
      IntPtr subscriptionList = uxpdl_getSubscriptions(_handle, _session, username);
      if (HasError)
      {
        return new ByteArrayList();
      }
      return ByteArrayList.fromHandle(subscriptionList);
    }

    /// <summary>
    /// DelegateService deconstructor
    /// </summary>
    ~DelegateService()
    {
      uxpdl_freeHandle(_handle);
    }
    private ServiceSession _session;
    private const string nullRefErrorMsg = "Session is not initialized or already closed";

  }
}
