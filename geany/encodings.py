import _geany_encodings


ISO_8859_1 = 0
ISO_8859_2 = 1
ISO_8859_3 = 2
ISO_8859_4 = 3
ISO_8859_5 = 4
ISO_8859_6 = 5
ISO_8859_7 = 6
ISO_8859_8 = 7
ISO_8859_8_I = 8
ISO_8859_9 = 9
ISO_8859_10 = 10
ISO_8859_13 = 11
ISO_8859_14 = 12
ISO_8859_15 = 13
ISO_8859_16 = 14

UTF_7 = 15
UTF_8 = 16
UTF_16LE = 17
UTF_16BE = 18
UCS_2LE = 19
UCS_2BE = 20
UTF_32LE = 21
UTF_32BE = 22

ARMSCII_8 = 23
BIG5 = 24
BIG5_HKSCS = 25
CP_866 = 26

EUC_JP = 27
EUC_KR = 28
EUC_TW = 29

GB18030 = 30
GB2312 = 31
GBK = 32
GEOSTD8 = 33
HZ = 34

IBM_850 = 35
IBM_852 = 36
IBM_855 = 37
IBM_857 = 38
IBM_862 = 39
IBM_864 = 40

ISO_2022_JP = 41
ISO_2022_KR = 42
ISO_IR_111 = 43
JOHAB = 44
KOI8_R = 45
KOI8_U = 46

SHIFT_JIS = 47
TCVN = 48
TIS_620 = 49
UHC = 50
VISCII = 51

WINDOWS_1250 = 52
WINDOWS_1251 = 53
WINDOWS_1252 = 54
WINDOWS_1253 = 55
WINDOWS_1254 = 56
WINDOWS_1255 = 57
WINDOWS_1256 = 58
WINDOWS_1257 = 59
WINDOWS_1258 = 60

NONE = 61
CP_932 = 62

MAX = 63


def convert_to_utf8(buf, size=-1):
    return _geany_encodings.convert_to_utf8(buf, size)

def convert_to_utf8_from_charset(buf, charset, size=-1, fast=False):
    return _geany_encodings.convert_to_utf8_from_charset(buf, charset, size, fast)

def get_charset_from_index(index):
    return _geany_encodings.get_charset_from_index(index)

def get_charsets_list():
    _encodings = []
    for i in range(0, MAX):
        enc = get_charset_from_index(i)
        if enc is not None:
            _encodings.append(enc)
    return _encodings
