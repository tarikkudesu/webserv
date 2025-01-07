#pragma once

# include "Request.hpp"

class Post
{
         private:
                RessourceHandler&       explorer;

        public:

                Post(RessourceHandler &explorer);
		Post(const Post &copy);
		Post &operator=(const Post &assign);
		~Post();
};
