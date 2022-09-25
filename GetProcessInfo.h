#ifndef GET_PROCESS_INFO_H
#define GET_PROCESS_INFO_H

#include <sys/uio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int getProcIdByName(string procName)
{
    int pid = -1;

    // Open the /proc directory
    DIR *dp = opendir("/proc");
    if (dp != NULL)
    {
        // Enumerate all entries in directory until process found
        struct dirent *dirp;
        while (pid < 0 && (dirp = readdir(dp)))
        {
            // Skip non-numeric entries
            int id = atoi(dirp->d_name);
            if (id > 0)
            {
                // Read contents of virtual /proc/{pid}/cmdline file
                string cmdPath = string("/proc/") + dirp->d_name + "/cmdline";
                ifstream cmdFile(cmdPath.c_str());
                string cmdLine;
                getline(cmdFile, cmdLine);
                if (!cmdLine.empty())
                {
                    // Keep first cmdline item which contains the program path
                    size_t pos = cmdLine.find('\0');
                    if (pos != string::npos)
                        cmdLine = cmdLine.substr(0, pos);
                    // Keep program name only, removing the path
                    pos = cmdLine.rfind('/');
                    if (pos != string::npos)
                        cmdLine = cmdLine.substr(pos + 1);
                    // Compare against requested process name
                    if (procName == cmdLine)
                        pid = id;
                }
            }
        }
    }
    closedir(dp);
    return pid;
}

long FromHex(string in) {
	stringstream ss;
	long out;
	ss << hex << in ;
	ss >> out;
	return out;
}

long GetMiscAddress(int _pid) {
	string pidStr = to_string(_pid);
	ifstream ifs;
	ifs.open("/proc/"+pidStr+"/maps", ifstream::in);
	long _miscAddr = 0x00;
	bool isLib = false;
	for ( string line; getline(ifs, line); ) {
		if ( line.find("/cores/") == string::npos && isLib) {
			string miscAddrStr = line.substr(0,12);
			cout << miscAddrStr << "\n";  
			_miscAddr = FromHex(miscAddrStr);
			break; }
		if ( line.find("/cores/") != string::npos )
			isLib = true;
	}
	ifs.close();
	if (!isLib) return -1;
	else return _miscAddr;
}

long GetHeapAddress(int _pid) {
	string pidStr = to_string(_pid);
	ifstream ifs;
	ifs.open("/proc/"+pidStr+"/maps", ifstream::in);
	long _heapAddr = 0x00;
	for ( string line; getline(ifs, line); ) {
		if ( line.find("[heap]") != string::npos ) {
			string heapAddrStr = line.substr(0,12);
			cout << heapAddrStr << "\n";  
			_heapAddr = FromHex(heapAddrStr);
			break; }
	}
	ifs.close();
	return _heapAddr;
}
iovec CreateRemoteBuffer (long addr, int buf) {
	void *ptr = (void *)addr;
    struct iovec _remote[1]; 
	_remote[0].iov_base = ptr; 
	_remote[0].iov_len = buf;
	return *_remote;
}
iovec CreateReturnBuffer (int buf) {
    struct iovec _local[1]; 
	_local[0].iov_base = calloc(buf, sizeof(char)); 
	_local[0].iov_len = buf;
	return *_local;
}

int ReadBuffer(pid_t _pid, iovec* local, iovec* remote) {
	// Call process_vm_readv - handle any error codes
    ssize_t nread = process_vm_readv(_pid, local, 2, remote, 1, 0);
    if (nread < 0) {
        switch (errno) {
            case EINVAL:
              printf("ERROR: INVALID ARGUMENTS.\n");
              break;
            case EFAULT:
              printf("ERROR: UNABLE TO ACCESS TARGET MEMORY ADDRESS.\n");
              break;
            case ENOMEM:
              printf("ERROR: UNABLE TO ALLOCATE MEMORY.\n");
              break;
            case EPERM:
              printf("ERROR: INSUFFICIENT PRIVILEGES TO TARGET PROCESS.\n");
              break;
            case ESRCH:
              printf("ERROR: PROCESS DOES NOT EXIST.\n");
              break;
            default:
              printf("ERROR: AN UNKNOWN ERROR HAS OCCURRED.\n");
        }

        return -1;
    }
    return *((int *)local[0].iov_base);
}

unsigned char* ReadFullBuffer(pid_t _pid, iovec* local, iovec* remote) {
	// Call process_vm_readv - handle any error codes
    ssize_t nread = process_vm_readv(_pid, local, 2, remote, 1, 0);
    if (nread < 0) {
        switch (errno) {
            case EINVAL:
              printf("ERROR: INVALID ARGUMENTS.\n");
              break;
            case EFAULT:
              printf("ERROR: UNABLE TO ACCESS TARGET MEMORY ADDRESS.\n");
              break;
            case ENOMEM:
              printf("ERROR: UNABLE TO ALLOCATE MEMORY.\n");
              break;
            case EPERM:
              printf("ERROR: INSUFFICIENT PRIVILEGES TO TARGET PROCESS.\n");
              break;
            case ESRCH:
              printf("ERROR: PROCESS DOES NOT EXIST.\n");
              break;
            default:
              printf("ERROR: AN UNKNOWN ERROR HAS OCCURRED.\n");
        }
    }
    return (unsigned char*)local[0].iov_base;
}
#endif //GET_PROCESS_INFO_H