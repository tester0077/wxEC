# wxEC


wxEC Enum Compiler 

##### I am posting this code to Github to, hopefully, make it useful for others because it does what I need at present & I no longer have the time for further major development. Please use the code as you see fit. I will try to help with getting anyone to compile the project under Windows, but my time is limited ...

#### All of my code is released under the wxWidgets license, the rest, under whichever license it was released.

 These classes extend the standard C++ enum concept by:
  - allowing the enum to be read and assigned to as a string.
  - ensuring only declared values are used by automatically selecting
    a default/unknown value if an assigned value is not found.
  - initialising the enum to a default/unknown value on creation.
  - handle wxWidgets' wxString

  This utility was inspired by Lloyd Williamsen's EC
 See: http://www.codeguru.com/cpp/cpp/cpp_mfc/article.php/c4001
 
*Purpose: Generate intelligent C++ enumeration classes from standard enum  declarations. This utility scans through files containing standard C++  source code (file which are typically given the suffix .e) for enums  and outputs .cpp and .h files containing a C++ class per enum.*

 Note: Line endings are auto-adjusted to the detected OS, but can be overridden.
 
## Prerequisites for the wxEC project

1. wxWidgets 3.1.0 - compiled with MSVC 2015 for static linking. 
	Uses the environment variable $(WXWIN_310_2015) = path to the wxWidgets root

2. MSVC 2015 IDE - Express - Community version

3. Inno Install builder from http://www.jrsoftware.org/isinfo.php - uses the ISS package 
	from github.com/stfx/innodependencyinstaller. A useful helper script to control the Microsoft re-distributable files also came originally from CodeProject see: https://www.codeproject.com/Articles/20868/NET-Framework-Installer-for-InnoSetup.
 I believe the latest code has now moved to Github: https://github.com/stfx/innodependencyinstaller The .sln file is set up to copy the executable, the help file as well as any documentation files to the directory ./Inno/src and the resulting executable will be left in ./Inno/bin

4. Appropriate MSVC distributable package. The files are expected to be left in ./Inno/bin/myProgramDependencies. These are typically downloaded from Microsoft. Copies of the necessary files for the version of the MSVC IDE are part of the package.

5. RCStamp utility - the executable RCStamp.exe is included in the ./Executables directory 
and is used as part of the 'release' build process to update the 
major, minor & build numbers. To modify either by more than one increment or to modify them in any other way, it is best to edit the .rc file by hand to change any one or all to the new starting value. But note, all 'copies' of each number in the .RC must be modified in sync.
It is a command line utility and you can get some help by running it in a DOS window. You will want to move or copy it to a directory on the PATH so that Windows can find it or you can add the full path in the 'release' build pre-link build event.
It is an adaptation of a CodeProject utility and you can find my modified source and more comments at
tester0077/rcstamp.


```
Usage: wxEC [/h] [/?] [/d] [/i <str>] [/m] [/o <str>] [/p] [/t] [/u] [/v] [/w] [/x] input file(s)...
  /h, --help            show this help message
  /?, --help            show this help message
  /d, --details         Show a more detailed usage message
  /i, --input=<str>     <input path>
  /m, --mac             force MAC line endings
  /o, --output=<str>    <output path>
  /p, --pause           pause at end of compile to allow user to inspect the output
  /t, --timestamp       skip compile if output files are older than input
  /u, --unix            force Unix line endings
  /v, --verbose         be verbose
  /w, --windows         force DOS/Windows line endings
  /x, --wxwidgets       add code for wxWidgets
```
Example:
The original enum file is translated into the header and code files
```
    enum EState
    {
    Unknown = 0,
    Solid = 1, // Solid Matter
    Liquid = 2, // "Liquid Matter "
    Gas, /* Vapour */
    };
```
Header & class file
=================
```
    class EState
    {
    public:
    // EState::Unknown,Solid,Liquid,Gas,
    enum Enum
    {
    Unknown = 0,
    Solid = 1, // "Solid Matter"
    Liquid = 2, // "Liquid Matter "
    Gas, // "Vapour"
    };
     
    Enum operator=(int i);
    operator int () const;
     
    Enum operator=(const char* sz);
    bool operator==(const char* sz);
    operator const char* () const;
     
    ...
    };
     
    EState::Map_t EState::m_Map[] =
    {
    {Unknown, "Unknown"},
    {Solid, "Solid Matter"},
    {Liquid, "Liquid Matter "},
    {Gas, "Vapour"}
    };
```

and used in this manner:
```
    int X;
    ...
    EState eState;
    eState = X;
    if(eState == EState::Unknown) {...}
    ...
    printf("eState is '%s' [%d]", (const char*)eState, eState);
    ...
    eState = "Text String";
    if(eState == EState::Unknown) ...
    ...
```