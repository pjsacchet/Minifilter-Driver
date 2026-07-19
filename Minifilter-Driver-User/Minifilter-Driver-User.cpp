// User mode executable will load and run minifitler driver 
    // Will assume driver is already installed via the inf file provided
    // Requires admin privs to run 

#include "Minifilter-Driver-User.h"

// Start our driver and pass the executable name to the driver
BOOL StartDriver(__in std::string executable)
{
    SC_HANDLE hSCM, hMinifilterDriver;
    LPCSTR args[] = { executable.c_str() };

    // Open a handle to the SCM
    hSCM = OpenSCManagerA(NULL, SERVICES_ACTIVE_DATABASEA, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL)
    {
        if (GetLastError() == ERROR_ACCESS_DENIED)
        {
            printf("Failed OpenSCManagerA, access denied; make sure to execute with admin privileges!\n");
        }

        else
        {
            printf("Failed OpenSCManagerA; ERROR 0x%X\n", GetLastError());
        }
        
        return FALSE;
    }

    // Open handle to our service
    hMinifilterDriver = OpenServiceA(hSCM, "Minifilter-Driver", SERVICE_ALL_ACCESS);
    if (hMinifilterDriver == NULL)
    {
        if (GetLastError() == ERROR_SERVICE_DOES_NOT_EXIST)
        {
            printf("Failed OpenServieA, service does not exist; make sure to install via the inf file first!\n");
        }

        else
        {
            printf("Failed OpenServiceA; ERROR 0x%X\n", GetLastError());
        }
        
        return FALSE;
    }

    // Start our service with the provided arg
    if (!StartServiceA(hMinifilterDriver, 1, args))
    {
        printf("Failed StartServiceA; ERROR 0x%X\n", GetLastError());
        return FALSE;
    }

    return TRUE;
}

int main()
{
    std::string executable;

    printf("Please enter the name of the executable you would like to block from usermode> \n");

    std::getline(std::cin, executable);

    if (!StartDriver(executable))
    {
        printf("Failed to start driver!\n");
        return -1;
    }

    return 0;
}

