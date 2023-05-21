#include <ntddk.h>

#define DEVNAME L"\\Device\\TARGETDRIVER"
#define SYMNAME L"\\DosDevices\\TARGETDRIVER"

#define IOCTL_SAGNJUN CTL_CODE(FILE_DEVICE_UNKNOWN,0x800,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAGNJUN2 CTL_CODE(FILE_DEVICE_UNKNOWN,0x801,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAGNJUN3 CTL_CODE(FILE_DEVICE_UNKNOWN,0x802,METHOD_BUFFERED,FILE_ANY_ACCESS)


// 만약 ioctl_sangjun -> write -> ioctl_sangjun2순으로 실행되면 crash가 발생하는 상황
// -> ioctl_sangju2n때 할당  --> write때 free --> ioctl_sangjun2때 또 free
int crash_sequence = 0;
typedef struct _DEVICE_EXTENSION {
	char Buffer[20];
	int DataSize;
}DEVICE_EXTENSION,*PDEVICE_EXTENSION;

void TargetUnload(PDRIVER_OBJECT pDrvObj)
{
	UNICODE_STRING SymName;
	RtlInitUnicodeString(&SymName, SYMNAME);

	IoDeleteSymbolicLink(&SymName);
	IoDeleteDevice(pDrvObj->DeviceObject);
}
NTSTATUS crashMe2(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	UNREFERENCED_PARAMETER(pIrp);
	PDEVICE_EXTENSION pDE;

	pDE = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	pIrp->IoStatus.Information = crash_sequence;
	char* buff = pDE->Buffer;
	if (buff[0] == 'A')
	{
		if (buff[1] == 'B')
		{
			if (buff[2] == 'C')
			{
				if (buff[3] == 'D')
				{
					if (buff[4] == 'E')
					{
						if (buff[5] == 'F')
						{
							if (buff[6] == 'G')
							{
								if (crash_sequence == 0)
									crash_sequence = 1;
								else
									crash_sequence = 0;
							}
						}
					}
				}
			}
		}
	}


	return STATUS_SUCCESS;
}


NTSTATUS crashMe3(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	UNREFERENCED_PARAMETER(pIrp);
	PDEVICE_EXTENSION pDE;

	pDE = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	pIrp->IoStatus.Information = crash_sequence;
	char* buff = pDE->Buffer;
	if (buff[0] == 'G')
	{
		if (buff[1] == 'F')
		{
			if (buff[2] == 'E')
			{
				if (buff[3] == 'D')
				{
					if (buff[4] == 'C')
					{
						if (buff[5] == 'B')
						{
							if (buff[6] == 'A')
							{
								if (crash_sequence == 1)
									crash_sequence = 2;
								else
									crash_sequence = 0;
							}
						}
					}
				}
			}
		}
	}


	return STATUS_SUCCESS;
}
NTSTATUS crashMe(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	UNREFERENCED_PARAMETER(pIrp);
	PDEVICE_EXTENSION pDE;

	pDE = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;

	char* buff = pDE->Buffer;

	pIrp->IoStatus.Information = crash_sequence;
	if (buff[0]=='S')
	{
		if (buff[1] == 'T')
		{
			if (buff[2] == 'A')
			{
				if (buff[3] == 'R')
				{
					if (buff[4] == 'T')
					{

						if (crash_sequence == 2)
							((VOID(*)())0x0)(); // crash
						else
							crash_sequence = 0;
					}
				}
			}
		}
	}
	
	return STATUS_SUCCESS;
}


NTSTATUS MyWriteDispatch(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	PIO_STACK_LOCATION pStack;
	int Length;
	unsigned char* pUserBuffer;
	DEVICE_EXTENSION* pDE;

	pStack = IoGetCurrentIrpStackLocation(pIrp);
	Length = pStack->Parameters.Write.Length;
	if (Length > 4) Length = 4;
	pUserBuffer = pIrp->UserBuffer;
	pDE = (DEVICE_EXTENSION*)pDevObj->DeviceExtension;
	memcpy(pDE->Buffer, pUserBuffer, Length);
	pDE->DataSize = Length;

	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = crash_sequence;
	if (pDE->Buffer[0] == 'T')
	{
		if (pDE->Buffer[1] == 'E')
			if (pDE->Buffer[2] == 'S')
				if (pDE->Buffer[3] == 'T')
					if (crash_sequence == 2)
						((VOID(*)())0x0)();
					else
						crash_sequence = 0;
				
						
	}

		

	IoCompleteRequest(pIrp, 0); // 0 -> IO_NO_INCREMENT
	return STATUS_SUCCESS;
}


