import read_cmds from "./ReadCmds"
declare var process: any;

function read_arg( arg: {type:string,name:string} ): string {
    switch ( arg.type ) {
        case "PI8"         : return `PI8 ${ arg.name } = bs.read_byte();`;
        case "SI32"        : return `SI32 ${ arg.name } = bs.read_signed();`;
        case "PI32"        : return `SI32 ${ arg.name } = bs.read_unsigned();`;
        case "Lex"         : return `Rc_string ${ arg.name } = read_Lex( bs );`;
        case "VecLex"      : return `Vec<Rc_string> ${ arg.name } = read_VecLex( bs );`;
        case "VecApply"    : return `Vec<AstVisitor::Apply> ${ arg.name } = read_VecApply( bs );`;
        case "String"      : return `Rc_string ${ arg.name } = read_String( bs );`;
        case "VecString"   : return `Vec<Rc_string> ${ arg.name } = read_VecString( bs );`;
        case "VecPT"       : return `Vec<size_t> ${ arg.name } = read_VecPT( bs );`;
        case "VecVecString": return `Vec<Vec<Rc_string>> ${ arg.name } = read_VecVecString( bs );`;
        default            : return `BIM;`;
    }
}

function sp( v, j = ', ', e = "" ) {
    return v.length ? ' ' + v.join( j ) + e + ' ' : '';
}

function disp_arg( arg: {type:string,name:string}, phase: number, sp: string, sep = null as {sep:string} ): string {
    function upd ( sep : {sep:string} ): string {
        const old = sep.sep;
        if ( old == "" )
            sep.sep = ",";
        return old;
    }
    switch ( arg.type ) {
        case "Lex"      : return phase ? `\n${ sp }os << sp << "${ arg.name }=\\n"; is(); os << sp; if ( ${ arg.name }.size() ) { ast_visit( *this, ${ arg.name } ); } else os << "null\\n"; ds();` : "";
        case "VecLex"   : return phase ? `\n${ sp }os << sp << "${ arg.name }=\\n"; is(); for( size_t i = 0; i < ${ arg.name }.size(); ++i ) { os << sp; if ( ${ arg.name }[ i ].size() ) { is(); ast_visit( *this, ${ arg.name }[ i ] ); ds(); } else os << "null\\n"; } ds();` : "";
        case "VecApply" : return phase ? `\n${ sp }os << sp << "${ arg.name }=\\n"; is(); for( size_t i = 0; i < ${ arg.name }.size(); ++i ) { os << sp << "Apply\\n"; } ds();` : "";
        case "PI8"      : return phase ? "" : `\n${ sp }os << "${ upd( sep ) }${ arg.name }=" << PI32( ${ arg.name } );`;
        default         : return phase ? "" : `\n${ sp }os << "${ upd( sep ) }${ arg.name }=" << ${ arg.name };`;
    }
    
}

function f_arg( type: string, name: string ) {
    switch ( type ) {
        case "Lex"         : return `Rc_string ${ name }`;
        case "VecLex"      : return `const Vec<Rc_string> &${ name }`;
        case "VecApply"    : return `const Vec<Apply> &${ name }`;
        case "VecPT"       : return `const Vec<size_t> &${ name }`;
        case "String"      : return `Rc_string ${ name }`;
        case "VecString"   : return `const Vec<Rc_string> &${ name }`;
        case "VecVecString": return `const Vec<Vec<Rc_string>> &${ name }`;
        default            : return `${ type } ${ name }`;
    }
}

const cmds = read_cmds( process.argv[ 2 ] );

let max_name_size = 0;
for( const cmd of cmds )
    max_name_size = Math.max( max_name_size, cmd.name.length );
function mn( s: string ): string {
    return s + ' '.repeat( max_name_size - s.length );
}

console.log( `// File generated using ${ process.argv.slice( 1 ) }` );
console.log( `#pragma once` );

console.log( `` );
console.log( `#include <Hpipe/BinStream.h>` );
console.log( `#include "../../System/Raii_save.h"` );
console.log( `#include "../../System/Rc_string.h"` );
console.log( `#include "../../System/Stream.h"` );

