#include <filesystem>

int main( )
{
    std::filesystem::create_directories( "preprocess/include" );
    std::filesystem::create_directories( "preprocess/scripts" );

    //iterate over all lua scripts in the scripts folder
    for( const auto& entry : std::filesystem::directory_iterator( "preprocess/scripts" ) )
    {

    }

}
