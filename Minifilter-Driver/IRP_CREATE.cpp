// Used primarily to implement callbacks specific to the IRP_MJ_CREATE irp
#include "IRP_CREATE.h"


// Callback thats invoked before the filesystem actaully gets a chance to touch the file
FLT_PREOP_CALLBACK_STATUS PreCreateCallback(
    _Inout_                        PFLT_CALLBACK_DATA    Data,
    _In_                           PCFLT_RELATED_OBJECTS FltObjects,
    _Flt_CompletionContext_Outptr_ PVOID* CompletionContext)
{
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(CompletionContext);

    DbgPrint("Pre IRP_MJ_CREATE: %wZ\n", &Data->Iopb->TargetFileObject->FileName);

    // We can behave differently depending on who and where we are being opened from
    if (Data->RequestorMode == UserMode)
    {
        PEPROCESS currentProcess = PsGetCurrentProcess();
        PUNICODE_STRING processName = { 0 };

        // Fetch the executable image name string
        NTSTATUS status = SeLocateProcessImageName(currentProcess, &processName);

        if (NT_SUCCESS(status) && processName != NULL) 
        {
            // Check if the calling process is notepad.exe
            UNICODE_STRING targetProcess;
            RtlInitUnicodeString(&targetProcess, L"notepad.exe");

            // Look for a substring match at the end of the full path
            if (RtlSuffixUnicodeString(&targetProcess, processName, TRUE)) 
            {
                // Block the file open request completely
                Data->IoStatus.Status = STATUS_ACCESS_DENIED;
                Data->IoStatus.Information = 0;

                ExFreePool(processName); 
                return FLT_PREOP_COMPLETE; // Tell FltMgr to stop processing and return error
            }

            // Clean up name buffer if it wasn't blocked
            ExFreePool(processName);
        }
    }

    return FLT_PREOP_SUCCESS_NO_CALLBACK;
}

// Callback thats invoked after the filesystem has worked with the target file 
FLT_POSTOP_CALLBACK_STATUS PostCreateCallback(
    _Inout_  PFLT_CALLBACK_DATA    Data,
    _In_     PCFLT_RELATED_OBJECTS FltObjects,
    _In_opt_ PVOID                 CompletionContext,
    _In_     FLT_POST_OPERATION_FLAGS Flags)
{
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(CompletionContext);
    UNREFERENCED_PARAMETER(Flags);

    DbgPrint("Post IRP_MJ_CREATE: %wZ\n", &Data->Iopb->TargetFileObject->FileName);

    return FLT_POSTOP_FINISHED_PROCESSING;
}