// #define AST_TOK_
console.log( `` );
console.log( `#ifndef AST_TOK_NUMS` );
console.log( `#define AST_TOK_NUMS` );
cmds.forEach( ( cmd, ind ) => {
    console.log( `#define AST_TOK_${ cmd.name }_NUM ${ ind }` );
} );
console.log( `#endif // AST_TOK_NUMS` );

// AstVisitor
console.log( `` );
console.log( `struct AstVisitor {` );
console.log( `    struct Apply { Rc_string attr; Vec<Rc_string> args, arg_names; };` );
console.log( `    AstVisitor() { rec = true; }` );
console.log( `    virtual ~AstVisitor() {}` );
console.log( `    virtual void set_src( size_t src, size_t off ) {}` );
cmds.forEach( ( cmd, ind ) => {
    // const tr = new Array<{type:string,name:string}>();
    // for( const arg of cmd.args )
    //     tr.push( ...trans_arg( arg ) );
    let v = cmd.args.map( arg => f_arg( arg.type, arg.name ) );
    let c = cmd.args.map( arg => arg.name );
    // v.push( ...tr.map( arg => `${ arg.type }${ arg.name }` ) );

    console.log( `    virtual bool enter_test_${ mn( cmd.name ) }(${ sp( v ) }) { enter_${ cmd.name }(${ sp( c ) }); return rec; }` );
    console.log( `    virtual void enter_${ mn( cmd.name ) }     (${ sp( v ) }) { default_enter( "${ mn( cmd.name ) }" ); }` );
    console.log( `    virtual void leave_${ mn( cmd.name ) }     () { default_leave( "${ mn( cmd.name ) }" ); }` );
} );
console.log( `    virtual void default_enter( const char *name ) {}` );
console.log( `    virtual void default_leave( const char *name ) {}` );
console.log( `    bool rec;` );
console.log( `};` );

// helpers
console.log( `inline Rc_string read_Lex( Hpipe::BinStream<Rc_string> bs ) {` );
console.log( `    char *ptr = bs.buf->ptr();` );
console.log( `    size_t off = bs.read_unsigned();` );
console.log( `    return off ? Rc_string{ *bs.buf, ptr + off, bs.buf->end() } : Rc_string{};` );
console.log( `}` );
console.log( `inline Vec<Rc_string> read_VecLex( Hpipe::BinStream<Rc_string> bs ) {` );
console.log( `    Vec<Rc_string> res;` );
console.log( `    res.resize( bs.read_unsigned() );` );
console.log( `    for( size_t i = 0; i < res.size(); ++i )` );
console.log( `        res[ i ] = read_Lex( bs );` );
console.log( `    return res;` );
console.log( `}` );
console.log( `inline Rc_string read_String( Hpipe::BinStream<Rc_string> bs ) {` );
console.log( `    size_t n = bs.read_unsigned();` );
console.log( `    Rc_string res{ *bs.buf, bs.buf->ptr(), bs.buf->ptr() + n };` );
console.log( `    bs.skip_some( n );` );
console.log( `    return res;` );
console.log( `}` );
console.log( `inline Vec<size_t> read_VecPT( Hpipe::BinStream<Rc_string> bs ) {` );
console.log( `    Vec<size_t> res;` );
console.log( `    res.resize( bs.read_unsigned() );` );
console.log( `    for( size_t i = 0; i < res.size(); ++i )` );
console.log( `        res[ i ] = bs.read_unsigned();` );
console.log( `    return res;` );
console.log( `}` );
console.log( `inline Vec<Rc_string> read_VecString( Hpipe::BinStream<Rc_string> bs ) {` );
console.log( `    Vec<Rc_string> res;` );
console.log( `    res.resize( bs.read_unsigned() );` );
console.log( `    for( size_t i = 0; i < res.size(); ++i )` );
console.log( `        res[ i ] = read_String( bs );` );
console.log( `    return res;` );
console.log( `}` );
console.log( `inline Vec<Vec<Rc_string>> read_VecVecString( Hpipe::BinStream<Rc_string> bs ) {` );
console.log( `    Vec<Vec<Rc_string>> res;` );
console.log( `    res.resize( bs.read_unsigned() );` );
console.log( `    for( size_t i = 0; i < res.size(); ++i )` );
console.log( `        res[ i ] = read_VecString( bs );` );
console.log( `    return res;` );
console.log( `}` );
console.log( `inline Vec<AstVisitor::Apply> read_VecApply( Hpipe::BinStream<Rc_string> bs ) {` );
console.log( `    Vec<AstVisitor::Apply> res;` );
console.log( `    res.resize( bs.read_unsigned() );` );
console.log( `    for( size_t i = 0; i < res.size(); ++i ) {` );
console.log( `        res[ i ].attr = read_String( bs );` );
console.log( `        res[ i ].args = read_VecLex( bs );` );
console.log( `        res[ i ].arg_names = read_VecString( bs );` );
console.log( `    }` );
console.log( `    return res;` );
console.log( `}` );

