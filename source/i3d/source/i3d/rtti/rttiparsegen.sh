clear
bison -o RttiParser.cpp -d -l -p rttiparse_ rtti_parser.y 
flex -oRttiLexer.cpp -Prttilex_  rtti_lexer.l