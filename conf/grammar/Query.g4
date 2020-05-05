/*
 * This section of the grammar defines all parser rules and import the lexer
 * rules.
 */
grammar Query
    ;
import lexerGrammar
    ;

options {
    language = Cpp;
}


query: units (association units)* EOF;

units: unit*;

unit
    : word
    | regexword
    | optionset
    | orderset
    | dictset
    | qmark
    | plus
    | asterisk
    ;

optionset
    : LBRACKET_TOKEN (regexword | word | phrase)* RBRACKET_TOKEN
    ;
orderset
    : LBRACE_TOKEN (regexword | word | phrase)* RBRACE_TOKEN
    ;
dictset: HASH_TOKEN word;
phrase
    : DBLQUOTE_TOKEN (regexword | word)* DBLQUOTE_TOKEN
    ;

asterisk: ASTERISK_TOKEN;
regexword: REGEXWORD_TOKEN;
word: WORD_TOKEN;
qmark: QMARK_TOKEN;
plus: PLUS_TOKEN;
association: ASSOCIATION_TOKEN;
