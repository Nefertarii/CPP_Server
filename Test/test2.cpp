#include <functional>
#include <iostream>
#include <latch>
#include <string>
#include <thread>

int main() {
    struct job {
        const std::string name;
        std::string product{ "not worked" };
        std::thread action{};
    }
    jobs[] = { {"annika"}, {"buru"}, {"chuck"} };
    std::latch work_done{ std::size(jobs) };
    std::latch start_clean_up{ 1 };
    auto work = [&](job& my_job) {
        my_job.product = my_job.name + " worked";
        work_done.count_down();
        start_clean_up.wait();
        my_job.product = my_job.name + " cleaned";
    };
    std::cout << "Work starting... ";
    for (auto& job : jobs) {
        job.action = std::thread{ work, std::ref(job) };
    }
    work_done.wait();
    std::cout << "done:\n";
    for (auto const& job : jobs) {
        std::cout << "" << job.product << '\n';
    }
    std::cout << "Workers cleaning up... ";
    start_clean_up.count_down();
    for (auto& job : jobs) {
        job.action.join();
    }
    std::cout << "done:\n";
    for (auto const& job : jobs) {
        std::cout << "" << job.product << '\n';
    }
}