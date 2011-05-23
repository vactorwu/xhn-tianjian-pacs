+----------------------------------------+
| ComPort Library version 2.61           |
| for Delphi 3, 4, 5                     |
| and C++ Builder 3, 4, 5                |
|                                        |
| by Dejan Crnila                        |
| 1998-2000                              |
| C++ Builder support by Paul Doland     |
+----------------------------------------+

Contents
1. Author information
2. Files in archive
3. Examples
4. Package names
5. Installing ComPort Library
6. C++ Builder notes
7. Installing help file
8. Known problems and issues
9. Version history


1. Author information
---------------------------------------------------------------------------------
Name: Dejan Crnila
E-mail: dejancrn@yahoo.com
Home page: http://www2.arnes.si/~sopecrni
Home address: Dolenja vas 111, 3312 Prebold, SLOVENIA
Year of birth: 1978
Occupation: Student of computer science at University of Ljubljana

Note:
ComPort Library has been in development more than two years with
a lot of work put in it. I have been thinking about making it
shareware, but i have decided to distribute it as freeware with
sources, because i want to contribute as much as possible to the 
Delphi community on the internet. I have tried several programming
languages, but not a single one comes close to Delphi's simplicity
and power. However, i ask all ComPort Library users to send me at 
least a postcard if they find the component useful. That will give 
me a motivation for further development of the component. I hope 
that this is not too much to ask from you.


2. Files in archive
--------------------------------------------------------------------------------
  ReadMe.Txt - this file
  Sources.zip - ComPort Library sources
  Help.zip - Delphi context-sensitive help file for library
  Examples.zip - Example projects


3. Examples
--------------------------------------------------------------------------------
  ComExample.dpr   - shows some basic send-recieve features
  ModTest.dpr      - modem test console application
  MiniTerm.dpr     - simple terminal application
  CPortMonitor.pas - TCPortMonitor component for monitoring incoming and 
                     outgoing data. This example shows how to link to 
                     TCustomComPort component. Author: Roelof Y. Ensing 
                     (e-mail: ensingroel@msn.com).


4. Package names
--------------------------------------------------------------------------------
		Design-Time Source	Run-Time Source
                ----------------	---------------
Delphi 3	DsgnCPort3.dpk		CPortLib3.dpk
Delphi 4	DsgnCPort4.dpk		CPortLib4.dpk
Delphi 5	DsgnCPort5.dpk		CPortLib5.dpk
C++ Builder 3	DsgnCPortCB3.bpk	CPortLibCB3.bpk
C++ Builder 4	DsgnCPortCB4.bpk	CPortLibCB4.bpk
C++ Builder 5	DsgnCPortCB5.bpk	CPortLibCB5.bpk

		Design-Time library	Run-Time library
		----------------	----------------
Delphi 3	DsgnCPort3.dpl		CPortLib3.dpl
Delphi 4	DsgnCPort4.bpl		CPortLib4.bpl
Delphi 5	DsgnCPort5.bpl		CPortLib5.bpl
C++ Builder 3	DsgnCPortCB3.bpl	CPortLibCB3.bpl (also .lib and .bpi)
C++ Builder 3	DsgnCPortCB3.bpl	CPortLibCB4.bpl (also .lib and .bpi)
C++ Builder 3	DsgnCPortCB3.bpl	CPortLibCB5.bpl (also .lib and .bpi)


5. Installation
--------------------------------------------------------------------------------
Remove all previously installed files of ComPort Library (TComPort
component). Create a new folder under Delphi directory and extract 
Sources zip file into new folder. Set Library Path to new ComPort 
folder (Tools-Environment Options-Library-Library Path).

For Delphi 3, 4, 5 & C++ Builder 4, 5:
(C++ Builder users also need to read the C++ Builder notes)

Use "File/Open" menu item in Delphi/C++ Builder IDE to open 
ComPort run-time package source file (see above). Click "Compile" 
button in Package window to compile the library. Now move run-time
package library file or files (see above) from ComPort folder to a
folder that is accessible through the search PATH (e.g. WinNT\System32).

Now you have to install design-time package. Use File/Open menu item
to open design-time package source file (see above). Click "Compile" 
button in Package window to compile the package and "Install" button
to register ComPort into the IDE. ComPort components appear in 
"CPortLib" page of component pallete.  If it complains that it 
can't install it because it can't find a library, you probably
did not put the run-time package in the search path. You might
not get this error until the next time you try to start
Delphi/C++ Builder.

Note: Do not save packages under Delphi/C++ Builder IDE.

For C++ Builder 3
(C++ Builder users also need to read the C++ Builder notes)

C++ Builder 3 does not have a "Package window" like the other products.
So, installation is slightly different.

Use "File/Open" menu item in Delphi/C++ Builder IDE to open 
ComPort run-time package source file (see above). Compile the
package from the project menu or Ctrl-F9. Now move run-time
package library files (see above) from ComPort folder to a folder
that is accessible through the search PATH (e.g. WinNT\System32).

Now you have to install design-time package. Use File/Open menu item
to open design-time package source file (see above).  Compile the
package from the project menu or Ctrl-F9.  To install the package
into the IDE, go to the Component menu, "Install Packages" option.
Click the Add button.  Browse to the design-time library and select
it.  If it complains that it can't install it because it can't find
a library, you probably did not put the run-time package in the
search path as described above.

