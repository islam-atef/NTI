#include <stdio.h>
#include <stdlib.h>


void messageEncrypt(char *arr, char key);
void messageDecrypt(char *arr, char key);

#define Encrypt 0
#define Decrypt 1

int main() {

  char key = 0;
  char process = 0;
  char Arr[2000];
  while(1)
  {
    printf("Enter the process [Encrypt -> 0],[Decrypt -> 1] ,[End -> 2] : ");
    scanf("%d",&process);
    switch(process)
    {
    case Encrypt:
        printf("Enter the Key value :");
        scanf("%d",&key);
    printf("Enter the Message :");
        scanf("%s",Arr);
        messageEncrypt(Arr,key);
        break;
    case Decrypt:
        printf("Enter the Key value :");
        scanf("%d",&key);
        printf("Enter the Message :");
        scanf("%s",Arr);
        messageDecrypt(Arr,key);
        break;
    case 2:
        return 0;
        break;
    default:
        break;
    }
    printf("the Message : %s\n",Arr);
  }
  return 0;
}





void messageEncrypt(char *arr, char key)
{
    char counter = 0;
    char buffer_F = 0;
    char Flag = 0;
    if(key > 26)
    {
        key %= 26;

    }
    char KEY = key;
    while(arr[counter] != '\0')
    {
        if( Flag )
        {
            key = KEY;
            Flag = 0;
        }
        if(arr[counter] > 'Z')
        {
            arr[counter] -= 32;
            buffer_F = 1;
        }
        if((arr[counter] + key)>'Z')
        {

            if( !Flag )
            {
               key -= (('Z'-arr[counter])+1);
               Flag = 1;
            }
            arr[counter] = 'A';
        }
        arr[counter] += key;
        if(buffer_F)
        {
            arr[counter] += 32;
            buffer_F = 0;
        }
        counter++;
    }
}


void messageDecrypt(char *arr , char key)
{
    char counter = 0;
    char buffer_F = 0;
    char Flag = 0;
    if(key > 26)
    {
        key %= 26;
    }
    char KEY = key;
    while(arr[counter] != '\0')
    {
        if( Flag )
        {
            key = KEY;
            Flag = 0;
        }
        if(arr[counter] > 'Z')
        {
            arr[counter] -= 32;
            buffer_F = 1;
        }
        if((arr[counter] - key)<'A')
        {
            if( !Flag )
            {
               key -= (arr[counter] - 'A')+1;
               Flag = 1;
            }
            arr[counter] = 'Z';
        }
        arr[counter] -= key;
        if(buffer_F)
        {
            arr[counter] += 32;
            buffer_F = 0;
        }
        counter++;
    }
}
