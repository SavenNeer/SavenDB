// test_table.cpp

#ifndef _TEST_TABLE_cpp_
#define _TEST_TABLE_cpp_

#include "test_table.h"

TableUI::TextTable::TextTable( char horizontal, char vertical, char corner) :
    _horizontal( horizontal ),
    _vertical( vertical ),
    _corner( corner )
{}


unsigned TableUI::TextTable::Calsetw(unsigned show_width, const std::string& str)const{
    unsigned width = GetWidth(str);
    return str.size() + show_width - width;
}

void TableUI::TextTable::SetAlignment( unsigned i, Alignment alignment ){
    _alignment[ i ] = alignment;
}
 
TableUI::Alignment TableUI::TextTable::GetAlignment( unsigned i )const{
    return _alignment[ i ];
}
 
char TableUI::TextTable::Vertical()const{
    return _vertical;
}

char TableUI::TextTable::Horizontal()const{
    return _horizontal;
}

void TableUI::TextTable::Add( std::string const & content ){
    _current.push_back( content );
}

void TableUI::TextTable::EndOfRow(){
    _rows.push_back( _current );
    _current.assign( 0, "" );
}

template <typename Iterator>
void TableUI::TextTable::AddRow( Iterator begin, Iterator end ){
    for(Iterator i = begin; i != end; ++i ){
        add( * i ); 
    }
    TableUI::TextTable::EndOfRow();
}

template <typename Container>
void TableUI::TextTable::AddRow( Container const & container ){
    TableUI::TextTable::AddRow( container.begin(), container.end() );
}

std::vector<TableUI::Row> const & TableUI::TextTable::Rows()const{
    return _rows;
}

void TableUI::TextTable::Setup()const{
    DetermineWidths();
    SetupAlignment();
}

std::string TableUI::TextTable::Ruler()const{
    std::string result;
    result += _corner;
    for( std::vector<unsigned>::iterator width = _width.begin(); width != _width.end(); ++ width ) {
        result += Repeat( * width, _horizontal );
        result += _corner;
    }
    return result;
}
 

std::vector<TableUI::Row> TableUI::TextTable::GetRows()const{
    return _rows;
}


int TableUI::TextTable::Width( unsigned i )const{
    return _width[ i ];
}


std::string TableUI::TextTable::Repeat( unsigned times, char c ){
    std::string result;
    for( ; times > 0; --times ){
        result += c;
    }
    return result;
}
 
unsigned TableUI::TextTable::GetWidth(const std::string &str) const {
    unsigned width = 0;
    size_t i = 0;
    while(i < str.size()) {
        if (str[i] >= 0 && str[i] <= 127) {
            width += 1;
            ++i;
        }   
        else {
            width += 2;
            i += 3;
        }   
    }   
    return width;
}

unsigned TableUI::TextTable::Columns() const
{
    if (_rows.size() > 0) {
        return _rows[ 0 ].size();
    }
    else {
        return 0;
    }
}

void TableUI::TextTable::DetermineWidths() const
{
    _width.assign( Columns(), 0 );
    std::vector<Row>::const_iterator rowIterator = _rows.begin();
    for ( ; rowIterator != _rows.end(); ++rowIterator ) {
        Row const & row = * rowIterator;
        for ( unsigned i = 0; i < row.size(); ++i ) {
            //_width[ i ] = _width[ i ] > row[ i ].size() ? _width[ i ] : row[ i ].size();
            unsigned row_width = GetWidth(row[i]);
            _width[ i ] = _width[ i ] > row_width ? _width[ i ] : row_width;
        }
    }
}

void TableUI::TextTable::SetupAlignment() const
{
    for ( unsigned i = 0; i < Columns(); ++i ) {
        if ( _alignment.find( i ) == _alignment.end() ) {
            _alignment[ i ] = LEFT;
        }
    }
}


/* 输出流函数 */
std::ostream& TableUI::operator<<( std::ostream & stream, TableUI::TextTable const & table )
{
    table.Setup();
    stream << table.Ruler() << "\n";
    std::vector<Row> rows = table.GetRows();
    for (size_t i = 0; i < rows.size(); ++i) {
        Row row = rows[i];
        stream << table.Vertical();
        for (size_t j = 0; j < row.size(); ++j) {
            //stream << std::setw( table.width( j ) ) << std::right << row[ j ];
            stream << std::setw( table.Calsetw(table.Width(j), row[j])) << std::right << row[ j ];
            stream << table.Vertical();
        }
        stream << "\n";
        stream << table.Ruler() << "\n";
    }
    return stream;
}


//转换器
int TableUI::convertTextTable(TableUI::TBlis rhs,TableUI::TextTable& res){
    TableUI::TextTable one( '-', '|', '+' );
    int lenth = rhs.size();
    int maxSize = 0;

    for(int i=0;i<lenth;i++){
        int lsize = rhs[i].size();
        maxSize = lsize > maxSize ? lsize : maxSize;
    }

    if(maxSize == 0 || lenth == 0){
        one.Add("Nothing In Table");
        one.EndOfRow();
        res = one;
        return 0;
    }

    for(int i=0;i<lenth;i++){
        std::vector<std::string>& lis = rhs[i];
        int len = lis.size();
        for(int k=0;k<maxSize;k++){
            if(k >= len){
                one.Add("+Empty+");
            }else{
                one.Add(lis[k]);
            }
        }
        one.EndOfRow();
    }

    res = one;
    return 0;
}


#endif //_TEXT_TABLE_CPP_




// // https://github.com/haarcuba/cpp-text-table
// // https://blog.csdn.net/dreamvyps/article/details/79088935
// //  demo code
//  int main() {
//  TextTable t( '-', '|', '+' );
//  t.Add( "" );
//  t.Add( "性别" );
//  t.Add( "Age" );
//  t.Add( "属性值" );
//  t.EndOfRow();
//  t.Add( "Moses" );
//  t.Add( "male" );
//  t.Add( "4556" );
//  t.Add("");
//  t.EndOfRow();
//  std::cout << t;
//  }




