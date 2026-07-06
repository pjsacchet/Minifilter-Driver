# WARNING
PLEASE NOTE: this project is a Windows driver that is NOT meant to be run on your host system. Please have a Windows 10/11 Virtual Machine available for testing if desired.

# Introduction
One of the many system driver types Windows offers is a minifilter driver. These serve as intermediary drivers that lie between the user and the filesystem that intercept I/O Request Packets (IRPs) that travel from user mode to kernel. If a adversary was able to successfully load a minifilter driver on target, they could manipulate the filesystem in various ways, typically the desired effect is to hide or conceal malicious files on disk from antivirus solutions and/or the user to prevent discovery. In this project, we will take a more nuanced approach and change the outcome of interfacing with a specific malicious file of ours depending on who is asking for information on said file. For example, if Defender is asking to open the file, we return a non malicious string, whereas if Powershell is requesting to read file contents we instead return the file's true contents. 

# Setup
- In order to view ouput from this driver, users will want to download DbgView from system internals (please run this with admin privileges)

- When running DbgView, ensure you check 'Capture Kernel (cntrl+k)' and check everything under the 'Capture' tab 
- In order to run custom drivers, we need to disable integrity checks and enable test signing on Windows, to do so run the following commands (in admin cmd prompt):
  - bcdedit /set testsigning on
  - bcdedit /set nointegritychecks on

# TODO
- Add support for other IRP types
- Add file hiding

# References
- https://learn.microsoft.com/en-us/windows-hardware/drivers/develop/creating-a-new-filter-driver
- https://github.com/microsoft/Windows-driver-samples/tree/main/filesys/miniFilter/avscan
- https://medium.com/@s12deff/building-a-windows-file-system-minifilter-driver-intercepting-file-access-55b933ccd6a4
- https://www.apriorit.com/dev-blog/675-driver-windows-minifilter-driver-development-tutorial
- https://medium.com/@WaterBucket/understanding-mini-filter-drivers-for-windows-vulnerability-research-exploit-development-391153c945d6
- https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/fltkernel/ns-fltkernel-_flt_registration
- 