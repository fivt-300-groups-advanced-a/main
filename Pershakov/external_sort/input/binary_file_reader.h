#ifndef BINARY_FILE_READER

#define BINARY_FILE_READER

#include <string>
#include <fstream>

template <class T> class Binary_file_reader {
    public:
        explicit Binary_file_reader(std::ifstream &new_stream){
            this->in = &new_stream;
        }

        explicit Binary_file_reader(std::string file_name){
            this->in = 
                new std::ifstream(file_name.c_str(), 
                        std::ifstream::binary | std::ifstream::in); 
        }

        explicit Binary_file_reader(char *file_name){
            this->in = 
                new std::ifstream(file_name, 
                        std::ifstream::binary | std::istream::in);
        }

        bool operator () (T &next){
            return this->in->read((char*)&next, sizeof(T));
        }
};

#endif
