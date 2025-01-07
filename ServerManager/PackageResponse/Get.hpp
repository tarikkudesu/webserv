#pragma once

# include "Request.hpp"

class Get
{
        private:

                bool                    isIndexing;
                RessourceHandler&       explorer;
                String                 body;

                String&                 readFile( void );
                t_svec                  autoIndexing( void );

        public:

                /*         constructors      */

                Get(bool isIndexing, RessourceHandler &explorer);
		Get(const Get &copy);
		Get &operator=(const Get &assign);
		~Get();

                /*         class logic      */

                String          getBody();

};
