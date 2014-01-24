/*
4) Даны файлы river.txt, sea.txt, ocean.txt в которых сохранены
соответствующие таблицы sql в следующих форматах (разделитель запятая):
river.txt:
id, name, seaid

sea.txt:
id, name, oceanid

ocean.txt:
id, name

Например
river.txt:
1, Хуанхэ, 1
2, Онега, 3
3, Кемь, 3

sea.txt:
1, Жёлтое‎, 1
2, Берингово, 1
3, Белое‎, 2

ocean.txt:
1,Тихий
2,Северный ледовитый

Необходимо пополнить списки данными. Добавить 1-2 океана, в каждом
океане должно быть 3-4 моря, в каждом море 3-4 реки.
Реализовать следующий отчет и вывести его в консоль или файл: название
океана, список его морей через запятую, список его рек, через запятую. В
отчет должны попасть все океаны, моря и реки. Разделитель колонок |
*/

#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <hash_map>
#include <map>
#include <exception>


using namespace std;

void Warning( string text
              )
{
    cerr << text << "\n";
}

struct Output 
{          
    void operator() (const std::pair<string,unsigned> & p) const
    { 
        std::cout << p.first << '=' << p.second << "\n";
    }
} ;

class River
{
public:
    typedef int RiverID;

    string name_;

    River(const string & name)
        : name_( name )
    {

    }
};

struct Output 
{          
    void operator() (const std::pair<Ocean::OceanID,Ocean> & p) const
    { 
        std::cout << p.second.Name() << '|' << p.second.seas_ << "\n";
    }
} ;


class Sea
{
public:
    typedef int SeaID;

    Sea( string name )
        : name_( name )
    {

    }
    std::string Name() const { return name_; }

    void AddRiver( River::RiverID id, string name )
    {
        rivers_.insert( make_pair(id, River( name ) ) ); 
    }

private:
    string name_;
    typedef map<River::RiverID, River> RiversStorage;

    RiversStorage rivers_;
};


class Ocean 
{
    string name_;
    typedef map<Sea::SeaID, Sea> SeasStorage;
    
    SeasStorage seas_;

public:
    typedef int OceanID;

    Ocean( string name )
        : name_(name)
    {

    }
    std::string Name() const { return name_; }
    
    void AddSea( Sea::SeaID id, string name )
    {
        seas_.insert( make_pair(id, Sea( name ) ) ); 
    }

    void AddRiver( Sea::SeaID idSea, River::RiverID idRiver, string name )
    {
        SeasStorage::iterator itSea = seas_.find( idSea );
        if( itSea == seas_.end() )
        {
            throw exception( "Failed to find sea id" );
        }
        itSea->second.AddRiver( idRiver, name );
    }

    void Print()
    {

    }
};

//////////////////////////////////////////////////////////////////////////
class LineReader
{
    fstream f_;
    string typeName_;

    void SkipDelimiters( char auxdelim = ',' )
    {
        while( f_ && ( f_.peek() == ' ' || f_.peek() == auxdelim ) )
        {
            f_.get();
        }
    }
public:
    LineReader( string path, string typeName )
        : f_( path.c_str() )
        , typeName_(typeName)
    {

    }
public:

    
    bool Read( bool modeTriple, int & id, string & name, int & idParent )
    {
        
        if( !f_ )
            return false;

        
        if( !( f_ >> id ) )
        {
            //throw exception( "Unable to read ID ");
            return false;

        }

        SkipDelimiters();

        char delimiter = modeTriple ? ',' : '\n';


        if( !getline( f_, name, delimiter ) || name.empty() )
        {
            throw exception( ("Unable to read name of " + typeName_).c_str() );
        }


        if( modeTriple )
        {
            SkipDelimiters();
            if( !( f_ >> idParent ) )
            {
                throw exception( "Unable to read parent ID ");
            }
        }
        

        return true;
    }
};

//////////////////////////////////////////////////////////////////////////
class ReportGenerator
{
    typedef map<Ocean::OceanID, Ocean> OceansStorage;
    typedef map<Sea::SeaID, Ocean::OceanID> MapIDParentForSea;

    OceansStorage oceans_;
    MapIDParentForSea idParentForSea_;


public:

    //void ReadOceans()
    //{
    //    std::ifstream file("ocean.txt");
    //    std::string oceanName;
    //    while( file )
    //    {
    //        int id;
    //        file >> id;

    //        if( !file ) break;

    //        getline( file, oceanName, ',' );
    //        if( !file ) break;

    //        getline( file, oceanName );
    //        if( !oceanName.empty() ) break;


    //        cout << id << " " << oceanName << "\n";

    //        OceansStorage::iterator i;
    //        // проверяем, на дубликаты
    //        if( ( i = oceans_.find( id ) ) != oceans_.end() )
    //        {
    //            if( i->second.Name() == oceanName )
    //            {
    //                Warning( "Duplicate ocean " + oceanName + ", skipped" );
    //            }
    //            else
    //            {
    //                Warning( "Duplicate id, skipped" );
    //            }
    //        }
    //        else
    //        {
    //            // добавляем новый пустой океан
    //            oceans_.insert( make_pair( id, Ocean(oceanName) ) );
    //        }
    //    }
    //}

    void ReadOceans()
    {
        LineReader reader( "ocean.txt", "ocean" );
        int id;
        string oceanName;
        int dumb;
        while( reader.Read( false, id, oceanName, dumb ) )
        {
            cout << id << " " << oceanName << "\n";
            // без проверки на повторы
            oceans_.insert( make_pair( id, Ocean(oceanName) ) ); 
        }
     
    }

    void ReadSeas()
    {
        LineReader reader( "sea.txt", "sea" );
        int id;
        string seaName;
        int idParent;
        while( reader.Read( true, id, seaName, idParent ) )
        {
            cout << id << " " << seaName << idParent << "\n";
            // без проверки на повторы
            OceansStorage::iterator i = oceans_.find(idParent);
            if( i == oceans_.end() )
            {
                throw exception( "Wrong ocean id in seas.txt" );
            }
            i->second.AddSea( id, seaName ); // не оптимально, зато красиво
            idParentForSea_[id] = idParent;
        }
    }

    void ReadRivers()
    {
        LineReader reader( "river.txt", "river" );
        int id;
        string riverName;
        int idSeaParent;
        while( reader.Read( true, id, riverName, idSeaParent ) )
        {
            cout << id << " " << riverName << idSeaParent << "\n";

            MapIDParentForSea::iterator it = idParentForSea_.find( idSeaParent );
            if( it == idParentForSea_.end() )
            {
                throw exception( "Wrong sea id in river.txt" );
            }

            int idOceanParent = it->second;

            // без проверки на повторы
            OceansStorage::iterator i = oceans_.find(idOceanParent);
            if( i == oceans_.end() )
            {
                throw exception( "Wrong sea id in river.txt" );
            }
            i->second.AddRiver( idSeaParent, id, riverName ); // не оптимально, зато красиво
        }
    }

    void GenerateReport() 
    {
        
    }


};

//std::istream & operator>>( std::istream & is, Ocean & my )
//{
//    int id;
//    is >> id
//    getline(is, my.str, '\t') )
//        return is >> my.v1 >> my.v2;
//}


int main()
{
    ReportGenerator report;
    try
    {
        report.ReadOceans();
        report.ReadSeas();
        report.ReadRivers();

        report.GenerateReport();
    }
    catch( const exception & e )
    {
        cout << e.what() << endl;
    }

    

    
  

    return 0;
}