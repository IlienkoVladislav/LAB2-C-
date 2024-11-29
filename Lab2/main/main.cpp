#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <random>

// Структура для збереження інформації про процес
struct Process {
    int id;             // ID процесу
    int arrivalTime;    // Час прибуття
    int burstTime;      // Час виконання
    int priority;       // Пріоритет
    int remainingTime;  // Залишковий час виконання
    int startTime;      // Час початку виконання
    int endTime;        // Час завершення виконання
    int waitingTime;    // Час очікування
};

// Функція для генерації випадкових процесів
std::vector<Process> generateProcesses(int numProcesses) {
    std::vector<Process> processes;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> arrivalDist(0, 10); // Час прибуття
    std::uniform_int_distribution<> burstDist(1, 10);   // Час виконання
    std::uniform_int_distribution<> priorityDist(1, 5); // Пріоритет

    for (int i = 0; i < numProcesses; ++i) {
        processes.push_back({
            i + 1,                        // ID процесу
            arrivalDist(gen),             // Час прибуття
            burstDist(gen),               // Час виконання
            priorityDist(gen),            // Пріоритет
            0,                            // Залишковий час
            0,                            // Час початку
            0,                            // Час завершення
            0                             // Час очікування
            });
        processes.back().remainingTime = processes.back().burstTime; // Ініціалізація залишкового часу
    }

    return processes;
}

// Функція для виведення процесів
void printProcesses(const std::vector<Process>& processes) {
    std::cout << std::setw(5) << "ID" << std::setw(15) << "Arrival Time"
        << std::setw(15) << "Burst Time" << std::setw(10) << "Priority"
        << std::setw(15) << "Waiting Time" << std::setw(15) << "Turnaround Time" << std::endl;

    for (const auto& p : processes) {
        std::cout << std::setw(5) << p.id
            << std::setw(15) << p.arrivalTime
            << std::setw(15) << p.burstTime
            << std::setw(10) << p.priority
            << std::setw(15) << p.waitingTime
            << std::setw(15) << (p.endTime - p.arrivalTime) // Turnaround Time = час завершення - час прибуття
            << std::endl;
    }
}

// Алгоритм Round Robin
void roundRobin(std::vector<Process> processes, int timeQuantum) {
    std::cout << "\n=== Round Robin Scheduling ===\n";

    int currentTime = 0;
    std::queue<Process*> readyQueue;

    // Додаємо процеси, які прибули на початку
    for (auto& process : processes) {
        if (process.arrivalTime <= currentTime) {
            readyQueue.push(&process);
        }
    }

    while (!readyQueue.empty()) {
        Process* currentProcess = readyQueue.front();
        readyQueue.pop();

        // Виконуємо процес на час квантуму або до завершення
        int executeTime = std::min(currentProcess->remainingTime, timeQuantum);
        currentProcess->remainingTime -= executeTime;
        currentTime += executeTime;

        // Додаємо нові процеси, які прибули під час виконання
        for (auto& process : processes) {
            if (process.arrivalTime <= currentTime && process.remainingTime > 0) {
                // Перевірка, чи процес вже є у черзі
                bool alreadyInQueue = false;
                std::queue<Process*> tempQueue = readyQueue;
                while (!tempQueue.empty()) {
                    if (tempQueue.front() == &process) {
                        alreadyInQueue = true;
                        break;
                    }
                    tempQueue.pop();
                }
                if (!alreadyInQueue) {
                    readyQueue.push(&process);
                }
            }
        }

        // Якщо процес не завершився, повертаємо його в чергу
        if (currentProcess->remainingTime > 0) {
            readyQueue.push(currentProcess);
        }
        else {
            currentProcess->endTime = currentTime;
            currentProcess->waitingTime = currentProcess->endTime - currentProcess->arrivalTime - currentProcess->burstTime;
        }
    }

    printProcesses(processes);
}

// Головна функція
int main() {
    int numProcesses = 5;  // Кількість процесів
    int timeQuantum = 3;   // Часовий квант для Round Robin

    // Генеруємо випадкові процеси
    auto processes = generateProcesses(numProcesses);

    // Виводимо згенеровані процеси
    std::cout << "=== Generated Processes ===\n";
    printProcesses(processes);

    // Виконуємо Round Robin
    roundRobin(processes, timeQuantum);

    return 0;
}
