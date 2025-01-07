#pragma once

# include "Delete.hpp"

class Get
{
        private:

                bool                    isIndexing;
                RessourceHandler&       explorer;
                String                  body;

                void                 readFile( void );
                void                 autoIndexing( void );

        public:

                /*         constructors      */

                Get(bool isIndexing, RessourceHandler &explorer);
		Get(const Get &copy);
		Get &operator=(const Get &assign);
		~Get();

                /*         class logic      */

                String          getBody();

};
