#include <stdio.h>
#include "../include/fuctionality.h"

#include <stdio.h>

#define S_OK (0)
#define E_FAIL (-1)


int testOpen() {

    if (openDb() != 0)
        return E_FAIL;
    return S_OK;
}

int testLogin(){
    char* validLogin  = "mamin_bandit";
    char* validPassword = "12345678";

    if(login(validLogin, validPassword) != 0){
        return E_FAIL;
    }
    char* invalidPassword = "123456789";

    if(login(validLogin, invalidPassword) == 0){
        return E_FAIL;
    }
    return S_OK;
}


int run_tests() {
    if (E_FAIL == testOpen()) {
        printf("failed testOpen()\n");
        return E_FAIL;
    }
    if (E_FAIL == testLogin()) {
        printf("failed testLogin()\n");
        return E_FAIL;
    }
    return S_OK;
}


/*
	This main function only runs all the test code.
    If successful it returns S_OK which is equal to the numerical value of 0.
 	Any other value is considered a failure.

 */
int main()
{
    int result;

    printf("Running Example tests .. \n");
    result = run_tests();

    if (result == S_OK)
        printf ("tests passed.\n");
    else
        printf ("tests failed.\n");

    return result;  /* remember the value 0 is considered passing in a travis-ci sense */

}