#include "main.h"

int main()
{
    Matrix matrix;

    Matrix::observation_t observation = {1, 2, 5};
    matrix.AddObservation(observation);

    return 0;
}