#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

using namespace std;

void error(string msg)
{
    cout<<msg;
    exit(1);
}

int main()
{
    ifstream fp;
    fp.open("file.txt");
    char c;
    int a[26] = {0};
    int charcount = 0;
    // counting freq of each char for encoding
    while (!fp.eof())
    {
        fp.get(c);
        if (fp.eof())
            break;
        int asciival = c; 
        asciival = (asciival - 65);
        a[asciival]++;
        charcount++;
    }

    string present = "";
    // put characters in ascending order in string
    for (int i = 0; i < 26; i++)
    {
        if (a[i] != 0)
        {
            char ch = i + 65;
            present += ch;
        }
    }
    fp.close();

    // no of bits req to represent our characters in encoding
    int bit = ceil(log2((float)present.length()));
  
    fp.open("file.txt");
    int encoded[900] = {0};
    int v = 0, ccc = 0;

    // convert the characters into corressponding bits
    while (!fp.eof())
    {
        fp.get(c);
        if (fp.eof())
            break;
        int pos = present.find(c);
        if (pos != -1)
            for (int i = bit - 1; i >= 0; i--)
            {
                int k = pos >> i;
                if (k & 1)
                    encoded[v++] = 1;
                else
                    encoded[v++] = 0;
            }
    }
    fp.close();

    // creating conversion Table
    int bytes = v / 8;
    if (v % 8 != 0)
        bytes++;
    char *conversionTable = new char[bytes]; //((8*ll/l)
    int i;
    for (i = 0; i < bytes; i++)
    {
        conversionTable[i] = '\0';
    }
    for (i = 0; i < v; i++)
    {
        if (encoded[i])
        {
            conversionTable[i / 8] |= (1 << (i % 8));
        }
    }
    string presentWithQty = to_string(v) + present;
   
    int c_sock;
    c_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (c_sock < 0)
    {
        error("Error on socket opening");
    }
    struct sockaddr_in client;
    memset(&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons(9000);
    client.sin_addr.s_addr = INADDR_ANY;
    if (connect(c_sock, (struct sockaddr *)&client, sizeof(client)) == -1)
    {
        free(conversionTable);
        error("error in connection");
    }
    char buf1[presentWithQty.length()];
    strcpy(buf1, presentWithQty.c_str());
    send(c_sock, buf1, sizeof(buf1), 0);
    send(c_sock, conversionTable, bytes * sizeof(char), 0);
    cout << "File succesfully transfered to server " << endl;
    close(c_sock);

    delete[] conversionTable;
    return 0;
}