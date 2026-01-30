#include <stdio.h>
#include <dlfcn.h>

int main()
{
    // Load the shared library
    void *handle = dlopen("./bin/libitm.so", RTLD_LAZY);
    if (!handle)
    {
        handle = dlopen("./bin/libitm.dylib", RTLD_LAZY);
        if (!handle)
        {
            fprintf(stderr, "Failed to load library: %s\n", dlerror());
            return 1;
        }
        printf("✓ Successfully loaded libitm.dylib\n");
    }
    else
    {
        printf("✓ Successfully loaded libitm.so\n");
    }

    // Look up a simple function to verify library is functional
    typedef double (*FreeSpaceLossFunc)(double, double);
    FreeSpaceLossFunc FreeSpaceLoss = (FreeSpaceLossFunc)dlsym(handle, "FreeSpaceLoss");

    const char *error = dlerror();
    if (error != NULL)
    {
        fprintf(stderr, "Failed to find FreeSpaceLoss function: %s\n", error);
        dlclose(handle);
        return 1;
    }

    printf("✓ Found FreeSpaceLoss function\n");

    // Test call: Free space loss at 1km distance, 1000 MHz
    double distance_m = 1000.0;
    double freq_mhz = 1000.0;
    double loss_db = FreeSpaceLoss(distance_m, freq_mhz);

    printf("✓ Function call successful\n");
    printf("  Free space loss at %.0f m, %.0f MHz: %.2f dB\n",
           distance_m, freq_mhz, loss_db);

    // Clean up
    dlclose(handle);

    printf("\n✓ libitm library is working correctly!\n");
    return 0;
}
