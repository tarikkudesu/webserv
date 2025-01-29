#pragma once

#include "Delete.hpp"

class Get
{
    private:
        bool                            isIndexing;
        RessourceHandler&               explorer;
        std::vector< BasicString >      &body;
    
        void                            readFile(void);
        void                            autoIndexing(void);
    
    public:


        Get(bool isIndexing, RessourceHandler &explorer, std::vector<BasicString> &body);
        Get(const Get &copy);
        Get &operator=(const Get &assign);
        ~Get();
};
