#pragma once
#include <fltKernel.h>

#include "IRP_CREATE.h"


extern "C" NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath);
NTSTATUS FLTAPI UnloadCallback(_In_ FLT_FILTER_UNLOAD_FLAGS Flags);
NTSTATUS FLTAPI SetupCallback(_In_ PCFLT_RELATED_OBJECTS  FltObjects, _In_ FLT_INSTANCE_SETUP_FLAGS  Flags, _In_ DEVICE_TYPE  VolumeDeviceType, IN FLT_FILESYSTEM_TYPE  VolumeFilesystemType);
NTSTATUS FLTAPI QueryTeardownCallback(_In_ PCFLT_RELATED_OBJECTS FltObjects, _In_ FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags);

// This struct will hold callbacks for the different IRPs we want to support 
    // For now we just support IRP_MJ_CREATE since thats used to open a handle to any file 
CONST FLT_OPERATION_REGISTRATION g_callbacks[] =
{
    // Entry for IRP_MJ_CREATE with our callbacks 
    {
        IRP_MJ_CREATE,
        0,
        PreCreateCallback,
        PostCreateCallback
    },

    { IRP_MJ_OPERATION_END } // needs to end with this
};

// This struct provides information about a file system minifilter to the filter manager
    // Mainly has callbacks for events that may happen with our driver 
CONST FLT_REGISTRATION g_filterRegistration =
{
    sizeof(FLT_REGISTRATION),      //  Size
    FLT_REGISTRATION_VERSION,      //  Version
    0,                             //  Flags
    NULL,                          //  Context registration
    g_callbacks,                   //  Operation callbacks
    UnloadCallback,                //  FilterUnload
    SetupCallback,                 //  InstanceSetup
    QueryTeardownCallback,         //  InstanceQueryTeardown
    NULL,                          //  InstanceTeardownStart
    NULL,                          //  InstanceTeardownComplete
    NULL,                          //  GenerateFileName
    NULL,                          //  GenerateDestinationFileName
    NULL                           //  NormalizeNameComponent
};