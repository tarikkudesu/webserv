#pragma once

# include "Post.hpp"

class Delete
{
        private:
                RessourceHandler&       explorer;
                void                    deleteFile( void );

        public:

                Delete(RessourceHandler &explorer);
		Delete(const Delete &copy);
		Delete &operator=(const Delete &assign);
		~Delete();
};
