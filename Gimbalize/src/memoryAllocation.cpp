
extern "C" char* sbrk(int incr); // To get the heap pointer

const int totalSRAM = 32768; // 32 KB for Arduino MKR Zero
char* buffer = nullptr;      // Pointer to dynamically allocated buffer
int bufferSize = 0;          // Size of the allocated buffer
const int ledPin = 32;       // LED pin
const int allocationStep = sizeof(float) * 15; // Space for 15 float values

void monitorSRAM() {
    char stackTop;             // Current stack pointer
    char* heapEnd = sbrk(0);   // Current heap pointer
    int usedSRAM = totalSRAM - (&stackTop - heapEnd);
    int freeSRAM = &stackTop - heapEnd;
    float usagePercentage = (float(usedSRAM) / totalSRAM) * 100;

    // Print SRAM usage details
    Serial.printf("Buffer Size: %d bytes\n", bufferSize);
    Serial.printf("Used SRAM: %d bytes\n", usedSRAM);
    Serial.printf("Free SRAM: %d bytes\n", freeSRAM);
    Serial.printf("SRAM Usage: %.2f%%\n", usagePercentage);
    Serial.println("--------------------");

    // Turn on the LED if SRAM usage exceeds 90%
}

void memoryAllocateAndWrite(const float formattedData[]) {
    // Allocate additional memory for the buffer
    char* newBuffer = (char*)realloc(buffer, bufferSize + allocationStep);

    if (newBuffer) {
        buffer = newBuffer;
        float* floatBuffer = (float*)(buffer + bufferSize);
        bufferSize += allocationStep;

        // Copy data to the buffer
        for (int i = 0; i < 15; i++) {
            floatBuffer[i] = formattedData[i];
        }
    } else {
        Serial.println("Memory allocation failed!");
        while (true); // Halt if out of memory
    }

    // Monitor SRAM usage
    monitorSRAM();
    delay(20);
}
 
