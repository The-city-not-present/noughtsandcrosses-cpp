#include "ai_options.h"
#include <map>
#include <string>
#include "ai/ai.h"

std::map<string,long double> AI::opts = {};

AI_options::AI_options()
{
    AI::opts["ai_evaluate_limit"] = 0.27;
    AI::opts["ai_evaluate_max_iteraions_count"] = 20;

    //[ 1, 1, 1, 0.9897212759004829, 0.7677065071882779, 0.5137087449114055, 0.025636071060371912, 0.004827189035840084, 0.0012938728520440856, 0.0005304513292940941, 0, 0, 0, 0 ]
    AI::opts["ai_estimates_5_me"] = 1.0;
    AI::opts["ai_estimates_4_me"] = 1.0;
    AI::opts["ai_estimates_3_me"] = 0.97;//45488883375935;
    AI::opts["ai_estimates_2_me"] = 0.26;//694428726657823;
    AI::opts["ai_estimates_1_me"] = 0.055;//44655881425952;
    AI::opts["ai_estimates_0_me"] = 0.0;
    AI::opts["ai_estimates_x_me"] = 0.0;

    AI::opts["ai_estimates_5_notme"] = 1.0;
    AI::opts["ai_estimates_4_notme"] = 0.99985;
    AI::opts["ai_estimates_3_notme"] = 0.88;//72808895171915;
    AI::opts["ai_estimates_2_notme"] = 0.18;//253419810934783;
    AI::opts["ai_estimates_1_notme"] = 0.05;//15585151401826;
    AI::opts["ai_estimates_0_notme"] = 0.0;
    AI::opts["ai_estimates_x_notme"] = 0.0;
}

AI_options::~AI_options()
{
    //dtor
}

AI_options temp;

