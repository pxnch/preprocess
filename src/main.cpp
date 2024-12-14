#include <filesystem>
#include <fstream>
#include <sol/sol.hpp>
#include <utility>

class c_header
{
public:
    explicit c_header( std::string name )
        : name( std::move( name ) )
    {
    }

    void add_content( const std::string& content )
    {
        this->content += content;
    }

    void write( )
    {
        content += "\n#endif";
        std::ofstream file( "preprocess/include/preprocess/" + name + ".hpp" );
        file << content;
    }

    void add_macro( const sol::this_state state, const std::string& name, const sol::object& content )
    {
        sol::state_view lua( state );
        const sol::protected_function_result result = lua[ "tostring" ]( content );
        if( !result.valid( ) )
        {
            const sol::error err = result;
            std::cerr << err.what( ) << std::endl;
            return;
        }

        this->content += "#define " + name + " " + result.get< std::string >( ) + "\n";
    }

private:
    std::string name;
    std::string content = "#ifndef " + name + "_preprocess\n#define " + name + "_preprocess\n\n";
};

int main( )
{
    if( std::filesystem::exists( "preprocess/include" ) )
        std::filesystem::remove_all( "preprocess/include" );

    std::filesystem::create_directories( "preprocess/include/preprocess" );
    std::filesystem::create_directories( "preprocess/scripts" );

    std::cout << R"(

  _ __  _ __ ___ _ __  _ __ ___   ___ ___  ___ ___
 | '_ \| '__/ _ \ '_ \| '__/ _ \ / __/ _ \/ __/ __|
 | |_) | | |  __/ |_) | | | (_) | (_|  __/\__ \__ \
 | .__/|_|  \___| .__/|_|  \___/ \___\___||___/___/
 | |            | |
 |_|            |_|

)";

    int script_count = 0;
    sol::state lua;
    lua.open_libraries( sol::lib::base, sol::lib::package, sol::lib::coroutine, sol::lib::string, sol::lib::os,
                        sol::lib::math, sol::lib::table, sol::lib::debug, sol::lib::bit32, sol::lib::io,
                        sol::lib::utf8 );

    lua.new_usertype< c_header >( "c_header",
                                  "add_content", &c_header::add_content,
                                  "write", &c_header::write,
                                  "add_macro", &c_header::add_macro );

    sol::table table = lua.create_named_table( "preprocess" );
    table[ "create_header" ] = [ ] ( const std::string& name )
    {
        return c_header( name );
    };

    //iterate over all lua scripts in the scripts folder
    for( const auto& entry : std::filesystem::directory_iterator( "preprocess/scripts" ) )
    {
        if( sol::protected_function_result result = lua.script_file( entry.path( ).string( ) ); !result.valid( ) )
        {
            sol::error err = result;
            std::cerr << err.what( ) << std::endl;
        }

        ++script_count;
    }

    std::cout << "Preprocessed " << script_count << " scripts" << std::endl;
}
