#ifdef __APPLE__
#else
#endif

#include <stdlib.h>
#include "Application.h"

int main(int argc, char** argv) {
    Application application("Beadando");
    application.run();

    return EXIT_SUCCESS;
}

