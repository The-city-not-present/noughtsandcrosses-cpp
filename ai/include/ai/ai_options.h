#ifndef AI_OPTIONS_H
#define AI_OPTIONS_H
#include <string.h>

class AI_options
{
    public:
        AI_options();
        virtual ~AI_options();
        void update();

    protected:

    private:
};

struct char_cmp {
    bool operator () (const char *a,const char *b) const
    {
        return strcmp(a,b)<0;
    }
};


#endif // AI_OPTIONS_H
