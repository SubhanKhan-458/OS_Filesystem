#include <stdio.h>
#include <string.h>

void splitter(int size, char str[])
{
    int i = 0;
    char *array[10];
    char *ptr = strtok(str, "/");
    array[i] = ptr;

	while(ptr != NULL)
	{
        i++;
		printf("'%s'\n", ptr);
		ptr = strtok(NULL, "/");
        array[i] = ptr; 
	}
    //Checking 
    // int j = 0;
    // while(j != 6)
	// {
	// 	printf("'%s'\n", array[j++]);
	// }
}

int main()
{
	char str[] = "Home/k191285/desktop/folder/files/hello.txt";
	int init_size = strlen(str);

    splitter(init_size, str);
	
	return 0;
}