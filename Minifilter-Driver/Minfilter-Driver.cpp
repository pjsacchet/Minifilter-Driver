// Main entry and implementation for our minifilter
#include "Minifilter-Driver.h"

#pragma warning( push )
#pragma warning( disable : 4459 )

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

NTSTATUS RegistryQueryCallback(
	IN PWSTR ValueName,
	IN ULONG ValueType,
	IN PVOID ValueData,
	IN ULONG ValueLength,
	IN PVOID Context,
	IN PVOID EntryContext
) {
	UNREFERENCED_PARAMETER(ValueName);
	UNREFERENCED_PARAMETER(ValueType);
	UNREFERENCED_PARAMETER(ValueLength);
	UNREFERENCED_PARAMETER(Context);

	PUNICODE_STRING targetString = (PUNICODE_STRING)EntryContext;

	// Ensure we actually have data to read 
	if (ValueData != NULL) {
		// Duplicate the string data safely into kernel memory
		RtlCreateUnicodeString(targetString, (PCWSTR)ValueData);
	}
	return STATUS_SUCCESS;
}

// Main entry point Windows looks for when running our driver 
extern "C" NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	NTSTATUS status = STATUS_SUCCESS;
	RTL_QUERY_REGISTRY_TABLE queryTable[1] = { 0 };
	LPWSTR g_executable = nullptr;

	queryTable[0].QueryRoutine = RegistryQueryCallback;
	queryTable[0].Flags = RTL_QUERY_REGISTRY_REQUIRED;
	queryTable[0].Name = L"1"; // First and only arg
	queryTable[0].EntryContext = g_executable; // Our global so our callback can read this

	status = RtlQueryRegistryValues(RTL_REGISTRY_ABSOLUTE, RegistryPath->Buffer, queryTable, NULL, NULL);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("ERROR; Failed RtlQueryRegistryValues\n");
		return status;
	}

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