import * as fs from 'fs'

export
interface Cmd {
    doc : Array<string>;
    name: string;
    nsrc: boolean; ///< need src
    args: Array<{type:string,name:string}>;
};

export default
function read_cmds( filename: string ): Array<Cmd> {
    let cmds = new Array<Cmd>(), doc = new Array<string>();

    for( let line of fs.readFileSync( filename ).toString().split( "\n" ) ) {
        line = line.trim();
        if ( line.length == 0 ) {
            doc.length = 0;
            continue;
        }
        if ( line.startsWith( "#" ) ) {
            doc.push( line.slice( 1 ).trim() );
            continue;
        }
        let s = line.split( ' ' ).filter( s => s.length != 0 );
        cmds.push({
            doc: [ ...doc ],
            name: s[ 0 ].trim(),
            nsrc: Boolean( Number( s[ 1 ] ) ),
            args: s.slice( 2 ).map( arg => ({
                type: arg.split( ':' )[ 1 ],
                name: arg.split( ':' )[ 0 ]
            }) ),
        })
        doc.length = 0;
    }

    return cmds;
}
