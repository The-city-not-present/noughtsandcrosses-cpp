#include "ai_options.h"
#include <map>
#include <string>
#include "ai/ai.h"

std::map<const char*,double> AI::opts = {};

AI_options::AI_options()
{
    AI::opts["ai_evaluate_limit"] = 0.49;
    AI::opts["ai_evaluate_max_iteraions_count"] = 20;

    AI::opts["ai_estimates_5_me"] = 1.0;
    AI::opts["ai_estimates_4_me"] = 1.0;
    AI::opts["ai_estimates_3_me"] = 0.88;
    AI::opts["ai_estimates_2_me"] = 0.19;
    AI::opts["ai_estimates_1_me"] = 0.09;
    AI::opts["ai_estimates_0_me"] = 0.0001;
    AI::opts["ai_estimates_x_me"] = 0.0;

    AI::opts["ai_estimates_5_notme"] = 1.0;
    AI::opts["ai_estimates_4_notme"] = 0.995;
    AI::opts["ai_estimates_3_notme"] = 0.75;
    AI::opts["ai_estimates_2_notme"] = 0.13;
    AI::opts["ai_estimates_1_notme"] = 0.08;
    AI::opts["ai_estimates_0_notme"] = 0.0001;
    AI::opts["ai_estimates_x_notme"] = 0.0;
}

AI_options::~AI_options()
{
    //dtor
}

AI_options temp;

