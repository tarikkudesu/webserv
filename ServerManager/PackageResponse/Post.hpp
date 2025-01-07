#pragma once

# include "Request.hpp"

class Post
{
         private:
                RessourceHandler&       explorer;
		String			__requestbody;
                void                    writeFile( void );

        public:

                Post(RessourceHandler &explorer, String requestbody);
		Post(const Post &copy);
		Post &operator=(const Post &assign);
		~Post();
};
