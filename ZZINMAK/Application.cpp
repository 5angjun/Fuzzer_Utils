#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>

#define IOCTL_SAGNJUN CTL_CODE(FILE_DEVICE_UNKNOWN,0x800,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAGNJUN2 CTL_CODE(FILE_DEVICE_UNKNOWN,0x801,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_SAGNJUN3 CTL_CODE(FILE_DEVICE_UNKNOWN,0x802,METHOD_BUFFERED,FILE_ANY_ACCESS)

int main()
{
	HANDLE handle = (HANDLE)-1;
	BOOL bRet = FALSE;
	DWORD dwRet = 0;

	CHAR inbuff1[] = "ABCDEFG";
	CHAR inbuff2[] = "GFEDCBA";
	CHAR inbuff3[] = "TEST";
	CHAR inbuff4[] = "SSSSS";
	CHAR inbuff[20] = { 0, };
	CHAR outbuff[20] = { 0, };

	//scanf("%s", inbuff);

	handle = CreateFile(L"\\??\\TARGETDRIVER",
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (handle == (HANDLE)INVALID_HANDLE_VALUE)
	{
		printf("Open failed\n");
		goto exit;
	}
		


	bRet = DeviceIoControl(
		handle,
		IOCTL_SAGNJUN2,
		&inbuff1,
		8,
		&outbuff,
		sizeof(outbuff)-1,
		&dwRet,
		NULL
	);
	printf("SNAGJUN2 dwRet is %d\n", dwRet);


	bRet = DeviceIoControl(
		handle,
		IOCTL_SAGNJUN3,
		&inbuff2,
		8,
		&outbuff,
		sizeof(outbuff) - 1,
		&dwRet,
		NULL
	);
	printf("SNAGJUN3 dwRet is %d\n", dwRet);

	bRet = DeviceIoControl(
		handle,
		IOCTL_SAGNJUN,
		&inbuff4,
		8,
		&outbuff,
		sizeof(outbuff) - 1,
		&dwRet,
		NULL
	);
	printf("SNAGJUN dwRet is %d\n", dwRet);
	//bRet = WriteFile(
	//	handle,
	//	"AAAAA",
	//	5,
	//	&dwRet,
	//	NULL
	//);

	////printf("outputBuffer is %s\n", outbuff);
	//printf("WriteFile dwRet is %d\n", dwRet);
	CloseHandle(handle);
exit:
	return 0;
}