//
console.log( `inline void ast_visit( AstVisitor &visitor, Rc_string cm ) {` );
console.log( `    Hpipe::BinStream<Rc_string> bs( &cm );` );
console.log( `    PI8 cmd = bs.read_byte();` );
console.log( `    if ( bs.error() )` );
console.log( `        return;` );
console.log( `    switch ( cmd ) {` );
cmds.forEach( ( cmd, ind ) => {
    console.log( `    case ${ ind }: {` );
    if ( cmd.nsrc ) {
        console.log( `        if ( size_t nsrc = bs.read_unsigned() ) visitor.set_src( nsrc - 1, bs.read_unsigned() );` );
    }
    for( const arg of cmd.args )
        console.log( `        ${ read_arg( arg ) }` );
    let c = cmd.args.map( arg => arg.name );
    console.log( `        if ( ! visitor.enter_test_${ cmd.name }(${ sp( c ) }) ) break;` );
    for( const arg of cmd.args ) {
        switch ( arg.type ) {
            case 'Lex':
                console.log( `        ast_visit( visitor, ${ arg.name } );` );
                break;
            case 'VecLex':
                console.log( `        for( const Rc_string &cm : ${ arg.name } )` );
                console.log( `            ast_visit( visitor, cm );` );
                break;
            case 'VecApply':
                console.log( `        for( const AstVisitor::Apply &ap : ${ arg.name } )` );
                console.log( `            for( const Rc_string &arg : ap.args )` );
                console.log( `                ast_visit( visitor, arg );` );
                break;
        }
    }
    console.log( `        visitor.leave_${ cmd.name }();` );
    console.log( `        break;` );
    console.log( `    }` );
} );
console.log( `    }` );
console.log( `}` );

console.log( `inline void ast_visit_VecLex( AstVisitor &visitor, Rc_string cm ) {` );
console.log( `    Hpipe::BinStream<Rc_string> bs( &cm );` );
console.log( `    for( size_t n = bs.read_unsigned(); n--; )` );
console.log( `        ast_visit( visitor, read_Lex( bs ) );` );
console.log( `}` );

// AstVisitorDisplay
console.log( `` );
console.log( `struct AstVisitorDisplay : public AstVisitor {` );
console.log( `    AstVisitorDisplay( std::ostream &os, std::string sp = "" ) : os( os ), sp( sp ) {}` );
cmds.forEach( ( cmd, ind ) => {
    let v = cmd.args.map( arg => f_arg( arg.type, arg.name ) );
    let c = `\n        os << "${ cmd.name }(";`;
    let d = ``;
    for( const arg of cmd.args )
        d += disp_arg( arg, 1, "        " );
    let sep = {sep:""};
    for( const arg of cmd.args )
        c += disp_arg( arg, 0, "        ", sep );
    c += `\n        os << ")";`;
    if ( d.length ) {
        c += `\n        Raii_save<std::string> rs( sp, sp + "  " );`;
        c += `\n        os << "\\n";${ d }`;
    } else
        c += `\n        os << "\\n";`;
    console.log( `    bool enter_test_${ cmd.name }(${ sp( v ) }) override {${ c }\n        return false;\n    }` );
} );
console.log( `    void is( int n = 2 ) { while( n-- ) sp += ' '; }` );
console.log( `    void ds( int n = 2 ) { sp.resize( sp.size() - n ); }` );
console.log( `    std::ostream &os;` );
console.log( `    std::string   sp;` );
console.log( `};` );
