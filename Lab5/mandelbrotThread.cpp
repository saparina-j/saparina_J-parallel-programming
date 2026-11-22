#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include "CycleTimer.h"

typedef struct {
    float x0, x1;
    float y0, y1;
    unsigned int width;
    unsigned int height;
    int maxIterations;
    int* output;
    int threadId;
    int numThreads;
    bool interleaved;
} WorkerArgs;

extern void mandelbrotSerial(
    float x0, float y0, float x1, float y1,
    int width, int height,
    int startRow, int numRows,
    int maxIterations,
    int output[]);

void workerThreadStart(WorkerArgs * const args) {
    double startTime = CycleTimer::currentSeconds();

    if (!args->interleaved) {
        int rowsPerThread = args->height / args->numThreads;
        int startRow = args->threadId * rowsPerThread;
        int numRows = (args->threadId == args->numThreads - 1) 
                      ? (args->height - startRow) 
                      : rowsPerThread;

        mandelbrotSerial(args->x0, args->y0, args->x1, args->y1,
                         args->width, args->height,
                         startRow, numRows,
                         args->maxIterations, args->output);
    } else {
        for (unsigned int row = args->threadId; row < args->height; row += args->numThreads) {
            mandelbrotSerial(args->x0, args->y0, args->x1, args->y1,
                             args->width, args->height,
                             row, 1,
                             args->maxIterations, args->output);
        }
    }

    double endTime = CycleTimer::currentSeconds();
    if (std::getenv("PRINT_THREAD_TIME")) {
        printf("  [Thread %d] %.3f ms\n", args->threadId, (endTime - startTime) * 1000.f);
    }
}

void mandelbrotThread(
    int numThreads,
    float x0, float y0, float x1, float y1,
    int width, int height,
    int maxIterations,
    int output[])
{
    static constexpr int MAX_THREADS = 32;
    if (numThreads > MAX_THREADS) {
        fprintf(stderr, "Error: Max allowed threads is %d\n", MAX_THREADS);
        exit(1);
    }

    std::thread workers[MAX_THREADS];
    WorkerArgs args[MAX_THREADS];

    bool useInterleaved = (std::getenv("USE_INTERLEAVED") != nullptr);

    for (int i = 0; i < numThreads; i++) {
        WorkerArgs* w = args + i;
        w->x0 = x0;
        w->y0 = y0;
        w->x1 = x1;
        w->y1 = y1;
        w->width = width;
        w->height = height;
        w->maxIterations = maxIterations;
        w->numThreads = numThreads;
        w->output = output;
        w->threadId = i;
        w->interleaved = useInterleaved;
    }

    for (int i = 1; i < numThreads; i++) {
        *(workers + i) = std::thread(workerThreadStart, args + i);
    }

    workerThreadStart(args);

    for (int i = 1; i < numThreads; i++) {
        (workers + i)->join();
    }
}
