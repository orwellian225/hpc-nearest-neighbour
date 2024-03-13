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
    std::vector<std::pair<float, VecN*>> random {
        { 2.0, nullptr }, { 6.0, nullptr },
        { 4.0, nullptr }, { 8.0, nullptr }, { 5.0, nullptr }
    };
    
    REQUIRE( presorted.size() == 5 );
    REQUIRE( reversed.size() == 5 );
    REQUIRE( presorted.capacity() >= 5 );
    REQUIRE( reversed.capacity() >= 5 );

    SECTION("Partial Partition Indices") {
        size_t presorted_lefthalf_pivot = serial::partition(presorted, 0, 1);
        size_t presorted_righthalf_pivot = serial::partition(presorted, 3, 4);
        REQUIRE(presorted_lefthalf_pivot == 1);
        REQUIRE(presorted_righthalf_pivot == 4);

        size_t reversed_lefthalf_pivot = serial::partition(reversed, 0, 1);
        size_t reversed_righthalf_pivot = serial::partition(reversed, 3, 4);
        REQUIRE(reversed_lefthalf_pivot == 0);
        REQUIRE(reversed_righthalf_pivot == 3);

        size_t random_lefthalf_pivot = serial::partition(random, 0, 1);
        size_t random_righthalf_pivot = serial::partition(random, 3, 4);
        REQUIRE(random_lefthalf_pivot == 1);
        REQUIRE(random_righthalf_pivot == 3);
    }
   
    SECTION("Full Partition Indices") {
        size_t presorted_full_pivot = serial::partition(presorted, 0, presorted.size() - 1);
        size_t reversed_full_pivot = serial::partition(reversed, 0, reversed.size() - 1);
        size_t random_full_pivot = serial::partition(random, 0, random.size() - 1);


        REQUIRE(presorted_full_pivot == 4);
        REQUIRE(reversed_full_pivot == 0);
        REQUIRE(random_full_pivot == 2);
    }

    SECTION("Partition Elements") {
        size_t presorted_full_pivot = serial::partition(presorted, 0, presorted.size() - 1);
        size_t reversed_full_pivot = serial::partition(reversed, 0, reversed.size() - 1);
        size_t random_full_pivot = serial::partition(random, 0, random.size() - 1);

        for (size_t i = 0; i < 5; ++i) {
            INFO("Current i = " << i);
            if (i < presorted_full_pivot)
                REQUIRE(presorted[i].first < presorted[presorted_full_pivot].first);
            else if (i > presorted_full_pivot)
                REQUIRE(presorted[i].first >= presorted[presorted_full_pivot].first);
            else
                continue;
        }

        for (size_t i = 0; i < 5; ++i) {
            INFO("Current i = " << i);
            if (i < reversed_full_pivot)
                REQUIRE(reversed[i].first < reversed[reversed_full_pivot].first);
            else if (i > reversed_full_pivot)
                REQUIRE(reversed[i].first > reversed[reversed_full_pivot].first);
            else
                continue;
        }

        for (size_t i = 0; i < 5; ++i) {
            INFO("Current i = " << i);
            if (i < random_full_pivot)
                REQUIRE(random[i].first < random[random_full_pivot].first);
            else if (i > random_full_pivot)
                REQUIRE(random[i].first >= presorted[random_full_pivot].first);
            else
                continue;
        }
    }

    SECTION("Quicksort Elements") {
        serial::quicksort(presorted, 0, 4);
        serial::quicksort(reversed, 0, 4);
        serial::quicksort(random, 0, 4);

        for (size_t i = 0; i < 4; ++i) {
            INFO("Current i = " << i);
            REQUIRE(presorted[i] <= presorted[i + 1]);
        }

        for (size_t i = 0; i < 4; ++i) {
            INFO("Current i = " << i);
            REQUIRE(reversed[i] <= reversed[i + 1]);
        }

        for (size_t i = 0; i < 4; ++i) {
            INFO("Current i = " << i);
            REQUIRE(random[i] <= random[i + 1]);
        }
    }
}

TEST_CASE("Serial NearestNeighbour", "[serial][knn]") {
    VecN **points_data = new VecN*[2];
    points_data[0] = new VecN(1, 0.);
        (*points_data[0])[0] = 5.;
    points_data[1] = new VecN(1, 0.);
        (*points_data[1])[0] = 5.;

    VecN **queries_data = new VecN*[1];
    queries_data[0] = new VecN(1, 0.);
        (*queries_data[0])[0] = 1.;

    VecNArray points(points_data, 2);
    VecNArray queries(queries_data, 1);

    SECTION("Nearest Neighbours K = 1") {
        std::vector<QueryPoints> neighbours = serial::nearest_neigbours(points, queries, 1);

        REQUIRE(neighbours[0].points.size() == 1);
        REQUIRE((*neighbours[0].points[0])[0] == 5.);
    }

    delete[] points_data, queries_data;

}