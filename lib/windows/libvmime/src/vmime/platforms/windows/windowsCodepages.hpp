// This file is auto-generated by Converter.exe

#pragma once

namespace vmime     {
namespace platforms {
namespace windows   {

class windowsCodepages
{
public:
    static int getByName(const char* s8_Name)
    {
        if (stricmp(s8_Name, "asmo-708")          == 0) return 708;
        if (stricmp(s8_Name, "big5")              == 0) return 950;
        if (stricmp(s8_Name, "cp1025")            == 0) return 21025;
        if (stricmp(s8_Name, "cp866")             == 0) return 866;
        if (stricmp(s8_Name, "cp875")             == 0) return 875;
        if (stricmp(s8_Name, "dos-720")           == 0) return 720;
        if (stricmp(s8_Name, "dos-862")           == 0) return 862;
        if (stricmp(s8_Name, "euc-cn")            == 0) return 51936;
        if (stricmp(s8_Name, "euc-jp")            == 0) return 51932;
        if (stricmp(s8_Name, "euc-kr")            == 0) return 51949;
        if (stricmp(s8_Name, "gb18030")           == 0) return 54936;
        if (stricmp(s8_Name, "gb2312")            == 0) return 936;
        if (stricmp(s8_Name, "gbk")               == 0) return 936;
        if (stricmp(s8_Name, "hz-gb-2312")        == 0) return 52936;
        if (stricmp(s8_Name, "ibm00858")          == 0) return 858;
        if (stricmp(s8_Name, "ibm00924")          == 0) return 20924;
        if (stricmp(s8_Name, "ibm01047")          == 0) return 1047;
        if (stricmp(s8_Name, "ibm01140")          == 0) return 1140;
        if (stricmp(s8_Name, "ibm01141")          == 0) return 1141;
        if (stricmp(s8_Name, "ibm01142")          == 0) return 1142;
        if (stricmp(s8_Name, "ibm01143")          == 0) return 1143;
        if (stricmp(s8_Name, "ibm01144")          == 0) return 1144;
        if (stricmp(s8_Name, "ibm01145")          == 0) return 1145;
        if (stricmp(s8_Name, "ibm01146")          == 0) return 1146;
        if (stricmp(s8_Name, "ibm01147")          == 0) return 1147;
        if (stricmp(s8_Name, "ibm01148")          == 0) return 1148;
        if (stricmp(s8_Name, "ibm01149")          == 0) return 1149;
        if (stricmp(s8_Name, "ibm037")            == 0) return 37;
        if (stricmp(s8_Name, "ibm1026")           == 0) return 1026;
        if (stricmp(s8_Name, "ibm273")            == 0) return 20273;
        if (stricmp(s8_Name, "ibm277")            == 0) return 20277;
        if (stricmp(s8_Name, "ibm278")            == 0) return 20278;
        if (stricmp(s8_Name, "ibm280")            == 0) return 20280;
        if (stricmp(s8_Name, "ibm284")            == 0) return 20284;
        if (stricmp(s8_Name, "ibm285")            == 0) return 20285;
        if (stricmp(s8_Name, "ibm290")            == 0) return 20290;
        if (stricmp(s8_Name, "ibm297")            == 0) return 20297;
        if (stricmp(s8_Name, "ibm420")            == 0) return 20420;
        if (stricmp(s8_Name, "ibm423")            == 0) return 20423;
        if (stricmp(s8_Name, "ibm424")            == 0) return 20424;
        if (stricmp(s8_Name, "ibm437")            == 0) return 437;
        if (stricmp(s8_Name, "ibm500")            == 0) return 500;
        if (stricmp(s8_Name, "ibm737")            == 0) return 737;
        if (stricmp(s8_Name, "ibm775")            == 0) return 775;
        if (stricmp(s8_Name, "ibm850")            == 0) return 850;
        if (stricmp(s8_Name, "ibm852")            == 0) return 852;
        if (stricmp(s8_Name, "ibm855")            == 0) return 855;
        if (stricmp(s8_Name, "ibm857")            == 0) return 857;
        if (stricmp(s8_Name, "ibm860")            == 0) return 860;
        if (stricmp(s8_Name, "ibm861")            == 0) return 861;
        if (stricmp(s8_Name, "ibm863")            == 0) return 863;
        if (stricmp(s8_Name, "ibm864")            == 0) return 864;
        if (stricmp(s8_Name, "ibm865")            == 0) return 865;
        if (stricmp(s8_Name, "ibm869")            == 0) return 869;
        if (stricmp(s8_Name, "ibm870")            == 0) return 870;
        if (stricmp(s8_Name, "ibm871")            == 0) return 20871;
        if (stricmp(s8_Name, "ibm880")            == 0) return 20880;
        if (stricmp(s8_Name, "ibm905")            == 0) return 20905;
        if (stricmp(s8_Name, "ibm-thai")          == 0) return 20838;
        if (stricmp(s8_Name, "iso-2022-jp")       == 0) return 50222;
        if (stricmp(s8_Name, "iso2022-jp")        == 0) return 50222;
        if (stricmp(s8_Name, "iso-2022-kr")       == 0) return 50225;
        if (stricmp(s8_Name, "iso2022-kr")        == 0) return 50225;
        if (stricmp(s8_Name, "iso-8859-1")        == 0) return 28591;
        if (stricmp(s8_Name, "iso8859-1")         == 0) return 28591;
        if (stricmp(s8_Name, "iso-8859-13")       == 0) return 28603;
        if (stricmp(s8_Name, "iso8859-13")        == 0) return 28603;
        if (stricmp(s8_Name, "iso-8859-15")       == 0) return 28605;
        if (stricmp(s8_Name, "iso8859-15")        == 0) return 28605;
        if (stricmp(s8_Name, "iso-8859-2")        == 0) return 28592;
        if (stricmp(s8_Name, "iso8859-2")         == 0) return 28592;
        if (stricmp(s8_Name, "iso-8859-3")        == 0) return 28593;
        if (stricmp(s8_Name, "iso8859-3")         == 0) return 28593;
        if (stricmp(s8_Name, "iso-8859-4")        == 0) return 28594;
        if (stricmp(s8_Name, "iso8859-4")         == 0) return 28594;
        if (stricmp(s8_Name, "iso-8859-5")        == 0) return 28595;
        if (stricmp(s8_Name, "iso8859-5")         == 0) return 28595;
        if (stricmp(s8_Name, "iso-8859-6")        == 0) return 28596;
        if (stricmp(s8_Name, "iso8859-6")         == 0) return 28596;
        if (stricmp(s8_Name, "iso-8859-7")        == 0) return 28597;
        if (stricmp(s8_Name, "iso8859-7")         == 0) return 28597;
        if (stricmp(s8_Name, "iso-8859-8")        == 0) return 28598;
        if (stricmp(s8_Name, "iso8859-8")         == 0) return 28598;
        if (stricmp(s8_Name, "iso-8859-8-i")      == 0) return 38598;
        if (stricmp(s8_Name, "iso8859-8-i")       == 0) return 38598;
        if (stricmp(s8_Name, "iso-8859-9")        == 0) return 28599;
        if (stricmp(s8_Name, "iso8859-9")         == 0) return 28599;
        if (stricmp(s8_Name, "johab")             == 0) return 1361;
        if (stricmp(s8_Name, "koi8-r")            == 0) return 20866;
        if (stricmp(s8_Name, "koi8-u")            == 0) return 21866;
        if (stricmp(s8_Name, "ks_c_5601-1987")    == 0) return 949;
        if (stricmp(s8_Name, "macintosh")         == 0) return 10000;
        if (stricmp(s8_Name, "unicodefffe")       == 0) return 1201;
        if (stricmp(s8_Name, "us-ascii")          == 0) return 20127;
        if (stricmp(s8_Name, "utf-16")            == 0) return 1200;
        if (stricmp(s8_Name, "utf16")             == 0) return 1200;
        if (stricmp(s8_Name, "utf-32")            == 0) return 12000;
        if (stricmp(s8_Name, "utf32")             == 0) return 12000;
        if (stricmp(s8_Name, "utf-32be")          == 0) return 12001;
        if (stricmp(s8_Name, "utf32be")           == 0) return 12001;
        if (stricmp(s8_Name, "utf-7")             == 0) return 65000;
        if (stricmp(s8_Name, "utf7")              == 0) return 65000;
        if (stricmp(s8_Name, "utf-8")             == 0) return 65001;
        if (stricmp(s8_Name, "utf8")              == 0) return 65001;
        if (stricmp(s8_Name, "windows-1250")      == 0) return 1250;
        if (stricmp(s8_Name, "windows1250")       == 0) return 1250;
        if (stricmp(s8_Name, "windows-1251")      == 0) return 1251;
        if (stricmp(s8_Name, "windows1251")       == 0) return 1251;
        if (stricmp(s8_Name, "windows-1252")      == 0) return 1252;
        if (stricmp(s8_Name, "windows1252")       == 0) return 1252;
        if (stricmp(s8_Name, "windows-1253")      == 0) return 1253;
        if (stricmp(s8_Name, "windows1253")       == 0) return 1253;
        if (stricmp(s8_Name, "windows-1254")      == 0) return 1254;
        if (stricmp(s8_Name, "windows1254")       == 0) return 1254;
        if (stricmp(s8_Name, "windows-1255")      == 0) return 1255;
        if (stricmp(s8_Name, "windows1255")       == 0) return 1255;
        if (stricmp(s8_Name, "windows-1256")      == 0) return 1256;
        if (stricmp(s8_Name, "windows1256")       == 0) return 1256;
        if (stricmp(s8_Name, "windows-1257")      == 0) return 1257;
        if (stricmp(s8_Name, "windows1257")       == 0) return 1257;
        if (stricmp(s8_Name, "windows-1258")      == 0) return 1258;
        if (stricmp(s8_Name, "windows1258")       == 0) return 1258;
        if (stricmp(s8_Name, "windows-874")       == 0) return 874;
        if (stricmp(s8_Name, "windows874")        == 0) return 874;
        if (stricmp(s8_Name, "x-chinese-cns")     == 0) return 20000;
        if (stricmp(s8_Name, "x-chinese-eten")    == 0) return 20002;
        if (stricmp(s8_Name, "x-cp20001")         == 0) return 20001;
        if (stricmp(s8_Name, "x-cp20003")         == 0) return 20003;
        if (stricmp(s8_Name, "x-cp20004")         == 0) return 20004;
        if (stricmp(s8_Name, "x-cp20005")         == 0) return 20005;
        if (stricmp(s8_Name, "x-cp20261")         == 0) return 20261;
        if (stricmp(s8_Name, "x-cp20269")         == 0) return 20269;
        if (stricmp(s8_Name, "x-cp20936")         == 0) return 20936;
        if (stricmp(s8_Name, "x-cp20949")         == 0) return 20949;
        if (stricmp(s8_Name, "x-cp50227")         == 0) return 50227;
        if (stricmp(s8_Name, "x-ebcdic-koreanextended") == 0) return 20833;
        if (stricmp(s8_Name, "x-europa")          == 0) return 29001;
        if (stricmp(s8_Name, "x-ia5")             == 0) return 20105;
        if (stricmp(s8_Name, "x-ia5-german")      == 0) return 20106;
        if (stricmp(s8_Name, "x-ia5-norwegian")   == 0) return 20108;
        if (stricmp(s8_Name, "x-ia5-swedish")     == 0) return 20107;
        if (stricmp(s8_Name, "x-iscii-as")        == 0) return 57006;
        if (stricmp(s8_Name, "x-iscii-be")        == 0) return 57003;
        if (stricmp(s8_Name, "x-iscii-de")        == 0) return 57002;
        if (stricmp(s8_Name, "x-iscii-gu")        == 0) return 57010;
        if (stricmp(s8_Name, "x-iscii-ka")        == 0) return 57008;
        if (stricmp(s8_Name, "x-iscii-ma")        == 0) return 57009;
        if (stricmp(s8_Name, "x-iscii-or")        == 0) return 57007;
        if (stricmp(s8_Name, "x-iscii-pa")        == 0) return 57011;
        if (stricmp(s8_Name, "x-iscii-ta")        == 0) return 57004;
        if (stricmp(s8_Name, "x-iscii-te")        == 0) return 57005;
        if (stricmp(s8_Name, "x-mac-arabic")      == 0) return 10004;
        if (stricmp(s8_Name, "x-mac-ce")          == 0) return 10029;
        if (stricmp(s8_Name, "x-mac-chinesesimp") == 0) return 10008;
        if (stricmp(s8_Name, "x-mac-chinesetrad") == 0) return 10002;
        if (stricmp(s8_Name, "x-mac-croatian")    == 0) return 10082;
        if (stricmp(s8_Name, "x-mac-cyrillic")    == 0) return 10007;
        if (stricmp(s8_Name, "x-mac-greek")       == 0) return 10006;
        if (stricmp(s8_Name, "x-mac-hebrew")      == 0) return 10005;
        if (stricmp(s8_Name, "x-mac-icelandic")   == 0) return 10079;
        if (stricmp(s8_Name, "x-mac-japanese")    == 0) return 10001;
        if (stricmp(s8_Name, "x-mac-korean")      == 0) return 10003;
        if (stricmp(s8_Name, "x-mac-romanian")    == 0) return 10010;
        if (stricmp(s8_Name, "x-mac-thai")        == 0) return 10021;
        if (stricmp(s8_Name, "x-mac-turkish")     == 0) return 10081;
        if (stricmp(s8_Name, "x-mac-ukrainian")   == 0) return 10017;

        throw exception(std::string("Unknown charset: ") + s8_Name);
    }
};

} // windows
} // platforms
} // vmime
