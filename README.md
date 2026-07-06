# WARNING
PLEASE NOTE: this project is a Windows driver that is NOT meant to be run on your host system. Please have a Windows 10/11 Virtual Machine available for testing if desired. Testing for this tool was done in Windows 10 Home Version 10.0.19045 via VMWare.

# Introduction
One of the many system driver types Windows offers is a minifilter driver. These serve as intermediary drivers that lie between the user and the filesystem that intercept I/O Request Packets (IRPs) that travel from user mode to kernel. If a adversary was able to successfully load a minifilter driver on target, they could manipulate the filesystem in various ways, typically the desired effect is to hide or conceal malicious files on disk from antivirus solutions and/or the user to prevent discovery. In this project, we will take a more nuanced approach and change the outcome of interfacing with a specific malicious file of ours depending on who is asking for information on said file. For example, if Defender is asking to open the file, we return a non malicious string, whereas if Powershell is requesting to read file contents we instead return the file's true contents. 

This tool works by hooking the IRP_MJ_CREATE and denying any usermode requests to open any file with notepad. One can verify this driver is installed and working by attemtping to open any file in notepad and being met with an error. This tool can be easily extended to support other IRPs and other red team functionality. 

# Setup
## DebugView for Output
In order to view ouput from this driver, users will want to download DbgView from system internals (please run this with admin privileges) (https://learn.microsoft.com/en-us/sysinternals/downloads/debugview)

When running DbgView, ensure you check 'Capture Kernel (cntrl+k)' and check 'Enable Verbose Kernel Output' under the 'Capture' tab so you can see all output from our driver.

## Windows System Configuration
In order to run custom drivers, we need to disable integrity checks and enable test signing on Windows, to do so run the following commands (in admin cmd prompt):

```> bcdedit /set testsigning on```

```> bcdedit /set nointegritychecks on```

And reboot your system. Following this, you should be able to see a 'Test Mode' water mark in the bottom right of your screen.

## Installing the Driver
In order to install the driver, all you need are the three output files found under Minifilter-Driver\Minifilter-Driver\x64\Debug\Minifilter-Driver:
- minifilter-driver.cat
- Minifilter-Driver.inf
- Minifilter-Driver.sys

Copy these three files to your target system. At that point, right click on the inf file and select 'install'. This should be a quick and easy process, and the service should automatically start from there (check the debug output to verify)

## Helpful Commands
In order to manually start, stop and delete our service:

```> sc start Minifilter-Driver```

```> sc stop Minifilter-Driver```

```>sc delete Minifilter-Driver```

We can also use the filter manager to load and unload our driver, as well as list all active filters on our machine:

```>fltmc load Minifilter-Driver```

```>fltmc unload Minifilter-Driver```

```>fltmc```

Example output from fltmc:
```
Filter Name                     Num Instances    Altitude    Frame
------------------------------  -------------  ------------  -----
bindflt                                 1       409800         0
UCPD                                    4       385250.5       0
Minifilter-Driver                       4       370030         0
WdFilter                                4       328010         0
storqosflt                              0       244000         0
wcifs                                   0       189900         0
CldFlt                                  0       180451         0
FileCrypt                               0       141100         0
luafv                                   1       135000         0
npsvctrig                               1        46000         0
Wof                                     2        40700         0
FileInfo                                4        40500         0
```

## Registry
Once our driver is installed, one should be able to view it in the registry in the following location:

```Computer\HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Minifilter-Driver```

# Building it Yourself
I really don't recommend attempting to build this, but thats only due to the fact that one would need to install Visual Studio along with that VS's WDK (Windows Driver Kit). This can take up a lot of storage and is a good amount of work for just a simple driver.

# TODO
- Add support for other IRP types
- Add file hiding or other malicious red team functionlaity 

# References
- https://learn.microsoft.com/en-us/windows-hardware/drivers/develop/creating-a-new-filter-driver
- https://github.com/microsoft/Windows-driver-samples/tree/main/filesys/miniFilter/avscan
- https://medium.com/@s12deff/building-a-windows-file-system-minifilter-driver-intercepting-file-access-55b933ccd6a4
- https://www.apriorit.com/dev-blog/675-driver-windows-minifilter-driver-development-tutorial
- https://medium.com/@WaterBucket/understanding-mini-filter-drivers-for-windows-vulnerability-research-exploit-development-391153c945d6
- https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/fltkernel/ns-fltkernel-_flt_registration