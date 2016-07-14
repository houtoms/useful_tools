#ifndef _FILE_OP
#define _FILE_OP

#include <fstream>
#include <sstream>
#include <stdlib.h>
using namespace std;

long file_load(string filename, char *&buffer)
{
    filebuf *pbuf;
    ifstream filestr;
    long size;
    filestr.open (filename.c_str(), ios::binary);
    pbuf = filestr.rdbuf();
    size = pbuf->pubseekoff(0,ios::end,ios::in);
    pbuf->pubseekpos(0,ios::in);
    buffer = new char[size];
    pbuf->sgetn(buffer,size);
    filestr.close();
    return size;
}

void analyze_file(char *buf, long size, int *&seg_start, int *&seg_length, int &seg_num)
{
    buf[size-1] = '\0';
    string buf_str(buf);

    // fetch the number of segments
    string target = "# Segments number: ";
    size_t off     = buf_str.find(target);
    size_t off_end = buf_str.find("\n", off);
    off += target.length();
    string seg_num_str = buf_str.substr(off, off_end-off);
    seg_num = atoi(seg_num_str.c_str());

    seg_start  = new int[seg_num];
    seg_length = new int[seg_num];

    // skip all the comments
    target = "# Segments in format (start_point, length):";
    off = buf_str.find(target);
    off += target.length();

    stringstream stm;
    stm.str(buf_str.substr(off));
    string dummy; // comma put here
    for(int i = 0; i < seg_num; i++)
    {
        stm >> seg_start[i] >> dummy >> seg_length[i];
    }
}

#endif