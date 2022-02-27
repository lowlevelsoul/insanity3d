echo off
set PATH=%PATH%;Y:\work\libs\WinFlexBison
win_bison -o RttiParser.cpp -d -l -p rttiparse_ rtti_parser.y 
win_flex --wincompat -oRttiLexer.cpp -Prttilex_  rtti_lexer.l