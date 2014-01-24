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
//#include <map>
#include <exception>
#include <list>


using namespace std;
using namespace stdext; // для hash_map в VS2008


class River
{
private:
    string name_;

public:
    typedef int RiverID;


    River(const string & name)
        : name_( name )
    {

    }
    std::string Name() const { return name_; }

};



class Sea
{
public:
    typedef int SeaID;

    Sea( string name )
        : name_( name )
    {

    }
    std::string Name() const { return name_; }
    

private:
    string name_;

};


class Ocean 
{
    string name_;
    typedef list<Sea> SeasStorage;
    SeasStorage seas_; // Список морей

    typedef list<River> RiversStorage;
    RiversStorage rivers_; // Список рек

public:
    typedef int OceanID;

    Ocean( string name )
        : name_(name)
    {

    }
    std::string Name() const { return name_; }
    
    void AddSea( Sea::SeaID id, string name )
    {
        seas_.push_back( Sea( name ) ); 
    }

    void AddRiver( const string & name )
    {
        rivers_.push_back( River( name ) );
    }

    void Print()
    {
        cout << name_;
        cout << "|";
        for( SeasStorage::iterator it = seas_.begin(); it != seas_.end(); ++ it )
        {
            if( it != seas_.begin() )
                cout << "," ;
            cout << it->Name();
            
        }
        cout << "|";
        for( RiversStorage::iterator it = rivers_.begin(); it != rivers_.end(); ++ it )
        {
            if( it != rivers_.begin() )
                cout << "," ;
            cout << it->Name();
        }

    }
};

//////////////////////////////////////////////////////////////////////////
// класс для чтения файла с разделителями
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
    typedef hash_map<Ocean::OceanID, Ocean> OceansStorage;
    typedef hash_map<Sea::SeaID, Ocean::OceanID> MapIDParentForSea;

    OceansStorage oceans_;
    MapIDParentForSea idParentForSea_;


public:


    void ReadOceans()
    {
        LineReader reader( "ocean.txt", "ocean" );
        Ocean::OceanID id;
        string oceanName;
        int dumb;
        while( reader.Read( false, id, oceanName, dumb ) )
        {
            //cout << id << " " << oceanName << "\n";

            // без проверки на повторы
            oceans_.insert( make_pair( id, Ocean(oceanName) ) ); 
        }
     
    }

    void ReadSeas()
    {
        LineReader reader( "sea.txt", "sea" );
        Sea::SeaID id;
        string seaName;
        Ocean::OceanID idParent;
        while( reader.Read( true, id, seaName, idParent ) )
        {
            //cout << id << " " << seaName << idParent << "\n";

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
        River::RiverID id;
        string riverName;
        Sea::SeaID idSeaParent;
        while( reader.Read( true, id, riverName, idSeaParent ) )
        {
            //cout << id << " " << riverName << idSeaParent << "\n";

            MapIDParentForSea::iterator it = idParentForSea_.find( idSeaParent );
            if( it == idParentForSea_.end() )
            {
                throw exception( "Wrong sea id in river.txt" );
            }

            Ocean::OceanID idOceanParent = it->second;

            // без проверки на повторы
            OceansStorage::iterator i = oceans_.find(idOceanParent);
            if( i == oceans_.end() )
            {
                throw exception( "Wrong sea id in river.txt" );
            }
            i->second.AddRiver( riverName ); 
        }
    }

    void GenerateReport() // генерируем отчет по всем океанам
    {
        for( OceansStorage::iterator it = oceans_.begin(); it != oceans_.end(); ++ it )
        {
            it->second.Print();
            cout << "\n";
        }

    }


};


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