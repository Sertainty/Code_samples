Sertainty C# Sample
===================

Files included
--------------

1. uxpCoreAPI.cs
Reference mapping file from Sertainy C Api to C# via DllImport

2. sample_id_c.cs
Port of sample_id.c to C#

3. uxpCoreWrappers.cs
Object Oriented Wrapper over uxpCoreAPI.cs

4. sample_id.cs
Port of sample_id.c to C# over Object Oriented Wrapper

Compilation Instructions
------------------------

Roslyn compiler is needed in order to compile the sample cs code.

Compiling sample_id_c.cs
`csc.exe /out:sample_cs.exe uxpCoreAPI.cs sample_id_c.cs`

Compiling sample_id.cs
`csc.exe /out:sample_cs.exe Mappings.cs uxpCoreAPI.cs sample_id.cs`

Running sample_cs.exe
---------------------

In order to run sample_cs.exe, first copy sample_cs.exe, SertaintyCore.dll (or SertaintyCored.dll), sampleid.xml, data.pdf, data2.pdf into the same folder, then run sample_cs.exe.
