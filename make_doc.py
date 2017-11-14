from pygments.token import Keyword, Name, Comment, String, Error, Number, Operator, Generic
from pygments.formatters import HtmlFormatter
from pygments.lexer import RegexLexer, include
from pygments.style import Style
from pygments.token import Token
import pygments.unistring as uni
import pygments
import re

JS_IDENT_START = ('(?:[$_' + uni.combine('Lu', 'Ll', 'Lt', 'Lm', 'Lo', 'Nl') +
                  ']|\\\\u[a-fA-F0-9]{4})')
JS_IDENT_PART = ('(?:[$' + uni.combine('Lu', 'Ll', 'Lt', 'Lm', 'Lo', 'Nl',
                                       'Mn', 'Mc', 'Nd', 'Pc') +
                 u'\u200c\u200d]|\\\\u[a-fA-F0-9]{4})')
JS_IDENT = JS_IDENT_START + '(?:' + JS_IDENT_PART + ')*'

# http://pygments.org/docs/tokens/
class SaneLexer(RegexLexer):
    name = 'Sane'
    aliases = ['sane']
    filenames = ['*.sane','*.met']
    #  flags = re.DOTALL | re.UNICODE | re.MULTILINE
    flags = re.UNICODE | re.MULTILINE

    tokens = {
        'root': [
            ( r'\s+', Token.Text ),

            ( r'#.*?\n', Token.Comment.Single ),
            ( r'#~.*?~#', Token.Comment.Multiline ),

            ( r'(\.\d+|[0-9]+\.[0-9]*)([eE][-+]?[0-9]+)?', Token.Number.Float ),
            ( r'0[bB][01]+', Token.Number.Bin ),
            ( r'0[oO][0-7]+', Token.Number.Oct ),
            ( r'0[xX][0-9a-fA-F]+', Token.Number.Hex ),
            ( r'[0-9]+', Token.Number.Integer ),

            ( r'(def|class)\b', Token.Keyword.Declaration ),
            ( r'(raii|enum|init_of|if|else|while|for|if_ws|while_ws|for_ws|return|try|catch|throw|continue|break|wpc)\b', Token.Keyword ),

            ( r'(switchable|private|protected|import|from|export|mixin|with|wfp|when|pertinence|virtual|static|extends|inline|throw|and|or|xor|not|in|abstract|global|ref|override|is_a|is_not_a)\b', Token.Keyword ),
            ( r'(true|false|null|NaN|Infinity|undefined)\b', Token.Keyword.Constant),
            ( r'(sizeof|sizeof_bits|typeof|load|aligof|aligof_bits|assert|operator|convert|destroy|construct|info|infon|move|__arguments|null_ptr|super|globals|self|this|pass|true|false|current_scope)\b', Token.Name.Builtin),
    
            ( r'"', Token.String.Double, 'interp' ),

            ( JS_IDENT, Token.Name.Variable ),

            ( r'\+\+|--|~|&&|\?|:|\|\||\\(?=\n)|'
              r'(<<|>>>?|==?|!=?|[-<>+:*%&|^/])=?', Token.Operator ),

            ( r'[{(\[;,]', Token.Punctuation ),
            ( r'[})\].]', Token.Punctuation ),
        ],
        'interp': [
            ( r'"', Token.String.Double, '#pop' ),
            ( r'\\\\', Token.String.Double ),
            ( r'\\"', Token.String.Double ),
            ( r'\$\{', Token.String.Interpol, 'interp-inside' ),
            ( r'\$', Token.String.Double ),
            ( r'[^"\\$]+', Token.String.Double ),
        ],
        'interp-inside': [
            # TODO: should this include single-line comments and allow nesting strings?
            ( r'\}', Token.String.Interpol, '#pop' ),
            include('root'),
        ],
    }

# http://pygments.org/docs/tokens/
class SaneStyle(Style):
    default_style = ""
    styles = {
        Comment:                'italic #938d7f',
        Keyword:                '#005782',
        Keyword.Declaration:    '#00aaff',
        Operator:               'bold #6d3642',
        Name:                   '#000', 
        Name.Function:          '#0f0',
        Name.Class:             'bold #0f0',
        String.Double:          '#181',
        Number:                 '#93488f',
    }

c = file( "_README.md" ).read()
o = open( "README.md","w" ) 

l = c.split( "```python" )
o.write( l[ 0 ] )
del l[ 0 ]
for i in l:
    m = i.split( "```" )
    o.write( pygments.highlight( m[ 0 ], SaneLexer(), HtmlFormatter( style = SaneStyle, noclasses = True ) ) )
    o.write( m[ 1 ] )
