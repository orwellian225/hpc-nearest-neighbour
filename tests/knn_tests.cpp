#define CATCH_CONFIG_MAIN

#include <vector>
#include <tuple>

#include <catch.hpp>

#include "VecN.hpp"
#include "serial.hpp"

TEST_CASE("Serial QuickSort", "[serial][qsort]") {
    std::vector<std::pair<float, VecN*>> presorted {
        { 1.0, nullptr }, { 2.0, nullptr },
        { 3.0, nullptr }, { 4.0, nullptr }, { 5.0, nullptr }
    };
    std::vector<std::pair<float, VecN*>> reversed {
        { 5.0, nullptr }, { 4.0, nullptr },
        { 3.0, nullptr }, { 2.0, nullptr }, { 1.0, nullptr }
    };
    
    REQUIRE( presorted.size() == 5 );
    REQUIRE( reversed.size() == 5 );
    REQUIRE( presorted.capacity() >= 5 );
    REQUIRE( reversed.capacity() >= 5 );
   
    SECTION("Partition Indices") {
        size_t presorted_full_pivot = serial::partition(presorted, 0, presorted.size() - 1);

        size_t presorted_lefthalf_pivot = serial::partition(presorted, 0, 1);
        size_t presorted_righthalf_pivot = serial::partition(presorted, 3, 4);

        REQUIRE(presorted_full_pivot == 0);

        REQUIRE(presorted_lefthalf_pivot == 0);
        REQUIRE(presorted_righthalf_pivot == 3);
    }

    SECTION("Partition Elements") {
        size_t presorted_full_pivot = serial::partition(presorted, 0, presorted.size() - 1);
        size_t reversed_full_pivot = serial::partition(reversed, 0, reversed.size() - 1);

        for (size_t i = 1; i < 5; ++i)
            REQUIRE(presorted[i].first > presorted[0].first);

        for (size_t i = 0; i < 5; ++i) {
            INFO("Current i = " << i);
            if (i < 2)
                REQUIRE(reversed[i].first < reversed[2].first);
            else if (i > 2)
                REQUIRE(reversed[i].first > reversed[2].first);
            else
                continue;
        }
    }
}

TEST_CASE("NearestNeighbour", "[knn]") {}