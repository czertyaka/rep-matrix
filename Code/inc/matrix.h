class Matrix
{

public:

    struct observation_t
    {
        float windDirection;
        float smithParameter;
        float windSpeed;
    };

    Matrix();
    ~Matrix();

    void AddObservation(observation_t observation);
    void ClearMatrix();
    void GetMatrix(float*** pppMatrix);

    const int GetN();
    const int GetJ();
    const int GetK();
    const int GetSize();

private:

    void _CalculateMatrix();

    int _WindDirIndex(observation_t observation);
    int _SmithParamIndex(observation_t observation);
    int _WindSpeedIndex(observation_t observation);

    static const int _n = 16;
    static const int _j = 7;
    static const int _k = 8;

    int _m[_n][_j][_k] = {0};
    int _w[_n][_j][_k] = {0};

};
