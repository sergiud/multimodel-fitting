#include <cstdint>

static const int32_t correct_result[] = {
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  54,  54,  54,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  54,  54,
     54,  54,  54,  54,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  54,  54,  -1,
     54,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     54,  54,  54,  54,  54,  54,  54,  54,  -1,  54,
     54,  54,  54,  54,  54,  54,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  -1,  54,  54,  54,  54,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  -1,  54,  54,  -1,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  -1,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  68,  68,  68,  68,  68,  68,  68,  68,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  68,  68,  68,  68,  68,  68,  68,  68,  68,
     68,  68,  68,  68,  68,  68,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  68,  68,  68,  68,  68,  68,  68,  68,  68,
     68,  68,  68,  68,  68,  68,  68,  68,  68,  68,
     68,  68,  68,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  68,  68,  68,  68,  68,  68,
     68,  68,  68,  68,  68,  68,  68,  68,  68,  68,
     68,  68,  68,  68,  68,  68,  68,  68,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54, 896, 896, 896,  68,
     68,  68,  68,  68,  68,  68,  68,  68,  68,  68,
     68,  68,  68,  68,  68,  68,  68,  68,  68,  68,
     68,  68,  68,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54, 896,
    896, 896, 896,  68,  68,  68,  68,  68,  68,  68,
     68,  68,  68,  68,  68,  68,  68,  68,  68,  68,
     68,  68,  68,  68,  68,  68,  68,  68,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54, 896, 896, 896, 896, 896, 896,
     68,  68,  68,  68,  68,  68,  68,  68,  68,  68,
     68,  68,  68,  68,  68,  68,  68,  68,  68,  68,
     68,  68,  68,  68,  68,  68,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54, 896,
    896, 896, 896, 896, 896, 896, 896,  68,  68,  68,
     68,  68,  68,  68,  68,  68,  68,  68,  68,  68,
     68,  68,  68,  68,  68,  68,  68,  68,  68,  68,
     68,  68,  68,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54, 896, 896, 896, 896, 896, 896,
    896, 896, 896,  68,  68,  68,  68,  68,  68,  68,
     68,  68,  68,  68,  68,  68,  68,  68,  68,  68,
     68,  68,  68,  68,  68,  68,  68,  26,  26,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54, 896, 896,
    896, 896, 896, 896, 896, 896, 896, 896,  68,  68,
     68,  68,  68,  68,  68,  68,  68,  68,  68,  68,
     68,  68,  68,  68,  68,  68,  68,  68,  68,  26,
     26,  26,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54, 896, 896, 896, 896, 896, 896, 896, 896,
    896, 896, 896, 896,  68,  68,  68,  68,  68,  68,
     68,  68,  68,  68,  68,  68,  68,  68,  68,  68,
     26,  26,  26,  26,  26,  26,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54, 896, 896, 896, 896,
    896, 896, 896, 896, 896, 896, 896, 896, 896,  68,
     68,  68,  68,  68,  68,  68,  68,  68,  68,  68,
     68,  26,  26,  26,  26,  26,  26,  26,  26,  26,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
    896, 896, 896, 896, 896, 896, 896, 896, 896, 896,
    896, 896, 896, 896, 896,  68,  68,  68,  68,  68,
     68,  68,  68,  26,  26,  26,  26,  26,  26,  26,
     26,  26,  26,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54, 896, 896, 896, 896, 896, 896,
    896, 896, 896, 896, 896, 896, 896, 896, 896, 896,
    896,  68,  68,  68,  26,  26,  26,  26,  26,  26,
     26,  26,  26,  26,  26,  26,  26,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54, 896,
    896, 896, 896, 896, 896, 896, 896, 896, 896, 896,
    896, 896, 896, 896, 896, 896,  26,  26,  26,  26,
     26,  26,  26,  26,  26,  26,  26,  26,  26,  26,
     26,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54, 896, 896, 896, 896, 896, 896, 896,
    896, 896, 896, 896, 896, 896, 896, 896, 896, 896,
     26,  26,  26,  26,  26,  26,  26,  26,  26,  26,
     26,  26,  26,  26,  26,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54, 896, 896, 896,
    896, 896, 896, 896, 896, 896, 896, 896, 896, 896,
    896, 896, 896, 896,  26,  26,  26,  26,  26,  26,
     26,  26,  26,  26,  26,  26,  26,  26,  26,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54, 896, 896, 896, 896, 896, 896, 896, 896, 896,
    896, 896, 896, 896, 896, 896, 896, 896,  26,  26,
     26,  26,  26,  26,  26,  26,  26,  26,  26,  26,
     26,  26,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54, 896, 896, 896, 896, 896, 896,
    896, 896, 896, 896, 896, 896, 896, 896, 896, 896,
    896,  26,  26,  26,  26,  26,  26,  26,  26,  26,
     26,  26,  26,  26,  26,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54, 896,
    896, 896, 896, 896, 896, 896, 896, 896, 896, 896,
    896, 896, 896, 896, 896,  26,  26,  26,  26,  26,
     26,  26,  26,  26,  26,  26,  26,  26,  26,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54, 896, 896, 896, 896, 896, 896, 896,
    896, 896, 896, 896, 896, 896, 896, 896, 896,  26,
     26,  26,  26,  26,  26,  26,  26,  26,  26,  26,
     26,  26,  26,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54, 896, 896, 896,
    896, 896, 896, 896, 896, 896, 896, 896, 896, 896,
    896, 896, 896,  26,  26,  26,  26,  26,  26,  26,
     26,  26,  26,  26,  26,  26,  26,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54, 896, 896, 896, 896, 896, 896, 896, 896,
    896, 896, 896, 896, 896, 896, 896,  26,  26,  26,
     26,  26,  26,  26,  26,  26,  26,  26,  26,  26,
     26,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54, 896, 896, 896,
    896, 896, 896, 896, 896, 896, 896, 896, 896, 896,
    896,  26,  26,  26,  26,  26,  26,  26,  26,  26,
     26,  26,  26,  26,  26,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54, 896, 896, 896, 896, 896, 896, 896, 896,
    896, 896, 896, 896, 896,  26,  26,  26,  26,  26,
     26,  26,  26,  26,  26,  26,  26,  26,  26,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54, 896, 896,
    896, 896, 896, 896, 896, 896, 896, 896, 896,  26,
     26,  26,  26,  26,  26,  26,  26,  26,  26,  26,
     26,  26,  26,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54, 896, 896, 896, 896, 896, 896, 896, 896,
    896, 896, 896,  26,  26,  26,  26,  26,  26,  26,
     26,  26,  26,  26,  26,  26,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54, 896, 896, 896,
    896, 896, 896, 896, 896, 896, 896,  26,  26,  26,
     26,  26,  26,  26,  26,  26,  26,  26,  26,  26,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54, 896, 896, 896, 896, 896, 896, 896, 896,
    896,  26,  26,  26,  26,  26,  26,  26,  26,  26,
     26,  26,  26,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54, 896, 896, 896,
    896, 896, 896, 896, 896,  26,  26,  26,  26,  26,
     26,  26,  26,  26,  26,  26,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54, 896, 896, 896, 896, 896, 896, 896,  26,
     26,  26,  26,  26,  26,  26,  26,  26,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54, 896, 896, 896,
    896, 896, 896,  26,  26,  26,  26,  26,  26,  26,
     26,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54, 896, 896, 896, 896, 896,  26,  26,  26,
     26,  26,  26,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54, 896, 896,
    896,  26,  26,  26,  26,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54, 896,  26,  26,  26,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     26,  26,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54,  54,  54,  54,  54,  54,  54,
     54,  54,  54,  54
};
