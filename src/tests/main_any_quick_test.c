//
// file:			main_any_quick_test.c
// path:			src/tests/main_any_quick_test.c
// created on:		2023 May 08
// created by:		Davit Kalantaryan (davit.kalantaryan@gmail.com)
//


#include <cinternal/disable_compiler_warnings.h>
#include <stdio.h>
#include <cinternal/undisable_compiler_warnings.h>


#ifdef _MSC_VER
#pragma warning (disable:4710)  //  'printf': function not inlined
#endif


int main(void)
{
    printf("Hello world!\n");

	return 0;
}
