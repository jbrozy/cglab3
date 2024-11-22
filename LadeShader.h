#pragma once

#include <stdio.h> 
#include <stdlib.h>



char* readTextFileIntoString(const char* filename)
{
	printf("Reading shader %s\n", filename);
	char* buf = 0;
	long length;
	FILE* f;
	int i = fopen_s(&f, filename, "rb");

	if (f)
	{
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		buf = new char[length + 1];//End of String needs space
		if (buf)
		{
			fread(buf, 1, length, f);
		}
		fclose(f);
		buf[length] = 0;
	}
	else
	{
		printf("File not found!\n");

		return 0;
	}
	return buf;
}
