#include <stdio.h>

#include <vm.h>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>


TEST_CASE( "Running", "[test]" ) {
    puts("JieCardVM is running!");
    vm_init();
}

