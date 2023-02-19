
#include <system/exe/parent.hpp>
#include <iostream>
#include <stdio.h>

int main(int a_argc, char* a_argv[])
{
    //char vName[]={'n','e','d','i','t',0};
    //char* vArgv[2];
    //vArgv[0] = vName;
    //vArgv[1] = nullptr;

#ifndef _WIN32
    printf("Press any key then enter to proceed! "); fflush(stdout);
    getchar();
#endif

    if(a_argc<2){
        ::std::cerr<< "provide exe name to run\n";
        return 1;
    }

    systemN::exe::parent::THandle aHandle = systemN::exe::parent::RunNoWaitU(0,0,a_argv+1,CPPUTILS_NULL,1);

    if(!aHandle){
        ::std::cerr<< "Unamble to create\n";
        return 1;
    }

    int nRet;
    systemN::exe::parent::WaitAndClear(aHandle,-1,&nRet);

    ::std::cout << "return code from exe: "<<nRet<< ::std::endl;

    return 0;
}
