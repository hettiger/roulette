#include <iostream>
#include "src/Samplesize.h"
#include "src/Configuration.h"
#include "src/Sample.h"
#include <thread>

using namespace std;

std::mutex default_mutex;

void worker(Samplesize *samplesize, Configuration *configuration) {
    while (samplesize->decrease()) {
        auto *sample = new Sample(false, configuration, default_mutex);
        sample->execute();
    }
}

int main(int argc, char *argv[]) {
    auto *samplesize = new Samplesize(1, default_mutex);
    auto *configuration = new Configuration(argc, argv, samplesize);

    if (configuration->getVerbosity() > 0 || samplesize->getSize() < configuration->getNum_threads()) {
        for (uint sampleindex = 0; sampleindex < samplesize->getSize(); sampleindex++) {
            auto *sample = new Sample((!(bool) sampleindex), configuration, default_mutex);
            sample->execute();
        }
    } else {
        // The first sample should be processed single threaded to guarantee CSV header comes first.
        auto *sample = new Sample(true, configuration, default_mutex);
        sample->execute();
        samplesize->decrease();
        thread threads[configuration->getNum_threads() - 1];

        // Process samples on individual threads
        for (uint threadindex = 0; threadindex < configuration->getNum_threads() - 1; threadindex++) {
            threads[threadindex] = thread(worker, samplesize, configuration);
        }

        // Put some load on the main thread as well
        worker(samplesize, configuration);

        // Wait for all threads to complete their work
        for (uint threadindex = 0; threadindex < configuration->getNum_threads() - 1; threadindex++) {
            threads[threadindex].join();
        }
    }

    return 0;
}
