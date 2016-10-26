#ifndef _FILE_OP
#define _FILE_OP

#include <fstream>
#include <sstream>
#include <stdlib.h>
using namespace std;

size_t file_load(string filename, char *&buffer)
{
    filebuf *pbuf;
    ifstream filestr;
    size_t size;
    filestr.open (filename.c_str(), ios::binary);
    pbuf = filestr.rdbuf();
    size = pbuf->pubseekoff(0,ios::end,ios::in);
    pbuf->pubseekpos(0,ios::in);
    buffer = new char[size+1];
    pbuf->sgetn(buffer,size);
    filestr.close();
    buffer[size] = '\0';
    return size;
}

size_t file_load_no_linesep(string filename, char *&buffer)
{
    filebuf *pbuf;
    ifstream filestr;
    size_t size;
    filestr.open (filename.c_str(), ios::binary);
    pbuf = filestr.rdbuf();
    size = pbuf->pubseekoff(0,ios::end,ios::in);
    pbuf->pubseekpos(0,ios::in);
    buffer = new char[size+1];
    pbuf->sgetn(buffer,size);
    filestr.close();
    buffer[size] = '\0';

    // remove line breaks
    size_t cur = 0;
    size_t new_size = size;
    for(size_t i = cur; i < size; i++)
    {
        if(((int)buffer[i])==10) // new line ascii 
        {
            new_size--;
            continue;
        } else
        {
            buffer[cur] = buffer[i];
            cur++;
        }
    }
    buffer[cur] = '\0';
    return new_size;
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
