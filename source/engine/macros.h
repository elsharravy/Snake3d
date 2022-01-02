#define LOG(text) std::cout<<text<<std::endl;
#define ERROR_EXIT_CODE -1

#define GETTER( type, name ) type get##name(  ){ return this->name; }
#define SETTER( type, name ) void set##name( type name ){ this->name = name; }