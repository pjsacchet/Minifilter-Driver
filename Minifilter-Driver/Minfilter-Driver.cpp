// Main entry and implementation for our minifilter
#include "Minifilter-Driver.h"

PFLT_FILTER g_minifilterHandle = NULL;

// Callback thats invoked before filter is unloaded
NTSTATUS FLTAPI UnloadCallback(_In_ FLT_FILTER_UNLOAD_FLAGS Flags)
{
	UNREFERENCED_PARAMETER(Flags);

	NTSTATUS status = STATUS_SUCCESS;

	if (NULL != g_minifilterHandle)
	{
		FltUnregisterFilter(g_minifilterHandle);
	}

	return status;
}

// This is called to see if a filter would like to attach an instance to the given volume
	// For now just returns
NTSTATUS FLTAPI SetupCallback(_In_ PCFLT_RELATED_OBJECTS  FltObjects, _In_ FLT_INSTANCE_SETUP_FLAGS  Flags, _In_ DEVICE_TYPE  VolumeDeviceType, IN FLT_FILESYSTEM_TYPE  VolumeFilesystemType)
{
	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(Flags);
	UNREFERENCED_PARAMETER(VolumeFilesystemType);
	UNREFERENCED_PARAMETER(VolumeDeviceType);

	NTSTATUS status = STATUS_SUCCESS;

	return status;
}

// This is called to see if the filter wants to detach from the given volume
	// For now just returns
NTSTATUS FLTAPI QueryTeardownCallback(_In_ PCFLT_RELATED_OBJECTS FltObjects, _In_ FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags)
{
	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(Flags);

	NTSTATUS status = STATUS_SUCCESS;

	return status;
}

// Main entry point Windows looks for when running our driver 
extern "C" NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);

	NTSTATUS status = STATUS_SUCCESS;

	// Register our minifilter
	status = FltRegisterFilter(DriverObject, &g_filterRegistration, &g_minifilterHandle);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("ERROR; Failed FltRegisterFilter\n");
		return status;
	}

	// Start filtering!
	status = FltStartFiltering(g_minifilterHandle);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("ERROR; Failed FltStartFiltering\n");
		return status;
	}


	return status;
}