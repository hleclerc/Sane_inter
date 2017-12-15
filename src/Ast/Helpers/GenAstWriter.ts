import read_cmds from "./ReadCmds"

function trans_arg( arg: {type:string,name:string} ) : Array<{type:string,name:string}> {
    switch ( arg.type ) {
        case "VecVecString": return [ { type: "const Vec<Vec<Hpipe::CmString>> &", name: arg.name }, ];
        case "String"      : return [ { type: "const char *"                 , name: arg.name + "_beg" },
                                      { type: "int "                         , name: arg.name + "_len" } ];
        case "VecPT"       : return [ { type: "const Vec<size_t> &"          , name: arg.name }, ];
        case "VecString"   : return [ { type: "const Vec<Hpipe::CmString> &"     , name: arg.name }, ];
        case "VecLex"      : return [ { type: "const Vec<const Lexem *> &"   , name: arg.name }, ];
        case "VecApply"    : return [ { type: "const Vec<Apply> &"           , name: arg.name }, ];
        case "Lex"         : return [ { type: "const Lexem *"                , name: arg.name }, ];
        case "SI32"        :   
        case "PI32"        :   
        case "PI8"         : return [ { type: `${ arg.type } `               , name: arg.name }, ];
        default            : return [ { type: `const ${ arg.type } &`        , name: arg.name }, ];
    }
}

function bs_write( arg: {type:string,name:string} ) {
    switch ( arg.type ) {
        case "PI8"         : return `bs.write_byte( ${ arg.name } );`;
        case "SI32"        : return `bs.write_signed( ${ arg.name } );`;
        case "PI32"        : return `bs.write_unsigned( ${ arg.name } );`;
        case "Lex"         : return `_write_Lex( ${ arg.name }, wr_flags );`;
        case "VecLex"      : return `_write_VecLex( ${ arg.name }, wr_flags );`;
        case "VecApply"    : return `_write_VecApply( ${ arg.name } );`;
        case "VecPT"       : return `_write_VecPT( ${ arg.name } );`;
        case "VecString"   : return `_write_VecString( ${ arg.name } );`;
        case "VecVecString": return `_write_VecVecString( ${ arg.name } );`;
        default            : return `bs.write_${ arg.type }( ${ trans_arg( arg ).map( arg => arg.name ).join( ', ' ) } );`;
    }
}

const cmds = read_cmds( process.argv[ 2 ] );

console.log( `// Generated file` );
console.log( `#pragma once` );

console.log( `` );
console.log( `#include <Hpipe/BinStreamWithOffsets.h>` );
console.log( `#include <unordered_map>` );
console.log( `#include "../Lexem.h"` );
console.log( `#include <deque>` );

// #define AST_TOK_
console.log( `` );
console.log( `#ifndef AST_TOK_NUMS` );
console.log( `#define AST_TOK_NUMS` );
cmds.forEach( ( cmd, ind ) => {
    console.log( `#define AST_TOK_${ cmd.name }_NUM ${ ind }` );
} );
console.log( `#endif // AST_TOK_NUMS` );

// write_
console.log( `` );
console.log( `struct AstWriter {` );
console.log( `    struct Apply { Hpipe::CmString attr; Vec<const Lexem *> args; Vec<Hpipe::CmString> arg_names; };` );
console.log( `    struct Waiting { PT *first; const Lexem *second; int flags; };` );
console.log( `    AstWriter() : bs( &cq ) {}` );
cmds.forEach( ( cmd, ind ) => {
    const tr = new Array<{type:string,name:string}>();
    for( const arg of cmd.args )
        tr.push( ...trans_arg( arg ) );
    let v = [];
    if ( cmd.nsrc )
        v.push( "const Lexem *lex" );
    v.push( ...tr.map( arg => `${ arg.type }${ arg.name }` ) );
    console.log( `    void write_${ cmd.name }( ${ v.join( ', ' ) }${ v.length ? ", int wr_flags = 0" : "" } ) {` );
    console.log( `        bs.write_byte( AST_TOK_${ cmd.name }_NUM );` );
    if ( cmd.nsrc )
        console.log( `        _write_src( lex );` );
    for( const arg of cmd.args )
        console.log( `        ${ bs_write( arg ) }` );
    console.log( `    }` );
} );
console.log( `    void _write_src( const Lexem *lex ) {` );
console.log( `        if ( ! lex || ! lex->src ) {` );
console.log( `            bs.write_unsigned( 0 );` );
console.log( `            return;` );
console.log( `        }` );
console.log( `        auto iter = src_map.find( lex->src );` );
console.log( `        if ( iter == src_map.end() )` );
console.log( `            iter = src_map.insert( iter, std::make_pair( lex->src, src_map.size() + 1 ) );` );
console.log( `        bs.write_unsigned( iter->second );` );
console.log( `        bs.write_unsigned( lex->off() );` );
console.log( `    }` );
console.log( `    ` );
console.log( `    void _write_VecPT( const Vec<size_t> &vl ) {` );
console.log( `        bs.write_unsigned( vl.size() );` );
console.log( `        for( size_t val : vl )` );
console.log( `            bs.write_unsigned( val );` );
console.log( `    }` );
console.log( `    void _write_VecString( const Vec<Hpipe::CmString> &vl ) {` );
console.log( `        bs.write_unsigned( vl.size() );` );
console.log( `        for( const Hpipe::CmString &str : vl )` );
console.log( `            bs << str;` );
console.log( `    }` );
console.log( `    void _write_VecVecString( const Vec<Vec<Hpipe::CmString>> &vl ) {` );
console.log( `        bs.write_unsigned( vl.size() );` );
console.log( `        for( const Vec<Hpipe::CmString> &vs : vl )` );
console.log( `            _write_VecString( vs );` );
console.log( `    }` );
console.log( `    void _write_VecApply( const Vec<Apply> &vl ) {` );
console.log( `        bs.write_unsigned( vl.size() );` );
console.log( `        for( const Apply &l : vl ) {` );
console.log( `            bs << l.attr;` );
console.log( `            _write_VecLex( l.args );` );
console.log( `            _write_VecString( l.arg_names );` );
console.log( `        }` );
console.log( `    }` );
console.log( `    void _write_VecLex( const Vec<const Lexem *> &vl, int wr_flags = 0 ) {` );
console.log( `        bs.write_unsigned( vl.size() );` );
console.log( `        for( const Lexem *l : vl )` );
console.log( `            _write_Lex( l, wr_flags );` );
console.log( `    }` );
console.log( `    void _write_Lex( const Lexem *l, int wr_flags = 0 ) {` );
console.log( `        if ( l )` );
console.log( `            ql.push_back( Waiting{ bs.new_offset(), l, wr_flags } );` );
console.log( `        else` );
console.log( `            bs.write_unsigned( 0 );` );
console.log( `    }` );
console.log( `    std::unordered_map<std::string,unsigned>    src_map;` );
console.log( `    Hpipe::BinStreamWithOffsets<Hpipe::CbQueue> bs;` );
console.log( `    Hpipe::CbQueue                              cq;` );
console.log( `    std::deque<Waiting>                         ql;` );
console.log( `};` );
