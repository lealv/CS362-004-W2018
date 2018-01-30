#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
    //Printable characters are ASCII 32 through 126.
    char randomChar = (rand() % (126 - 32 + 1)) + 32; //return a number from 32 through 126).
    return randomChar;
}

char *inputString()
{
    char* charPool = "acegilmorstvx";
    int poolSize = 13;

    int stringSize = 6; //Fix the random string size to 6 (including null terminator).
    //char *randomString[stringSize];
    char *randomString = malloc(sizeof(char) * stringSize);

    int i;
    for(i = 0; i < stringSize - 1; i++)
    {
        int index = rand() % poolSize;
        randomString[i] = charPool[index];
    }
    randomString[stringSize - 1] = '\0';

    return randomString;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1; //ascii 91
    if (c == '(' && state == 1) state = 2; // ascii 40
    if (c == '{' && state == 2) state = 3; //ascii 123
    if (c == ' ' && state == 3) state = 4; //
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      free(s);
      exit(200);
    }
    free(s);
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
