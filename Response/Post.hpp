#pragma once

# include "RessourceHandler.hpp"

class Post
{
         private:
                RessourceHandler&       explorer;
		            Request&			          request;

                t_svec                  curr_file;
                void                    writeFile( void );
                void                    getCurr_file( void );

        public:

                Post(RessourceHandler &explorer, Request& request);
		            Post(const Post &copy);
		            Post &operator=(const Post &assign);
		            ~Post();
};
