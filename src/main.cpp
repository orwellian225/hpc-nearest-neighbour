#include <stdlib.h>
#include <stdint.h>
#include <chrono>

#include <fmt/core.h>
#include <vector>

#include "VecN.hpp"
#include "query.hpp"
#include "serial.hpp"
#include "sections.hpp"

int main(int argc, char **argv) {

    fmt::println("COMS4040A High Performance Computing Assignment 1");
    fmt::println("Brendan Griffiths - 2426284");
    fmt::println("Determine the K-Nearest-Neighbours for synthetic d-dimensional data");
    fmt::println("{:-<80}", "-");

    if (argc != 4) {
        fmt::println(stderr, "Incorrect arguments - please specify the dimensions of a vector, the number of points and the number of queries");
        return -1;
    }

    uint32_t dimensions = atoi(argv[1]);
    uint32_t num_points = atoi(argv[2]);
    uint32_t num_queries = atoi(argv[3]);
    constexpr uint32_t num_points_per_queries = 2;

    fmt::println("Problem Properties");
    fmt::println("------------------");
    fmt::println("\t{:<32} {}", "Vector dimensionality:", dimensions);
    fmt::println("\t{:<32} {}", "Number of points:", num_points);
    fmt::println("\t{:<32} {}", "Number of queries:", num_queries);
    fmt::println("\t{:<32} {}", "K = ", num_points_per_queries);
    fmt::println("");

    std::vector<VecN> points;
    std::vector<Query> queries, serial_q, section_q, task_q;

    for (size_t i = 0; i < num_points; ++i) {
        VecN new_point(dimensions, 1.0);
        new_point.randomize();
        points.push_back(new_point);
    }

    for (size_t i = 0; i < num_points; ++i) {
        VecN new_query_point(dimensions, 1.0);
        new_query_point.randomize();
        Query new_query;
        new_query.query = new_query_point;
        queries.push_back(new_query);
    }
    serial_q = std::vector<Query>(queries);
    section_q = std::vector<Query>(queries);
    task_q = std::vector<Query>(queries);

    bool serial_correct = false;
    bool sections_correct = false;
    bool tasks_correct = false;
    float serial_duration_ms = 0.0, sections_duration_ms = 0.0, tasks_duration_ms = 0.0;

    auto serial_start = std::chrono::high_resolution_clock::now();
        serial::determine_queries(serial_q, points, num_points_per_queries);
    auto serial_end = std::chrono::high_resolution_clock::now();
    serial_duration_ms = std::chrono::duration_cast<std::chrono::microseconds>(serial_end - serial_start).count() / 1000.;

    auto section_start = std::chrono::high_resolution_clock::now();
        sections::determine_queries(section_q, points, num_points_per_queries);
    auto sections_end = std::chrono::high_resolution_clock::now();
    sections_duration_ms = std::chrono::duration_cast<std::chrono::microseconds>(sections_end - section_start).count() / 1000.;

    auto task_start = std::chrono::high_resolution_clock::now();
        sections::determine_queries(task_q, points, num_points_per_queries);
    auto task_end = std::chrono::high_resolution_clock::now();
    tasks_duration_ms = std::chrono::duration_cast<std::chrono::microseconds>(task_end - task_start).count() / 1000.;

    size_t incorrect_query_idx = -1;

    serial_correct = true;
    for (size_t i = 0; i < queries.size(); ++i) {
        for (size_t j = 0; j < num_points_per_queries - 1; ++j) {
            if (serial_q[i].points[j].distance > serial_q[i].points[j + 1].distance) {
                fmt::println("{} > {}", queries[i].points[j].distance, queries[i].points[j + 1].distance);
                serial_correct = false;
                incorrect_query_idx = i;
                break;
            }
        }
        
        if (!serial_correct)
            break;
    }

    sections_correct = true;
    for (size_t i = 0; i < queries.size(); ++i) {
        for (size_t j = 0; j < num_points_per_queries; ++j) {
            if (serial_q[i].points[j].distance != section_q[i].points[j].distance) {
                sections_correct = false;
                break;
            }
        }
        
        if (!sections_correct)
            break;
    }

    tasks_correct = true;
    for (size_t i = 0; i < queries.size(); ++i) {
        for (size_t j = 0; j < num_points_per_queries; ++j) {
            if (serial_q[i].points[j].distance != section_q[i].points[j].distance) {
                tasks_correct = false;
                break;
            }
        }
        
        if (!tasks_correct)
            break;
    }

    fmt::println("{:-<70}", "-");
    fmt::println("| {:<24} | {:<19} | {:<7} | {:<7} |",
        "Algorithm",
        "Execution Time (ms)",
        "Speedup",
        "Correct"
    );

    fmt::println("{:-<70}", "-");
    fmt::println("| {:<24} | {:^19.5f} | {:^7} | {:^7} |",
        "Serial",
        serial_duration_ms,
        "/",
        serial_correct ? "Yes" : "No"
    );

    fmt::println("| {:<24} | {:^19.5f} | {:^7.5f} | {:^7} |",
        "Sections",
        sections_duration_ms,
        serial_duration_ms / sections_duration_ms,
        sections_correct ? "Yes" : "No"
    );

    fmt::println("| {:<24} | {:^19.5f} | {:^7.5f} | {:^7} |",
        "Tasks",
        tasks_duration_ms,
        serial_duration_ms / tasks_duration_ms,
        tasks_correct ? "Yes" : "No"
    );

    fmt::println("{:-<70}", "-");

    return 0;
}