NTSTATUS VulnFunction(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	PIO_STACK_LOCATION pStack;;
	//PVOID pSystemBuffer = NULL;

	ULONG InputBufferLength;
	ULONG OutputBufferLength;
	NTSTATUS ntStatus;
	PCHAR inBuf, outBuf;
	PDEVICE_EXTENSION pDE;

	pDE = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;

	ntStatus = STATUS_NOT_SUPPORTED;
	pStack = IoGetCurrentIrpStackLocation(pIrp);

	//UNREFERENCED_PARAMETER(pDevObj);
	PAGED_CODE();

	InputBufferLength = pStack->Parameters.DeviceIoControl.InputBufferLength;
	OutputBufferLength = pStack->Parameters.DeviceIoControl.OutputBufferLength;

	if (!OutputBufferLength)
	{
		ntStatus = STATUS_INVALID_PARAMETER;
		goto End;
	}

	if (OutputBufferLength >= 20)
	{
		ntStatus = STATUS_INVALID_PARAMETER;
		goto End;
	}
	if (InputBufferLength >= 20)
	{
		ntStatus = STATUS_INVALID_PARAMETER;
		goto End;
	}

	switch (pStack->Parameters.DeviceIoControl.IoControlCode)
	{
		case IOCTL_SAGNJUN:
			inBuf = pIrp->AssociatedIrp.SystemBuffer;
			outBuf = pIrp->AssociatedIrp.SystemBuffer;

			memcpy(pDE->Buffer, inBuf, InputBufferLength);
			crashMe(pDevObj,pIrp);

			ntStatus = STATUS_SUCCESS;
			break;
		case IOCTL_SAGNJUN2:
			inBuf = pIrp->AssociatedIrp.SystemBuffer;
			outBuf = pIrp->AssociatedIrp.SystemBuffer;


			memcpy(pDE->Buffer, inBuf, InputBufferLength);
			crashMe2(pDevObj, pIrp);

			ntStatus = STATUS_SUCCESS;
			break;

		case IOCTL_SAGNJUN3:
			inBuf = pIrp->AssociatedIrp.SystemBuffer;
			outBuf = pIrp->AssociatedIrp.SystemBuffer;
			


			if (InputBufferLength > 20)
				InputBufferLength = 20;

			memcpy(pDE->Buffer, inBuf, InputBufferLength);
			crashMe3(pDevObj, pIrp);


			ntStatus = STATUS_SUCCESS;
			break;
	default:
		ntStatus = STATUS_INVALID_DEVICE_REQUEST;
		break;
	}
End:
	pIrp->IoStatus.Status = ntStatus;
	IoCompleteRequest(pIrp, 0);
	return ntStatus;
}

NTSTATUS handleIrp(PDEVICE_OBJECT pDrvObj, PIRP pIrp)
{
	UNREFERENCED_PARAMETER(pDrvObj);
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest(pIrp, 0); // IO_NO_INCREMENT == 0
	return pIrp->IoStatus.Status;
}


NTSTATUS DriverEntry(PDRIVER_OBJECT pDrvObj, PUNICODE_STRING pRegPath)
{
	
	UNICODE_STRING DevName;
	UNICODE_STRING SymName;
	PDEVICE_OBJECT DevObj=NULL;
	NTSTATUS ntStatus;

	UNREFERENCED_PARAMETER(pRegPath);
	PAGED_CODE();

	RtlInitUnicodeString(&DevName, DEVNAME);
	RtlInitUnicodeString(&SymName, SYMNAME);

	// Device Object생성
	ntStatus = IoCreateDevice(
		pDrvObj,
		sizeof(DEVICE_EXTENSION),
		&DevName,
		FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN,
		FALSE,
		&DevObj
	);

	if (!NT_SUCCESS(ntStatus))
	{
		IoDeleteDevice(pDrvObj->DeviceObject);
		return ntStatus;
	}

	//DevObj->Flags |= DO_BUFFERED_IO;

	pDrvObj->MajorFunction[IRP_MJ_CREATE] = handleIrp;
	pDrvObj->MajorFunction[IRP_MJ_CLOSE] = handleIrp;
	pDrvObj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = VulnFunction;
	pDrvObj->MajorFunction[IRP_MJ_WRITE] = MyWriteDispatch;
	pDrvObj->DriverUnload = TargetUnload;

	ntStatus = IoCreateSymbolicLink(&SymName, &DevName);
	DbgPrint("[+] KAFL target drv -- loaded");
	return ntStatus;
}