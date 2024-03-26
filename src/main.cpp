#include <stdlib.h>
#include <stdint.h>
#include <chrono>
#include <array>
#include <fmt/core.h>
#include <vector>

#include "VecN.hpp"
#include "Times.hpp"
#include "query.hpp"
#include "serial.hpp"
#include "sections.hpp"
#include "tasks.hpp"

int main(int argc, char **argv) {

    fmt::println("COMS4040A High Performance Computing Assignment 1");
    fmt::println("Brendan Griffiths - 2426284");
    fmt::println("Determine the K-Nearest-Neighbours for synthetic d-dimensional data");
    fmt::println("{:-<80}", "-");

    std::pair<size_t,size_t> problems[9] = {
        {64, 10000}, {64, 100000}, {64, 1000000},
        {128, 10000}, {128, 100000}, {128, 1000000},
        {256, 10000}, {256, 100000}, {256, 1000000}
    }; 
    for (size_t problem_i = 0; problem_i < 9; ++problem_i) {
        uint32_t dimensions = problems[problem_i].first;
        uint32_t num_points = problems[problem_i].second;
        uint32_t num_queries = 10000;
        constexpr uint32_t num_points_per_queries = 10;

        for (size_t try_i = 0; try_i < 3; ++try_i) {

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
            serial_q = queries;
            section_q = queries;
            task_q = queries;

            bool serial_correct = false;
            bool sections_correct = false;
            bool tasks_correct = false;
            float serial_duration_ms = 0.0, sections_duration_ms = 0.0, tasks_duration_ms = 0.0;

            Times serial_breakdown = { 0 }, sections_breakdown = { 0 }, tasks_breakdwon = { 0 };

            auto serial_start = std::chrono::high_resolution_clock::now();
                serial::determine_queries(serial_q, points, num_points_per_queries, &serial_breakdown);
            auto serial_end = std::chrono::high_resolution_clock::now();
            serial_duration_ms = std::chrono::duration_cast<std::chrono::microseconds>(serial_end - serial_start).count() / 1000.;

            auto section_start = std::chrono::high_resolution_clock::now();
                sections::determine_queries(section_q, points, num_points_per_queries, &sections_breakdown);
            auto sections_end = std::chrono::high_resolution_clock::now();
            sections_duration_ms = std::chrono::duration_cast<std::chrono::microseconds>(sections_end - section_start).count() / 1000.;

            auto task_start = std::chrono::high_resolution_clock::now();
                tasks::determine_queries(task_q, points, num_points_per_queries, &tasks_breakdwon);
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

            fmt::println("{:-<46}", "-");
            fmt::println("| {:<32} | Attempt |", "Problem (d,m,n)");
            fmt::println("| {:^32} | {:^7} |", fmt::format("({}, {}, {})", dimensions, num_points, num_queries), try_i + 1);
            fmt::println("{:-<133}", "-");
            fmt::println("| {:<24} | {:<19} | {:<29} | {:<28} | {:<7} | {:<7} |",
                "Algorithm",
                "Execution Time (ms)",
                "Distances Execution Time (ms)",
                "Sorting Exectution Time (ms)",
                "Speedup",
                "Correct"
            );

            fmt::println("{:-<133}", "-");
            fmt::println("| {:<24} | {:^19.5f} | {:^29.5f} | {:^28.5f} | {:^7} | {:^7} |",
                "Serial",
                serial_duration_ms,
                serial_breakdown.distance_time_ms,
                serial_breakdown.sort_time_ms,
                "/",
                serial_correct ? "Yes" : "No"
            );

            fmt::println("| {:<24} | {:^19.5f} | {:^29.5f} | {:^28.5f} | {:^7.5f} | {:^7} |",
                "Sections",
                sections_duration_ms,
                sections_breakdown.distance_time_ms,
                sections_breakdown.sort_time_ms,
                serial_duration_ms / sections_duration_ms,
                sections_correct ? "Yes" : "No"
            );

            fmt::println("| {:<24} | {:^19.5f} | {:^29.5f} | {:^28.5f} | {:^7.5f} | {:^7} |",
                "Tasks",
                tasks_duration_ms,
                tasks_breakdwon.distance_time_ms,
                tasks_breakdwon.sort_time_ms,
                serial_duration_ms / tasks_duration_ms,
                tasks_correct ? "Yes" : "No"
            );

            fmt::println("{:-<133}", "-");
        }
    }

    return 0;
}
