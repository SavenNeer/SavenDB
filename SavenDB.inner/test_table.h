#ifndef _TEXT_TABLE_H_
#define _TEXT_TABLE_H_

#include <iostream>
#include <map>
#include <iomanip>
#include <vector>
#include <string>

namespace TableUI{

    typedef std::vector<std::vector<std::string> > TBlis;
    typedef std::vector< std::string > Row;

    enum Alignment { LEFT, RIGHT }; 

    class TextTable {
    public:
        TextTable( char horizontal = '-', char vertical = '|', char corner = '+' );

        unsigned Calsetw(unsigned show_width, const std::string& str)const;

        void SetAlignment( unsigned i, Alignment alignment );

        Alignment GetAlignment( unsigned i )const;

        char Vertical()const;

        char Horizontal()const;

        void Add( std::string const & content );

        void EndOfRow();

        template <typename Iterator>
        void AddRow( Iterator begin, Iterator end );

        template <typename Container>
        void AddRow( Container const & container );

        std::vector< Row > const & Rows()const;

        void Setup()const;

        std::string Ruler()const;

        std::vector<Row> GetRows()const;

        int Width( unsigned i )const;

    private:
        char _horizontal;
        char _vertical;
        char _corner;
        Row _current;
        std::vector< Row > _rows;
        std::vector< unsigned > mutable _width;
        //std::vector< unsigned > _width;
        std::map< unsigned, Alignment > mutable _alignment;

    private:
        static std::string Repeat( unsigned times, char c );
        
        unsigned GetWidth(const std::string &str)const;

        unsigned Columns()const;

        void DetermineWidths()const;

        void SetupAlignment()const;
    };
    
    std::ostream& operator<<( std::ostream & stream, TextTable const & table );

    //转换器
    int convertTextTable(TBlis rhs,TextTable& res);

};

#endif //_TEXT_TABLE_H_