Note about Delphi 2:

Note: Delphi 2 is no longer suported, however, with some minor changes, 
it should compile under Delphi 2 as well.

Use "Component/Install" menu item to add "CPortReg.pas" unit to the 
component library. This unit registers ComPort components on 
"CPortLib" page of component pallete.


6. C++ Builder Notes
------------------------------------------------------------------------------
The .hpp file C++ Builder creates for cport.pas will have a bug in it.
The first time you compile a project, you will get one or two duplicate 
definitions within the EComPort exception class.  It seems to be safe to
delete or comment out the duplicates.

Also, the CPortCtl.HPP may have a bug in it.  If you get an error about
the following being ambiguous;

typedef TBitmap TLedBitmap;

Change it to:

typedef Grahpics::TBitmap TLedBitmap;

If someone knows how to fix these more cleanly, send me an e-mail to 
pdoland@flash.net.


7. Installing help file
------------------------------------------------------------------------------
The example shown is for Delphi version 4. For other version of Delphi 
change the 4 to your version number.

a) Copy the CPort.hlp and CPort.cnt to the Delphi4 Help directory.
b) Load the DELPHI4.CFG file (located in the Delphi4/Help directory)
   into an editor such as Notepad.  At the bottom of the file, you 
   will see something like this:

; Third-party Help
;-----------------
:Link quickrpt.hlp
:Link teechart.hlp
:Link imagedit.hlp

c) Add the following line to the bottom of the Third-party Help 
   section:  

:Link CPort.hlp

d) Save the DELPHI4.CFG file.

e) Load the DELPHI4.CNT file (located in the Delphi4/Help directory)
   into an editor such as Notepad.  At the top of the file you will 
   see something like this:

:Base delphi4.HLP>main
:Title Delphi Help

; Index section
;==============
:Index VCL Object and Component Reference=vcl3.hlp
:Index Object Pascal Reference=obpascl3.hlp

At the bottom of the Index section add the following line:

:Index ComPort Library=CPort.hlp

f) Go to the bottom of the DELPHI4.CNT file and you should see an
   "Include section" something like this:

; Include section
;================
:Include win32sdk.toc
:Include winhlp32.cnt

At the bottom of the Include section add the following line:

:Include CPort.cnt

Context-sensitive Help should now be available from the Delphi4 IDE 
(choosing the F1 key while a property is selected should bring up the 
appropriate item in the TComport.HLP file).  Also, the Comport 
Component Help will be available from the Delphi 4 Help file Contents
list, and Index list. 


8. Known problems and issues
-----------------------------------------------------------------------------
  1.) OnRxBuf event handler problem in Delphi IDE

      If user double clicks on OnRxBuf event in Delphi IDE, message pops up
      saying: "Property and Method ComPort1RxBuf are not compatible".
      This is a Delphi IDE bug, since it can't handle untyped parameters
      like Buffer parameter of OnRxBuf event.

      Solution: Application has to assign OnRxBuf handler manually in code.    

  2.) Two serial ports sharing the same IRQ address

      Some users of Windows 95/98 have reported that application can't have
      two TComPort components open simultaneouslly if serial ports share the
      same IRQ address. This usually COM1/COM3 and COM2/COM4.

      Solution: Not known yet.


9. Version history
-----------------------------------------------------------------------------
2.61 (11.6.2000)
  TComDataPacket supports custom packet forming
  TComTerminal LoadFromStream/SaveToStream methods
  lsConn signal support in TComLed
  Other minor changes

2.60 (1.5.2000)
  TComTerminal component
  WaitForEvent method in TComPort component

2.52 (30.3.2000)
  C++ Builder support added by Paul Doland
  Rx and Tx signal support in TComLed

2.51 (3.3.2000)
  OnAfterOpen, OnAfterClose, OnBeforeClose, OnBeforeOpen events
  Support for custom baud rate values
  Other minor changes and fixes

2.50 (10.2.2000)
  New components: TComDataPacket, TComComboBox, 
    TComRadioGroup and TComLed
  BeginUpdate, EndUpdate methods
  StoreSettings, LoadSettings methods
  TComFlowControl.FlowControl property
  Bug fix and code optimization

2.01 (1.11.1999)
  Added EnumComPorts procedure
  Port property changed to PortNum (now String type property)

2.00 (17.10.1999)
  New concept of calling asynchronous operations
  Some new properties, methods
  Code optimization, simplification
  Help file included

1.71 (27.05.1999)
  A bug fix!

1.70 (28.04.1999)
  New methods: SetDTR, SetRTS, SetBreak, SetXonXoff
  Properties can be changed during a session

1.60 (10.04.1999)
  New properties: EventChar, DiscardNull, SyncMethod
  Extended Parity property
  Fixed compilation problems
  Other bug fixes

1.50 (13.03.1999)
  Read/Write operations in asynchronous/synchronous mode
  Added OnRx80Full event
  Extended flow control 
  Added Timeouts property

1.01 (24.10.1998)
  Added more signal detection functions.
  Added this documentation.
  Added PurgeOut function.
  Fixed some minor bugs.

1.00 (29.09.1998)
  Basic version of the component
