#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Dynamic array of integers with automatic resizing.
typedef struct {
    int* data;
    int  capacity;
    int  size;
} DynamicArray;

// Holds the result of a profit or loss search over a price range.
typedef struct {
    int maxProfit;    // Largest achievable gain
    int maxBuyIndex;  // Buy index for maximum profit
    int maxSellIndex; // Sell index for maximum profit

    int maxLoss;      // Largest achievable loss (largest decline)
    int minBuyIndex;  // Buy index for maximum loss
    int minSellIndex; // Sell index for maximum loss
} TradeStats;

void initArray(DynamicArray* arr) {
    arr->capacity = 1;
    arr->data     = (int*)malloc(sizeof(int) * arr->capacity);
    arr->size     = 0;
}

static void growArray(DynamicArray* arr) {
    arr->capacity *= 2;
    int* newData = (int*)realloc(arr->data, arr->capacity * sizeof(int));
    if (!newData) {
        fprintf(stderr, "Memory allocation failed\n");
        free(arr->data);
        exit(1);
    }
    arr->data = newData;
}

void addToArray(DynamicArray* arr, int value) {
    if (arr->capacity == arr->size)
        growArray(arr);
    arr->data[arr->size++] = value;
}

void freeArray(DynamicArray* arr) {
    free(arr->data);
}

// Finds the buy/sell pair in arr[start..finish] that maximises profit.
// Prints the result or "N/A" if no profitable trade exists.
// Time complexity: O(n^2).
void analyzeProfit(TradeStats* stats, DynamicArray* arr, int start, int finish) {
    int currentMax = 0, maxIdx = 0, currentProfit = 0;
    stats->maxProfit = 0;

    for (int i = start; i < finish; i++) {
        for (int j = i + 1; j <= finish; j++) {
            if (currentMax < arr->data[j]) {
                currentMax = arr->data[j];
                maxIdx     = j;
            }
        }
        currentProfit = currentMax - arr->data[i];
        if (currentProfit > stats->maxProfit) {
            stats->maxBuyIndex  = i;
            stats->maxSellIndex = maxIdx;
            stats->maxProfit    = currentProfit;
        }
        currentProfit = 0;
        currentMax    = 0;
    }

    if (stats->maxProfit <= 0)
        printf("Maximum profit: N/A\n");
    else
        printf("Maximum profit: %d (%d - %d)\n",
               stats->maxProfit, stats->maxBuyIndex, stats->maxSellIndex);
}

// Finds the buy/sell pair in arr[start..finish] that maximises loss.
// Prints the result or "N/A" if no losing trade exists.
// Time complexity: O(n^2).
void analyzeLoss(TradeStats* stats, DynamicArray* arr, int start, int finish) {
    int currentMin = 0, minIdx = 0, currentLoss = 0;
    stats->maxLoss = INT_MIN;

    for (int i = start; i < finish; i++) {
        currentMin = INT_MAX;

        for (int j = i + 1; j <= finish; j++) {
            if (currentMin > arr->data[j]) {
                currentMin = arr->data[j];
                minIdx     = j;
            }
        }

        currentLoss = arr->data[i] - currentMin;
        if (currentLoss > stats->maxLoss) {
            stats->minBuyIndex  = i;
            stats->minSellIndex = minIdx;
            stats->maxLoss      = currentLoss;
        }
    }

    if (stats->maxLoss <= 0)
        printf("Maximum loss: N/A\n");
    else
        printf("Maximum loss: %d (%d - %d)\n",
               stats->maxLoss, stats->minBuyIndex, stats->minSellIndex);
}


// Interactive interface.
// Commands:
//   + <price>        — append a price to the array
//   ? <from> <to>    — find max profit and max loss in the index range [from, to]
//   0                — exit
int main(void) {
    DynamicArray arr;
    TradeStats   stats;
    initArray(&arr);

    char input[100];
    int  num1, num2, number;
    char extra;

    printf("Stock prices, search:\n");

    while (fgets(input, 100, stdin)) {
        if (input[0] == '0') break;

        if (input[0] == '+' && input[1] == ' '
            && sscanf(input + 2, "%d %c", &number, &extra) == 1
            && number >= 0) {
            addToArray(&arr, number);
        }
        else if (input[0] == '?' && input[1] == ' '
                 && sscanf(input + 2, "%d %d %c", &num1, &num2, &extra) == 2
                 && num1 <= num2 && num1 >= 0 && num2 >= 0
                 && num1 < arr.size && num2 < arr.size) {
            analyzeProfit(&stats, &arr, num1, num2);
            analyzeLoss(&stats, &arr, num1, num2);
        }
        else {
            printf("Invalid input.\n");
            break;
        }
    }

    freeArray(&arr);
    return 0;
}
