#include <cstdlib>
#include <math.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <bits/stdc++.h>

using namespace std;
void error(string msg)
{
    cout<<msg;
    exit(1);
}

int main()
{
    int c_sock, s_sock;
    char msg[750];
    s_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (s_sock < 0)
    {
        error("Error opening socket");
    }
    struct sockaddr_in server, other;
    memset(&server, 0, sizeof(server));
    memset(&other, 0, sizeof(other));
    server.sin_family = AF_INET;
    server.sin_port = htons(9000);
    server.sin_addr.s_addr = INADDR_ANY;
    socklen_t add;
    if (bind(s_sock, (struct sockaddr *)&server, sizeof(server)))
    {
        error("Binding Failed");
    }
    listen(s_sock, 10);
    add = sizeof(other);
    c_sock = accept(s_sock, (struct sockaddr *)&other, &add);
    if (c_sock < 0)
    {
        error("Error on accept");
    }
    recv(c_sock, msg, sizeof(msg), 0);
    int i = 0;
    // message is of format numOfBits+Present Characters
    // we extract number of bits
    while (msg[i] < 65 || msg[i] > 90)
        i++;
    string msg2(msg);
    int istr = i;
    string temp = msg2.substr(0, i);
    // v is number of bits 
    int v = stoi(temp);

    // no of bits req to represent
    int bit = msg2.length() - i;
    bit = ceil(log2((float)bit));
    int bytes = v / 8;
    if (v % 8 != 0)
        bytes++;
    char *buf = new char[bytes];
    recv(c_sock, buf, bytes * sizeof(char), 0);
    int s = strlen(buf);

    // convert to encoded to original
    int *encodedValues = new int[v];
    for (int i = 0; i < v; i++)
    {
        encodedValues[i] = 0;
    }

    for (int i = 0; i < v; i++)
    {
        encodedValues[i] = ((buf[i / 8] & (1 << (i % 8))) != 0);
    }
   
    string outp = "";
    for (i = 0; i < v;)
    {

        int sum = 0;
        for (int j = (bit - 1); j >= 0; j--)
        {
            sum += (pow(2, j) * encodedValues[i++]);
        }
        char ch = msg2[istr + sum];

        outp += ch;
    }

    cout << endl;

    char outt[outp.length()];
    strcpy(outt, outp.c_str());
    ofstream fp;
    fp.open("received.txt");
    fp << outt;
    fp.close();

    cout<<"Data received from client and saved to file received.txt\n";

    close(s_sock);

    delete[] encodedValues;

    return 0;
}