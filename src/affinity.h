#pragma once
#ifndef QUANT1X_STD_AFFINITY_H
#define QUANT1X_STD_AFFINITY_H 1

#include "base.h"

// CPU亲和性
namespace affinity {

    bool bind_current_thread_to_optimal_cpu(std::error_code &ec);
}

#endif // QUANT1X_STD_AFFINITY_H