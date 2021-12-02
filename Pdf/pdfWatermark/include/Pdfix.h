////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 PDFix (http://pdfix.net). All Rights Reserved.
// This file was generated automatically
////////////////////////////////////////////////////////////////////////////////
#ifndef _Pdfix_h
#define _Pdfix_h

#include <stdint.h>
#include <vector>
#include <stdexcept>
#ifdef _WIN32
#include <Windows.h>
#endif

// Unwanted definition of GetObject macro on Windows
#ifdef GetObject
#undef GetObject
#endif
#ifndef _NOEXCEPT
#define _NOEXCEPT throw()
#endif

#define PDFIX_VERSION_MAJOR 6
#define PDFIX_VERSION_MINOR 16
#define PDFIX_VERSION_PATCH 0
#define MAX_INT 2147483647
#define MIN_INT -2147483647
#define _in_
#define _out_
#define _callback_

namespace PDFixSDK {

struct PdsObject;
struct PdsBoolean;
struct PdsNumber;
struct PdsString;
struct PdsName;
struct PdsArray;
struct PdsDictionary;
struct PdsStream;
struct PdsNull;
struct PdsContent;
struct PdsPageObject;
struct PdsText;
struct PdsForm;
struct PdsPath;
struct PdsPathPoint;
struct PdsSoftMask;
struct PdsImage;
struct PdsShading;
struct PdsContentMark;
struct PdsTextRun;
struct PdsElement;
struct PdsWord;
struct PdsWordList;
struct PdeElement;
struct PdeContainer;
struct PdeList;
struct PdeToc;
struct PdeAnnot;
struct PdeFormField;
struct PdeImage;
struct PdeLine;
struct PdeRect;
struct PdeHeader;
struct PdeFooter;
struct PdeCell;
struct PdeTable;
struct PdeTextRun;
struct PdeWord;
struct PdeTextLine;
struct PdeText;
struct PdfColorSpace;
struct PdfColor;
struct PdfAction;
struct PdfActionHandler;
struct PdfAnnot;
struct PdfLinkAnnot;
struct PdfMarkupAnnot;
struct PdfTextAnnot;
struct PdfTextMarkupAnnot;
struct PdfWidgetAnnot;
struct PdfAnnotHandler;
struct PdfViewDestination;
struct PdfSecurityHandler;
struct PdfStandardSecurityHandler;
struct PdfCustomSecurityHandler;
struct PdfBaseDigSig;
struct PdfDigSig;
struct PdfCertDigSig;
struct PdfCustomDigSig;
struct PdfDocUndo;
struct PdfDoc;
struct PdfDocTemplate;
struct PdfPageTemplate;
struct PdfAlternate;
struct PdfHtmlAlternate;
struct PdfFont;
struct PdfFormField;
struct PdfPage;
struct PdePageMap;
struct PdfPageView;
struct PdfBookmark;
struct PdfNameTree;
struct PsRegex;
struct PsStream;
struct PsFileStream;
struct PsMemoryStream;
struct PsCustomStream;
struct PdsStructElement;
struct PdsClassMap;
struct PdsRoleMap;
struct PdsStructTree;
struct PsMetadata;
struct PsEvent;
struct PsAuthorization;
struct PsAccountAuthorization;
struct PsStandardAuthorization;
struct PsCommand;
struct PsRenderDeviceContext;
struct PsImage;
struct PsSysFont;
struct Pdfix;
struct PdfixPlugin;

typedef int PdfErrorType;
typedef int PdfSaveFlags;
typedef int PdfDocFlags;
typedef int PdfPageFlags;
typedef int PdfAnnotFlags;
typedef int PdfRemoveAnnotFlags;
typedef int PdfTextStateFlag;
typedef int PdfFieldFlags;
typedef int PdfRenderFlags;
typedef int PdfFontFlags;
typedef int PdfPageContentFlags;
typedef int PdfFontCreateFlags;
typedef int PdfTableType;
typedef int PdfWordFlags;
typedef int PdfTextLineFlags;
typedef int PdfTextFlags;
typedef int PdfElementFlags;
typedef int PdfPageInsertFlags;
typedef int PdsContentFlags;
typedef int PdfStateFlags;
typedef int PdfUserPermissionFlags;

typedef enum {
    kAuthPlatformWin = 0,
    kAuthPlatformMac = 1,
    kAuthPlatformLinux = 2,
    kAuthPlatformAndroid = 3,
    kAuthPlatformiOS = 4,
    kAuthPlatformServer = 5,
} PdfAuthPlatform;

typedef enum {
    kAuthOptionBasic = 0,
    kAuthOptionProfessional = 1,
    kAuthOptionEnterprise = 2,
    kAuthOptionDeveloper = 3,
    kAuthOptionTrial = 4,
    kAuthOptionLite = 5,
} PdfAuthOption;

typedef enum {
    kEventUnknown = 0,
    kEventDocWillSave = 1,
    kEventDocWillClose = 2,
    kEventDocDidOpen = 3,
    kEventDocDidSave = 4,
    kEventDocWillChangePages = 5,
    kEventDocDidChangePages = 6,
    kEventDocWillDeletePages = 7,
    kEventDocDidDeletePages = 8,
    kEventDocWillInsertPages = 9,
    kEventDocDidInsertPages = 10,
    kEventDocWillMovePages = 11,
    kEventDocDidMovePages = 12,
    kEventDocWillReplacePages = 13,
    kEventDocDidReplacePages = 14,
    kEventDocWillChangeFlags = 15,
    kEventDocDidChangeFlags = 16,
    kEventAnnotWillChange = 17,
    kEventAnnotDidChange = 18,
    kEventPageWillAddAnnot = 19,
    kEventPageWillRemoveAnnot = 20,
    kEventPageDidAddAnnot = 21,
    kEventPageDidRemoveAnnot = 22,
    kEventFormFieldWillChange = 23,
    kEventFormFieldDidChange = 24,
    kEventProgressDidChange = 25,
    kEventBookmarkWillChange = 26,
    kEventBookmarkDidChange = 27,
    kEventBookmarkWillRemove = 28,
    kEventBookmarkDidRemove = 29,
    kEventBookmarkDidCreate = 30,
    kEventBookmarkDidChangePosition = 31,
    kEventUndoDidCreate = 32,
    kEventUndoWillExecute = 33,
    kEventUndoDidExecute = 34,
    kEventUndoWillDestroy = 35,
    kEventPageMapWillChange = 36,
    kEventPageMapDidChange = 37,
    kEventStructTreeWillCreate = 38,
    kEventStructTreeDidCreate = 39,
    kEventStructTreeWillRemove = 40,
    kEventStructTreeDidRemove = 41,
    kEventStructElementWillAdd = 42,
    kEventStructElementDidAdd = 43,
    kEventStructElementWillChange = 44,
    kEventStructElementDidChange = 45,
    kEventStructElementChildWillRemove = 46,
    kEventStructElementChildDidRemove = 47,
    kEventDocTemplateWillChange = 48,
    kEventDocTemplateDidChange = 49,
    kEventDidCreate = 50,
    kEventWillDestroy = 51,
    kEventWillChange = 52,
    kEventDidChange = 53,
} PdfEventType;

enum {
    kSaveIncremental = 0x00,
    kSaveFull = 0x01,
    kSaveUncompressed = 0x02,
    kSaveCompressedStructureOnly = 0x04,
    kSaveIncludeComments = 0x0100,
} ;

enum {
    kDocNeedsSave = 0x01,
    kDocNeedsFullSave = 0x02,
    kDocIsModified = 0x04,
    kDocIsClosing = 0x08,
} ;

enum {
    kPageContentIsModified = 0x01,
} ;

typedef enum {
    kDigSigBlank = 0,
    kDigSigUnknown = 1,
    kDigSigInvalid = 2,
    kDigSigValid = 3,
    kDigSigDoubleChecked = 4,
    kDigSigValidStateEnumSize = 5,
} PdfDigSigValidState;

typedef enum {
    kAlignmentNone = 0,
    kAlignmentLeft = 1,
    kAlignmentRight = 2,
    kAlignmentJustify = 3,
    kAlignmentTop = 4,
    kAlignmentBottom = 5,
    kAlignmentCenter = 6,
} PdfAlignment;

typedef enum {
    kRotate0 = 0,
    kRotate90 = 90,
    kRotate180 = 180,
    kRotate270 = 270,
} PdfRotate;

typedef enum {
    kPdsUnknown = 0,
    kPdsBoolean = 1,
    kPdsNumber = 2,
    kPdsString = 3,
    kPdsName = 4,
    kPdsArray = 5,
    kPdsDictionary = 6,
    kPdsStream = 7,
    kPdsNull = 8,
    kPdsReference = 9,
} PdfObjectType;

typedef enum {
    kPdsPageUnknown = 0,
    kPdsPageText = 1,
    kPdsPagePath = 2,
    kPdsPageImage = 3,
    kPdsPageShading = 4,
    kPdsPageForm = 5,
} PdfPageObjectType;

typedef enum {
    kPdeUnknown = 0,
    kPdeText = 1,
    kPdeTextLine = 2,
    kPdeWord = 3,
    kPdeTextRun = 4,
    kPdeImage = 5,
    kPdeContainer = 6,
    kPdeList = 7,
    kPdeLine = 8,
    kPdeRect = 9,
    kPdeTable = 10,
    kPdeCell = 11,
    kPdeToc = 12,
    kPdeFormField = 13,
    kPdeHeader = 14,
    kPdeFooter = 15,
    kPdeAnnot = 16,
} PdfElementType;

typedef enum {
    kPdeContainerUnknown = 0,
    kPdeContainerPage = 1,
    kPdeContainerArt = 2,
    kPdeContainerCol = 3,
    kPdeContainerRow = 4,
    kPdeContainerSplitter = 5,
} PdfContainerType;

typedef enum {
    kTagUnknown = 0,
    kTagSect = 1,
    kTagArt = 2,
} PdfTagType;

typedef enum {
    kPdfLineCapButt = 0,
    kPdfLineCapRound = 1,
    kPdfLineCapSquare = 2,
} PdfLineCap;

typedef enum {
    kPdfLineJoinMiter = 0,
    kPdfLineJoinRound = 1,
    kPdfLineJoinBevel = 2,
} PdfLineJoin;

typedef enum {
    kFillTypeNone = 0,
    kFillTypeSolid = 1,
    kFillTypePattern = 2,
} PdfFillType;

typedef enum {
    kTextAlignmentNone = 0,
    kTextAlignmentLeft = 1,
    kTextAlignmentRight = 2,
    kTextAlignmentCenter = 3,
    kTextAlignmentJustify = 4,
} PdfTextAlignment;

typedef enum {
    kAnnotUnknown = 0,
    kAnnotText = 1,
    kAnnotLink = 2,
    kAnnotFreeText = 3,
    kAnnotLine = 4,
    kAnnotSquare = 5,
    kAnnotCircle = 6,
    kAnnotPolygon = 7,
    kAnnotPolyLine = 8,
    kAnnotHighlight = 9,
    kAnnotUnderline = 10,
    kAnnotSquiggly = 11,
    kAnnotStrikeOut = 12,
    kAnnotStamp = 13,
    kAnnotCaret = 14,
    kAnnotInk = 15,
    kAnnotPopup = 16,
    kAnnotFileAttachment = 17,
    kAnnotSound = 18,
    kAnnotMovie = 19,
    kAnnotWidget = 20,
    kAnnotScreen = 21,
    kAnnotPrinterMark = 22,
    kAnnotTrapNet = 23,
    kAnnotWatermark = 24,
    kAnnot3D = 25,
    kAnnotRedact = 26,
} PdfAnnotSubtype;

enum {
    kAnnotFlagNone = 0x0000,
    kAnnotFlagInvisible = 0x0001,
    kAnnotFlagHidden = 0x0002,
    kAnnotFlagPrint = 0x0004,
    kAnnotFlagNoZoom = 0x0008,
    kAnnotFlagNoRotate = 0x0010,
    kAnnotFlagNoView = 0x0020,
    kAnnotFlagReadOnly = 0x0040,
    kAnnotFlagLocked = 0x0080,
    kAnnotFlagToggleNoView = 0x0100,
    kAnnotFlagLockedContents = 0x0200,
} ;

enum {
    kRemoveAnnotSingle = 0x0000,
    kRemoveAnnotPopup = 0x0001,
    kRemoveAnnotReply = 0x0002,
} ;

typedef enum {
    kBorderSolid = 0,
    kBorderDashed = 1,
    kBorderBeveled = 2,
    kBorderInset = 3,
    kBorderUnderline = 4,
} PdfBorderStyle;

enum {
    kTextFlagNone = 0x000,
    kTextFlagUnderline = 0x001,
    kTextFlagStrikeout = 0x002,
    kTextFlagHighlight = 0x004,
    kTextFlagSubscript = 0x008,
    kTextFlagSuperscript = 0x010,
    kTextFlagNoUnicode = 0x020,
    kTextFlagPatternFill = 0x040,
    kTextFlagPatternStroke = 0x080,
    kTextFlagWhiteSpace = 0x100,
    kTextFlagUnicode = 0x200,
} ;

enum {
    kFieldFlagNone = 0x00000000,
    kFieldFlagReadOnly = 0x00000001,
    kFieldFlagRequired = 0x00000002,
    kFieldFlagNoExport = 0x00000004,
    kFieldFlagMultiline = 0x00001000,
    kFieldFlagPassword = 0x00002000,
    kFieldFlagNoToggleToOff = 0x00004000,
    kFieldFlagRadio = 0x00008000,
    kFieldFlagPushButton = 0x00010000,
    kFieldFlagCombo = 0x00200000,
    kFieldFlagEdit = 0x00400000,
    kFieldFlagSort = 0x00800000,
    kFieldFlagMultiSelect = 0x00200000,
    kFieldFlagDoNotSpellCheck = 0x00400000,
    kFieldFlagDCommitOnSelChange = 0x04000000,
    kFieldFlagFileSelect = 0x00100000,
    kFieldFlagDoNotScroll = 0x00800000,
    kFieldFlagComb = 0x01000000,
    kFieldFlagRichText = 0x02000000,
    kFieldFlagRadiosInUnison = 0x02000000,
} ;

typedef enum {
    kFieldUnknown = 0,
    kFieldButton = 1,
    kFieldRadio = 2,
    kFieldCheck = 3,
    kFieldText = 4,
    kFieldCombo = 5,
    kFieldList = 6,
    kFieldSignature = 7,
} PdfFieldType;

typedef enum {
    kActionEventAnnotEnter = 0,
    kActionEventAnnotExit = 1,
    kActionEventAnnotMouseDown = 2,
    kActionEventAnnotMouseUp = 3,
    kActionEventAnnotFocus = 4,
    kActionEventAnnotBlur = 5,
    kActionEventAnnotPageOpen = 6,
    kActionEventAnnotPageClose = 7,
    kActionEventAnnotPageVisible = 8,
    kActionEventAnnotPageInvisible = 9,
    kActionEventPageOpen = 10,
    kActionEventPageClose = 11,
    kActionEventFieldKeystroke = 12,
    kActionEventFieldFormat = 13,
    kActionEventFieldValidate = 14,
    kActionEventFieldCalculate = 15,
    kActionEventDocWillClose = 16,
    kActionEventDocWillSave = 17,
    kActionEventDocDidSave = 18,
    kActionEventDocWillPrint = 19,
    kActionEventDocDidPrint = 20,
    kActionEventDocOpen = 21,
} PdfActionEventType;

typedef enum {
    kActionUnknown = 0,
    kActionGoTo = 1,
    kActionGoToR = 2,
    kActionGoToE = 3,
    kActionLaunch = 4,
    kActionThread = 5,
    kActionURI = 6,
    kActionSound = 7,
    kActionMovie = 8,
    kActionHide = 9,
    kActionNamed = 10,
    kActionSubmitForm = 11,
    kActionResetForm = 12,
    kActionImportData = 13,
    kActionJavaScript = 14,
    kActionSetOCGState = 15,
    kActionRendition = 16,
    kActionTrans = 17,
    kActionGoTo3DView = 18,
} PdfActionType;

enum {
    kRenderAnnot = 0x001,
    kRenderLCDText = 0x002,
    kRenderNoNativeText = 0x004,
    kRenderGrayscale = 0x008,
    kRenderLimitedCache = 0x010,
    kRenderForceHalftone = 0x020,
    kRenderPrinting = 0x040,
    kRenderNoText = 0x080,
    kRenderNoBackground = 0x100,
    kRenderBorder = 0x200,
} ;

typedef enum {
    kImageFormatUnknown = 0,
    kImageFormatPng = 1,
    kImageFormatJpg = 2,
    kImageFormatBmp = 3,
    kImageFormatEmf = 4,
} PdfImageFormat;

enum {
    kFontFixedPitch = 0x00001,
    kFontSerif = 0x00002,
    kFontSymbolic = 0x00004,
    kFontScript = 0x00008,
    kFontNotSymbolic = 0x00020,
    kFontItalic = 0x00040,
    kFontAllCap = 0x10000,
    kFontSmallCap = 0x20000,
    kFontForceBold = 0x40000,
} ;

enum {
    kContentImage = 0x00001,
    kContentText = 0x00002,
    kContentPath = 0x00004,
    kContentForm = 0x00008,
    kContentShading = 0x00020,
    kContentTextTransparent = 0x00040,
    kContentTextFill = 0x00080,
    kContentTextStroke = 0x00100,
} ;

typedef enum {
    kFontAnsiCharset = 0,
    kFontDefaultCharset = 1,
    kFontSymbolCharset = 2,
    kFontUnknownCharset = 3,
    kFontMacintoshCharset = 77,
    kFontShiftJISCharset = 128,
    kFontHangeulCharset = 129,
    kFontKoreanCharset = 130,
    kFontGB2312Charset = 134,
    kFontCHineseBig5Charset = 136,
    kFontGreekCharset = 161,
    kFontTurkishCharset = 162,
    kFontVietnameseCharset = 163,
    kFontHebrewCharset = 177,
    kFontArabicCharset = 178,
    kFontArabicTCharset = 179,
    kFontArabicUCharset = 180,
    kFontHebrewUCharset = 181,
    kFontBalticCharset = 186,
    kFontRussianCharset = 204,
    kFontThaiCharset = 222,
    kFontEastEuropeCharset = 238,
} PdfFontCharset;

typedef enum {
    kFontDefANSICodepage = 0,
    kFontSymbolCodepage = 42,
    kFontMSDOSUSCodepage = 437,
    kFontArabicASMO708Codepage = 708,
    kFontMSDOSGreek1Codepage = 737,
    kFontMSDOSBalticCodepage = 775,
    kFontMSDOSWesternEuropeanCodepage = 850,
    kFontMSDOSEasternEuropeanCodepage = 852,
    kFontMSDOSCyrillicCodepage = 855,
    kFontMSDOSTurkishCodepage = 857,
    kFontMSDOSPortugueseCodepage = 860,
    kFontMSDOSIcelandicCodepage = 861,
    kFontMSDOSHebrewCodepage = 862,
    kFontMSDOSFrenchCanadianCodepage = 863,
    kFontMSDOSArabicCodepage = 864,
    kFontMSDOSNorwegianCodepage = 865,
    kFontMSDOSRussianCodepage = 866,
    kFontMSDOSGreek2Codepage = 869,
    kFontMSDOSThaiCodepage = 874,
    kFontShiftJISCodepage = 932,
    kFontChineseSimplifiedCodepage = 936,
    kFontHangulCodepage = 949,
    kFontChineseTraditionalCodepage = 950,
    kFontUTF16LECodepage = 1200,
    kFontUTF16BECodepage = 1201,
    kFontMSWinEasternEuropeanCodepage = 1250,
    kFontMSWinCyrillicCodepage = 1251,
    kFontMSWinWesternEuropeanCodepage = 1252,
    kFontMSWinGreekCodepage = 1253,
    kFontMSWinTurkishCodepage = 1254,
    kFontMSWinHebrewCodepage = 1255,
    kFontMSWinArabicCodepage = 1256,
    kFontMSWinBalticCodepage = 1257,
    kFontMSWinVietnameseCodepage = 1258,
    kFontJohabCodepage = 1361,
    kFontMACRomanCodepage = 10000,
    kFontMACShiftJISCodepage = 10001,
    kFontMACChineseTraditionalCodepage = 10002,
    kFontMACKoreanCodepage = 10003,
    kFontMACArabicCodepage = 10004,
    kFontMACHebrewCodepage = 10005,
    kFontMACGreekCodepage = 10006,
    kFontMACCyrillicCodepage = 10007,
    kFontMACChineseSimplifiedCodepage = 10008,
    kFontMACThaiCodepage = 10021,
    kFontMACEasternEuropeanCodepage = 10029,
    kFontMACTurkishCodepage = 10081,
    kFontUTF8Codepage = 65001,
} PdfFontCodepage;

enum {
    kFontCreateNormal = 0,
    kFontCreateEmbedded = 0x0001,
    kFontCreateSubset = 0x0002,
    kFontCreateDoNotEmbed = 0x0004,
    kFontCreateEncodeByGID = 0x0008,
    kFontCreateDeferWidths = 0x0010,
    kFontCreateGIDOverride = 0x0020,
    kFontCreateToUnicode = 0x0040,
    kFontCreateAllWidths = 0x0080,
    kFontCreateEmbedOpenType = 0x0100,
} ;

typedef enum {
    kFontUnknownType = 0,
    kFontType1 = 1,
    kFontTrueType = 2,
    kFontType3 = 3,
    kFontCIDFont = 4,
} PdfFontType;

typedef enum {
    kFontFormatTtf = 0,
    kFontFormatWoff = 1,
} PdfFontFormat;

typedef enum {
    kPdfZoomXYZ = 1,
    kPdfZoomFitPage = 2,
    kPdfZoomFitHorz = 3,
    kPdfZoomFitVert = 4,
    kPdfZoomFitRect = 5,
    kPdfZoomFitBbox = 6,
    kPdfZoomFitBHorz = 7,
    kPdfZoomFitBVert = 8,
} PdfDestZoomType;

typedef enum {
    kDigSigOpenSSL = 0,
    kDigSigCert = 1,
    kDigSigCustom = 2,
} PdfDigSigType;

typedef enum {
    kImageFigure = 0,
    kImageImage = 1,
    kImagePath = 2,
    kImageRect = 3,
    kImageShading = 4,
    kImageForm = 5,
} PdfImageType;

enum {
    kTableUnknown = 0x00,
    kTableGraphic = 0x01,
    kTableIsolated = 0x02,
    kTableIsolatedCol = 0x04,
    kTableIsolatedRow = 0x08,
    kTableForm = 0x10,
    kTableElement = 0x20,
} ;

typedef enum {
    kListNone = 0,
    kListUnordered = 1,
    kListOrdered = 2,
    kListDecimal = 3,
    kListRomanUpper = 4,
    kListRomanLower = 5,
    kListLetterUpper = 6,
    kListLetterLower = 7,
} PdfListType;

enum {
    kWordFlagHyphen = 0x0001,
    kWordFlagBullet = 0x0002,
    kWordFlagColon = 0x0004,
    kWordFlagNumber = 0x008,
    kWordFlagSubscript = 0x0010,
    kWordFlagSupercript = 0x0020,
    kWordFlagTerminal = 0x0040,
    kWordFlagFirstCap = 0x0080,
    kWordFlagImage = 0x00100,
    kWordFlagNumberingDecimal = 0x0200,
    kWordFlagNumberingRoman = 0x0400,
    kWordFlagNumberingLetter = 0x0800,
    kWordFlagPageNumber = 0x1000,
    kWordFlagFilling = 0x2000,
    kWordFlagAllCaps = 0x4000,
    kWordFlagComma = 0x8000,
    kWordFlagNoUnicode = 0x10000,
    kWordFlagLetter = 0x20000,
} ;

enum {
    kTextLineFlagHyphen = 0x0001,
    kTextLineFlagNewLine = 0x0002,
    kTextLineFlagIndent = 0x0004,
    kTextLineFlagTerminal = 0x0008,
    kTextLineFlagDropCap = 0x0010,
    kTextLineFlagFilling = 0x0020,
    kTextLineFlagAllCaps = 0x0040,
} ;

typedef enum {
    kTextNormal = 0,
    kTextH1 = 1,
    kTextH2 = 2,
    kTextH3 = 3,
    kTextH4 = 4,
    kTextH5 = 5,
    kTextH6 = 6,
    kTextH7 = 7,
    kTextH8 = 8,
    kTextNote = 9,
    kTextTitle = 10,
} PdfTextStyle;

enum {
    kTextFlagTableCaption = 0x0001,
    kTextFlagImageCaption = 0x0002,
    kTextFlagChartCaption = 0x0004,
    kTextFlagNoteCaption = 0x0008,
    kTextFlagFilling = 0x010,
    kTextFlagAllCaps = 0x020,
} ;

enum {
    kElemNoJoin = 0x001,
    kElemNoSplit = 0x002,
    kElemArtifact = 0x004,
    kElemHeader = 0x008,
    kElemFooter = 0x010,
    kElemSplitter = 0x020,
    kElemNoTable = 0x040,
    kElemTable = 0x080,
} ;

typedef enum {
    kPsWrite = 0,
    kPsReadOnly = 1,
    kPsTruncate = 2,
} PsFileMode;

typedef enum {
    kAlternatePdf = 0,
    kAlternateHtml = 1,
} PdfAlternateType;

typedef enum {
    kCSSMediaTypeAll = 0,
    kCSSMediaTypePrint = 1,
    kCSSMediaTypeScreen = 2,
    kCSSMediaTypeSpeech = 3,
} PdfMediaType;

typedef enum {
    kImageDIBFormatArgb = 0x220,
} PsImageDIBFormat;

typedef enum {
    kDataFormatJson = 0,
    kDataFormatXml = 1,
    kDataFormatTxt = 2,
} PsDataFormat;

typedef enum {
    kRenderDeviceTypeGDI = 0,
    kRenderDeviceTypeDirectX = 1,
} PsRenderDeviceType;

typedef enum {
    kFileStream = 0,
    kMemoryStream = 1,
    kProcStream = 2,
} PdfStreamType;

typedef enum {
    kPdsStructChildInvalid = 0,
    kPdsStructChildElement = 1,
    kPdsStructChildPageContent = 2,
    kPdsStructChildStreamContent = 3,
    kPdsStructChildObject = 4,
} PdfStructElementType;

enum {
    kPageInsertNone = 0x0000,
    kPageInsertBookmarks = 0x001,
    kPageInsertAll = 0x0002,
} ;

typedef enum {
    kAuthorizationStandard = 0,
    kAuthorizationAccount = 1,
} PdfAuthorizationType;

typedef enum {
    kDestFitUnknown = 0,
    kDestFitXYZ = 1,
    kDestFit = 2,
    kDestFitH = 3,
    kDestFitV = 4,
    kDestFitR = 5,
    kDestFitB = 6,
    kDestFitBH = 7,
    kDestFitBV = 8,
} PdfDestFitType;

typedef enum {
    kLabelNo = -1,
    kLabelNone = 0,
    kLabel = 1,
    kLabelLevel1 = 2,
    kLabelLevel2 = 3,
    kLabelLevel3 = 4,
    kLabelLevel4 = 5,
} PdfLabelType;

typedef enum {
    kAVCommandReady = 0,
    kAVCommandWorking = 1,
    kAVCommandDone = 2,
    kAVCommandCanceled = 3,
    kAVCommandInError = 4,
} PsCommandStatus;

typedef enum {
    kAppearanceNormal = 0,
    kAppearanceRollover = 1,
    kAppearanceDown = 2,
} PdfAnnotAppearanceMode;

typedef enum {
    kPathLineTo = 0,
    kPathBezierTo = 1,
    kPathMoveTo = 2,
} PdsPathPointType;

typedef enum {
    kBlendModeNormal = 0,
    kBlendModeMultiply = 1,
    kBlendModeScreen = 2,
    kBlendModeOverlay = 3,
    kBlendModeDarken = 4,
    kBlendModeLighten = 5,
    kBlendModeColorDodge = 6,
    kBlendModeColorBurn = 7,
    kBlendModeHardLight = 8,
    kBlendModeSoftLight = 9,
    kBlendModeDifference = 10,
    kBlendModeExclusion = 11,
    kBlendModeHue = 21,
    kBlendModeSaturation = 22,
    kBlendModeColor = 23,
    kBlendModeLuminosity = 24,
} PdfBlendMode;

typedef enum {
    kFillRuleNone = 0,
    kFillRuleEvenOdd = 1,
    kFillRuleWinding = 2,
} PdfFillRule;

enum {
    kContentToPage = 0x01,
    kContentToForm = 0x02,
    kContentToCharproc = 0x04,
} ;

typedef enum {
    kColorSpaceUnknown = 0,
    kColorSpaceDeviceGray = 1,
    kColorSpaceDeviceRGB = 2,
    kColorSpaceDeviceCMYK = 3,
    kColorSpaceCalGray = 4,
    kColorSpaceCalRGB = 5,
    kColorSpaceLab = 6,
    kColorSpaceICCBase = 7,
    kColorSpaceSeparation = 8,
    kColorSpaceDeviceN = 9,
    kColorSpaceIndexed = 10,
    kColorSpacePattern = 11,
} PdfColorSpaceFamily;

enum {
    kStateDefault = 0,
    kStateNoRender = 0x01,
    kStateExclude = 0x02,
} ;

enum {
    kInsertBeforeFirst = 0,
    kInsertAfterLast = -1,
} ;

typedef enum {
    kWordFinderAlgLatest = -1,
    kWordFinderAlgBasic = 0,
} PdfWordFinderAlgorithm;

enum {
    kUserPermissionAll = -1,
    kUserPermissionNone = -65536,
    kUserPermissionOpen = 0x0001,
    kUserPermissionOwner = 0x0002,
    kUserPermissionPrint = 0x0004,
    kUserPermissionModify = 0x0008,
    kUserPermissionExtract = 0x0010,
    kUserPermissionModifyAnnots = 0x0020,
    kUserPermissionFillForms = 0x0100,
    kUserPermissionExtractAccessibility = 0x0200,
    kUserPermissionAssembleDoc = 0x0400,
    kUserPermissionPrintHighRes = 0x0800,
} ;

typedef enum {
    kStandardEncryptionMethodNone = 0,
    kStandardEncryptionMethodRC4v2 = 1,
    kStandardEncryptionMethodAESv1 = 2,
    kStandardEncryptionMethodAESv2 = 3,
} PdfStandardEncryptionMethod;


typedef struct _PdfPoint {
    float x;
    float y;
    _PdfPoint() {
        x = 0;
        y = 0;
    }
} PdfPoint;

typedef struct _PdfDevPoint {
    int x;
    int y;
    _PdfDevPoint() {
        x = 0;
        y = 0;
    }
} PdfDevPoint;

typedef struct _PdfRect {
    float left;
    float top;
    float right;
    float bottom;
    _PdfRect() {
        left = 0;
        top = 0;
        right = 0;
        bottom = 0;
    }
} PdfRect;

typedef struct _PdfDevRect {
    int left;
    int top;
    int right;
    int bottom;
    _PdfDevRect() {
        left = 0;
        top = 0;
        right = 0;
        bottom = 0;
    }
} PdfDevRect;

typedef struct _PdfQuad {
    PdfPoint tl;
    PdfPoint tr;
    PdfPoint bl;
    PdfPoint br;
} PdfQuad;

typedef struct _PdfDevQuad {
    PdfDevPoint tl;
    PdfDevPoint tr;
    PdfDevPoint bl;
    PdfDevPoint br;
} PdfDevQuad;

typedef struct _PdfMatrix {
    float a;
    float b;
    float c;
    float d;
    float e;
    float f;
    _PdfMatrix() {
        a = 1;
        b = 0;
        c = 0;
        d = 1;
        e = 0;
        f = 0;
    }
} PdfMatrix;

typedef struct _PdfGray {
    int gray;
    _PdfGray() {
        gray = 0;
    }
} PdfGray;

typedef struct _PdfRGB {
    int r;
    int g;
    int b;
    _PdfRGB() {
        r = 0;
        g = 0;
        b = 0;
    }
} PdfRGB;

typedef struct _PdfCMYK {
    int c;
    int m;
    int y;
    int k;
    _PdfCMYK() {
        c = 0;
        m = 0;
        y = 0;
        k = 0;
    }
} PdfCMYK;

typedef struct _PdfColorState {
    PdfFillType fill_type;
    PdfFillType stroke_type;
    PdfColor* fill_color;
    PdfColor* stroke_color;
    int fill_opacity;
    int stroke_opacity;
    _PdfColorState() {
        fill_type = kFillTypeNone;
        stroke_type = kFillTypeNone;
        fill_color = nullptr;
        stroke_color = nullptr;
        fill_opacity = 255;
        stroke_opacity = 255;
    }
} PdfColorState;

typedef struct _PdfTextState {
    PdfColorState color_state;
    PdfFont* font;
    float font_size;
    float char_spacing;
    float word_spacing;
    PdfTextStateFlag flags;
    _PdfTextState() {
        font = nullptr;
        font_size = 0;
        char_spacing = 0;
        word_spacing = 0;
        flags = 0;
    }
} PdfTextState;

typedef struct _PdfGraphicState {
    PdfColorState color_state;
    float line_width;
    float miter_limit;
    PdfLineCap line_cap;
    PdfLineJoin line_join;
    PdfBlendMode blend_mode;
    PdfMatrix matrix;
    _PdfGraphicState() {
        line_width = 1;
        miter_limit = 0;
        line_cap = kPdfLineCapButt;
        line_join = kPdfLineJoinMiter;
        blend_mode = kBlendModeNormal;
    }
} PdfGraphicState;

typedef struct _PdfFontState {
    PdfFontType type;
    PdfFontFlags flags;
    PdfRect bbox;
    int ascent;
    int descent;
    int italic;
    int bold;
    int fixed_width;
    int vertical;
    int embedded;
    int height;
    _PdfFontState() {
        type = kFontUnknownType;
        flags = 0;
        ascent = 0;
        descent = 0;
        italic = 0;
        bold = 0;
        fixed_width = 0;
        vertical = 0;
        embedded = 0;
        height = 0;
    }
} PdfFontState;

typedef struct _PdfPageRenderParams {
    PsRenderDeviceContext* device;
    PsImage* image;
    PdfMatrix matrix;
    PdfRect clip_box;
    PdfRenderFlags render_flags;
    _PdfPageRenderParams() {
        device = nullptr;
        image = nullptr;
        clip_box.left = 0;
        clip_box.right = 0;
        clip_box.top = 0;
        clip_box.bottom = 0;
        render_flags = kRenderAnnot;
    }
} PdfPageRenderParams;

typedef struct _PdfAnnotAppearance {
    PdfRGB fill_color;
    PdfFillType fill_type;
    PdfRGB border_color;
    float border_width;
    PdfBorderStyle border;
    float opacity;
    float font_size;
    PdfTextAlignment text_align;
    _PdfAnnotAppearance() {
        fill_type = kFillTypeNone;
        border_width = 1;
        border = kBorderSolid;
        opacity = 1;
        font_size = 0;
        text_align = kTextAlignmentLeft;
    }
} PdfAnnotAppearance;

typedef struct _PdfBookmarkAppearance {
    PdfRGB color;
    int italic;
    int bold;
    _PdfBookmarkAppearance() {
        italic = 0;
        bold = 0;
    }
} PdfBookmarkAppearance;

typedef struct _PdfWhitespaceParams {
    float width;
    float height;
    _PdfWhitespaceParams() {
        width = 0;
        height = 0;
    }
} PdfWhitespaceParams;

typedef struct _PdfMediaQueryParams {
    PdfMediaType type;
    int min_width;
    _PdfMediaQueryParams() {
        type = kCSSMediaTypeAll;
        min_width = 1200;
    }
} PdfMediaQueryParams;

typedef struct _PdfImageParams {
    PdfImageFormat format;
    int quality;
    _PdfImageParams() {
        format = kImageFormatPng;
        quality = 100;
    }
} PdfImageParams;

typedef struct _PdfAccessibleParams {
    int accept_tags;
    int embed_fonts;
    int subset_fonts;
    int create_bookmarks;
    _PdfAccessibleParams() {
        accept_tags = 0;
        embed_fonts = 0;
        subset_fonts = 0;
        create_bookmarks = 0;
    }
} PdfAccessibleParams;

typedef struct _PdsContentParams {
    PdsContentFlags flags;
    int form_type;
    PdfRect bbox;
    PdfMatrix matrix;
    _PdsContentParams() {
        flags = 0;
        form_type = 0;
    }
} PdsContentParams;

typedef struct _PdfStandardSecurityParams {
    PdfUserPermissionFlags permissions;
    int revision;
    PdfStandardEncryptionMethod encrypt_method;
    int encrypt_metadata;
    _PdfStandardSecurityParams() {
        permissions = kUserPermissionAll;
        revision = 4;
        encrypt_method = kStandardEncryptionMethodAESv1;
        encrypt_metadata = 0;
    }
} PdfStandardSecurityParams;

typedef int (*PdfCancelProc) (void* client_data);
typedef void (*PdfEventProc) (void* client_data);
typedef void (*PdsObjectEventProc) (PdsObject* obj, PdfEventType event, const wchar_t* key, int index, void* client_data);
typedef void (*PdsContentEventProc) (PdsContent* content, PdfEventType event, void* client_data);
typedef void (*PdsPageObjectEventProc) (PdsPageObject* content, PdfEventType event, void* client_data);
typedef unsigned long (*PdfDigestDataProc) (int buffer_count, const unsigned char* buffer_to_sign[], unsigned long buffer_size[], unsigned char* sign_buff, unsigned long sign_buff_size, void* client_data);
typedef int (*PsStreamReadProc)(int offset, void* buffer, int size, void* client_data);
typedef int (*PsStreamWriteProc)(int offset, void* buffer, int size, void* client_data);
typedef void (*PsStreamDestroyProc)(void* client_data);
typedef int (*PsStreamGetSizeProc)(void* client_data);
typedef int (*PsStreamGetPosProc)(void* client_data);
typedef void (*PdfSecurityDestroyProc)(void* client_data);
typedef bool (*PdfSecuritySetAuthorizationDataProc)(void* data, void* client_data);
typedef bool (*PdfSecurityOnInitProc)(const PdsDictionary* trailer, void* client_data);
typedef int (*PdfSecurityGetPermissionsProc)(void* client_data);
typedef bool (*PdfSecurityIsMetadataEncryptedProc)(void* client_data);
typedef void (*PdfSecurityUpdateEncryptDictProc)(PdsDictionary* encrypt_dict, const PdsArray* id_array, void* client_data);
typedef int (*PdfSecurityGetDecryptSizeProc)(const void* data, int size, void* client_data);
typedef int (*PdfSecurityDecryptContentProc)(int objnum, int gennum, const void* data, int data_size, void* dest, int dest_size, void* client_data);
typedef int (*PdfSecurityGetEncryptSizeProc)(const void* data, int size, void* client_data);
typedef int (*PdfSecurityEncryptContentProc)(int objnum, int gennum, const void* data, int data_size, void* dest, int dest_size, void* client_data);
typedef bool (*PdfGetAuthDataProc)(PdfDoc* doc, PdfSecurityHandler* handler, void* data);
typedef PdfSecurityHandler* (*PdfSecurityCreateHandlerProc)(void* client_data);
typedef bool (*PdfAnnotHandlerCanCopyProc)(PdfAnnot* annot);
typedef void* (*PdfAnnotHandlerCopyProc)(PdfAnnot* annot);
typedef bool (*PdfAnnotHandlerCanPasteProc)(PdfPage* dest_page, const PdfPoint* center, void* data);
typedef PdfAnnot* (*PdfAnnotHandlerPasteProc)(PdfPage* dest_page, const PdfPoint* center, void* data);
typedef bool (*PdfAnnotHandlerDestroyDataProc)(void* data);
typedef void (*PdfAnnotHandlerDestroyProc)();
typedef bool (*PdfActionHandlerCanCopyProc)(PdfAction* action);
typedef void* (*PdfActionHandlerCopyProc)(PdfAction* action);
typedef bool (*PdfActionHandlerCanPasteProc)(PdfDoc* dest_doc, void* data);
typedef PdfAction* (*PdfActionHandlerPasteProc)(PdfDoc* dest_doc, void* data);
typedef bool (*PdfActionHandlerDestroyDataProc)(void* data);
typedef void (*PdfActionHandlerDestroyProc)();

struct PdsObject {
    virtual PdfObjectType GetObjectType() const = 0;
    virtual int GetId() const = 0;
    virtual int GetGenId() const = 0;
    virtual PdfDoc* GetDoc() const = 0;
    virtual bool RegisterEvent(PdfEventType type, _callback_ PdsObjectEventProc proc, void* data) = 0;
    virtual bool UnregisterEvent(PdfEventType type, _callback_ PdsObjectEventProc proc, void* data) = 0;
};

struct PdsBoolean : PdsObject {
    virtual bool GetValue() const = 0;
};

struct PdsNumber : PdsObject {
    virtual bool IsIntegerValue() const = 0;
    virtual int GetIntegerValue() const = 0;
    virtual float GetValue() const = 0;
};

struct PdsString : PdsObject {
    virtual int GetValue(_out_ char* buffer, int len) const = 0;
    virtual int GetText(_out_ wchar_t* buffer, int len) const = 0;
    std::wstring GetText() {
        std::wstring buffer;
        buffer.resize(GetText(nullptr, 0));
        GetText((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
};

struct PdsName : PdsObject {
    virtual int GetValue(_out_ char* buffer, int len) const = 0;
    virtual int GetText(_out_ wchar_t* buffer, int len) const = 0;
    std::wstring GetText() {
        std::wstring buffer;
        buffer.resize(GetText(nullptr, 0));
        GetText((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
};

struct PdsArray : PdsObject {
    virtual int GetNumObjects() const = 0;
    virtual PdsObject* Get(int index) const = 0;
    virtual bool Put(int index, PdsObject* value) = 0;
    virtual bool PutNumber(int index, float value) = 0;
    virtual bool PutName(int index, const wchar_t* value) = 0;
    virtual bool PutString(int index, const wchar_t* value) = 0;
    virtual bool Insert(int index, PdsObject* value) = 0;
    virtual PdsDictionary* InsertDict(int index) = 0;
    virtual PdsArray* InsertArray(int index) = 0;
    virtual bool RemoveNth(int index) = 0;
    virtual PdsDictionary* GetDictionary(int index) const = 0;
    virtual PdsArray* GetArray(int index) const = 0;
    virtual PdsStream* GetStream(int index) const = 0;
    virtual int GetString(int index, _out_ char* buffer, int len) const = 0;
    virtual int GetText(int index, _out_ wchar_t* buffer, int len) const = 0;
    virtual float GetNumber(int index) const = 0;
    virtual int GetInteger(int index) const = 0;
    std::wstring GetText(int index) {
        std::wstring buffer;
        buffer.resize(GetText(index, nullptr, 0));
        GetText(index, (wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
};

struct PdsDictionary : PdsObject {
    virtual bool Known(const wchar_t* key) const = 0;
    virtual int GetNumKeys() const = 0;
    virtual int GetKey(int index, _out_ wchar_t* buffer, int len) const = 0;
    virtual PdsObject* Get(const wchar_t* key) = 0;
    virtual bool Put(const wchar_t* key, PdsObject* value) = 0;
    virtual bool PutBool(const wchar_t* key, bool value) = 0;
    virtual bool PutName(const wchar_t* key, const wchar_t* value) = 0;
    virtual bool PutString(const wchar_t* key, const wchar_t* value) = 0;
    virtual bool PutNumber(const wchar_t* key, float value) = 0;
    virtual bool PutRect(const wchar_t* key, PdfRect* rect) = 0;
    virtual bool PutMatrix(const wchar_t* key, PdfMatrix* matrix) = 0;
    virtual PdsDictionary* PutDict(const wchar_t* key) = 0;
    virtual PdsArray* PutArray(const wchar_t* key) = 0;
    virtual PdsDictionary* GetDictionary(const wchar_t* key) = 0;
    virtual PdsArray* GetArray(const wchar_t* key) = 0;
    virtual PdsStream* GetStream(const wchar_t* key) = 0;
    virtual int GetString(const wchar_t* key, _out_ char* buffer, int len) const = 0;
    virtual int GetText(const wchar_t* key, _out_ wchar_t* buffer, int len) const = 0;
    virtual float GetNumber(const wchar_t* key) const = 0;
    virtual int GetInteger(const wchar_t* key, int default_value) const = 0;
    virtual bool GetBoolean(const wchar_t* key, bool default_value) const = 0;
    virtual bool RemoveKey(const wchar_t* key) = 0;
    std::wstring GetKey(int index) {
        std::wstring buffer;
        buffer.resize(GetKey(index, nullptr, 0));
        GetKey(index, (wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetText(const wchar_t* key) {
        std::wstring buffer;
        buffer.resize(GetText(key, nullptr, 0));
        GetText(key, (wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
};

struct PdsStream : PdsObject {
    virtual PdsDictionary* GetStreamDict() = 0;
    virtual int GetRawDataSize() const = 0;
    virtual bool IsEof() const = 0;
    virtual int GetSize() const = 0;
    virtual bool Read(int offset, _out_ uint8_t* buffer, int size) const = 0;
    virtual int GetPos() const = 0;
};

struct PdsNull : PdsObject {
};

struct PdsContent {
    virtual PdsText* AddNewText(int index, PdfFont* font, const PdfMatrix* matrix) = 0;
    virtual PdsPath* AddNewPath(int index, const PdfMatrix* matrix) = 0;
    virtual PdsImage* AddNewImage(int index, PdsStream* image_xobj, const PdfMatrix* matrix) = 0;
    virtual bool RemoveObject(PdsPageObject* object) = 0;
    virtual int GetNumObjects() const = 0;
    virtual PdsPageObject* GetObject(int index) = 0;
    virtual PdsStream* ToObject(PdfDoc* doc, const PdsContentParams* content_params) = 0;
    virtual bool RegisterEvent(PdfEventType type, _callback_ PdsContentEventProc proc, void* data) = 0;
    virtual bool UnregisterEvent(PdfEventType type, _callback_ PdsContentEventProc proc, void* data) = 0;
};

struct PdsPageObject {
    virtual PdfPageObjectType GetObjectType() const = 0;
    virtual void GetBBox(_out_ PdfRect* bbox) const = 0;
    virtual void GetQuad(_out_ PdfQuad* quad) const = 0;
    virtual int GetId() const = 0;
    virtual int GetStateFlags() const = 0;
    virtual bool SetStateFlags(int flags) = 0;
    virtual PdsObject* GetStructObject(bool struct_parent) = 0;
    virtual PdsContentMark* GetContentMark() = 0;
    virtual PdfPage* GetPage() = 0;
    virtual PdsContent* GetParentContent() = 0;
    virtual bool GetGState(_out_ PdfGraphicState* g_state) = 0;
    virtual bool SetGState(const PdfGraphicState* g_state) = 0;
    virtual bool SetMatrix(const PdfMatrix* matrix) = 0;
    virtual void MoveToContent(PdsContent* content, int index) = 0;
    virtual PdsPageObject* CopyToContent(PdsContent* content, int index) = 0;
    virtual PdfDoc* GetDoc() = 0;
    virtual bool RegisterEvent(PdfEventType type, _callback_ PdsPageObjectEventProc proc, void* data) = 0;
    virtual bool UnregisterEvent(PdfEventType type, _callback_ PdsPageObjectEventProc proc, void* data) = 0;
    PdfRect GetBBox() {
        PdfRect bbox;
        GetBBox(&bbox);
        return bbox;
    }
    PdfQuad GetQuad() {
        PdfQuad quad;
        GetQuad(&quad);
        return quad;
    }
    PdfGraphicState GetGState() {
        PdfGraphicState g_state;
        GetGState(&g_state);
        return g_state;
    }
};

struct PdsText : PdsPageObject {
    virtual int GetText(_out_ wchar_t* buffer, int len) const = 0;
    virtual void SetText(const wchar_t* buffer) = 0;
    virtual bool GetTextState(_out_ PdfTextState* text_state) = 0;
    virtual void SetTextState(const PdfTextState* text_state) = 0;
    virtual bool GetTextMatrix(_out_ PdfMatrix* matrix) const = 0;
    virtual int GetNumChars() const = 0;
    virtual int GetCharText(int index, _out_ wchar_t* buffer, int len) const = 0;
    virtual bool GetCharBBox(int index, _out_ PdfRect* bbox) const = 0;
    virtual bool GetCharQuad(int index, _out_ PdfQuad* quad) const = 0;
    virtual float GetCharAdvanceWidth(int index) const = 0;
    virtual PdsText* SplitAtChar(int index) = 0;
    virtual PdfStateFlags GetCharStateFlags(int index) = 0;
    virtual bool SetCharStateFlags(int index, PdfStateFlags flags) = 0;
    std::wstring GetText() {
        std::wstring buffer;
        buffer.resize(GetText(nullptr, 0));
        GetText((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    PdfTextState GetTextState() {
        PdfTextState text_state;
        GetTextState(&text_state);
        return text_state;
    }
    PdfMatrix GetTextMatrix() {
        PdfMatrix matrix;
        GetTextMatrix(&matrix);
        return matrix;
    }
    std::wstring GetCharText(int index) {
        std::wstring buffer;
        buffer.resize(GetCharText(index, nullptr, 0));
        GetCharText(index, (wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    PdfRect GetCharBBox(int index) {
        PdfRect bbox;
        GetCharBBox(index, &bbox);
        return bbox;
    }
    PdfQuad GetCharQuad(int index) {
        PdfQuad quad;
        GetCharQuad(index, &quad);
        return quad;
    }
};

struct PdsForm : PdsPageObject {
    virtual PdsContent* GetContent() = 0;
};

struct PdsPath : PdsPageObject {
    virtual int GetNumPathPoints() const = 0;
    virtual PdsPathPoint* GetPathPoint(int index) = 0;
    virtual bool SetStroke(bool stroke) = 0;
    virtual bool SetFillType(PdfFillRule fill) = 0;
    virtual bool MoveTo(const PdfPoint* point) = 0;
    virtual bool LineTo(const PdfPoint* point) = 0;
    virtual bool CurveTo(const PdfPoint* control_p_1, const PdfPoint* control_p_2, const PdfPoint* point) = 0;
    virtual bool ArcTo(const PdfPoint* end_p, const PdfPoint* radius_p, float angle, bool is_large, bool sweep) = 0;
    virtual bool ClosePath() = 0;
};

struct PdsPathPoint {
    virtual PdsPathPointType GetType() const = 0;
    virtual bool GetPoint(_out_ PdfPoint* point) = 0;
    virtual bool IsClosed() const = 0;
    PdfPoint GetPoint() {
        PdfPoint point;
        GetPoint(&point);
        return point;
    }
};

struct PdsSoftMask {
    virtual PdsStream* GetDataStm() = 0;
};

struct PdsImage : PdsPageObject {
    virtual PdsStream* GetDataStm() = 0;
    virtual PdsSoftMask* GetSMask() = 0;
    virtual bool HasSMask() const = 0;
};

struct PdsShading : PdsPageObject {
};

struct PdsContentMark {
    virtual int GetNumTags() const = 0;
    virtual int GetTagName(int index, _out_ wchar_t* buffer, int len) const = 0;
    virtual PdsDictionary* GetTagObject(int index) = 0;
    virtual int GetTagMcid() const = 0;
    virtual PdsDictionary* GetTagArtifact() const = 0;
    virtual bool AddTag(const char* name, PdsDictionary* object, bool indirect) = 0;
    virtual bool InsertTag(int index, const char* name, PdsDictionary* object, bool indirect) = 0;
    virtual bool RemoveTag(int index) = 0;
    virtual int GetNumEqualTags(const PdsContentMark* mark) const = 0;
    std::wstring GetTagName(int index) {
        std::wstring buffer;
        buffer.resize(GetTagName(index, nullptr, 0));
        GetTagName(index, (wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
};

struct PdsTextRun {
    virtual PdsText* GetTextObject() = 0;
    virtual int GetFirstCharIndex() const = 0;
    virtual int GetLastCharIndex() const = 0;
};

struct PdsElement {
    virtual void GetBBox(_out_ PdfRect* bbox) const = 0;
    virtual void GetQuad(_out_ PdfQuad* quad) const = 0;
    PdfRect GetBBox() {
        PdfRect bbox;
        GetBBox(&bbox);
        return bbox;
    }
    PdfQuad GetQuad() {
        PdfQuad quad;
        GetQuad(&quad);
        return quad;
    }
};

struct PdsWord : PdsElement {
    virtual int GetText(_out_ wchar_t* buffer, int len) const = 0;
    virtual int GetNumChars() const = 0;
    virtual int GetCharCode(int index) const = 0;
    virtual int GetCharText(int index, _out_ wchar_t* buffer, int len) const = 0;
    virtual void GetCharTextState(int index, _out_ PdfTextState* text_state) const = 0;
    virtual void GetCharBBox(int index, _out_ PdfRect* bbox) const = 0;
    virtual int GetNumTextRuns() const = 0;
    virtual PdsTextRun* GetTextRunAt(int index) = 0;
    std::wstring GetText() {
        std::wstring buffer;
        buffer.resize(GetText(nullptr, 0));
        GetText((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetCharText(int index) {
        std::wstring buffer;
        buffer.resize(GetCharText(index, nullptr, 0));
        GetCharText(index, (wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    PdfTextState GetCharTextState(int index) {
        PdfTextState text_state;
        GetCharTextState(index, &text_state);
        return text_state;
    }
    PdfRect GetCharBBox(int index) {
        PdfRect bbox;
        GetCharBBox(index, &bbox);
        return bbox;
    }
};

struct PdsWordList {
    virtual int GetNumWords() const = 0;
    virtual PdsWord* GetWord(int index) = 0;
    virtual int Release() = 0;
};

struct PdeElement {
    virtual PdfElementType GetType() const = 0;
    virtual void GetBBox(_out_ PdfRect* bbox) const = 0;
    virtual bool SetBBox(const PdfRect* bbox) = 0;
    virtual void GetQuad(_out_ PdfQuad* quad) const = 0;
    virtual int GetId() = 0;
    virtual void GetGraphicState(_out_ PdfGraphicState* g_state) = 0;
    virtual int GetNumChildren() const = 0;
    virtual PdeElement* GetChild(int index) = 0;
    virtual PdfAlignment GetAlignment() const = 0;
    virtual float GetAngle() const = 0;
    virtual void SetData(void* data) = 0;
    virtual void* GetData() const = 0;
    virtual bool SetAlt(const wchar_t* alt) = 0;
    virtual bool SetActualText(const wchar_t* text) = 0;
    virtual int GetFlags() const = 0;
    virtual bool SetFlags(int flags) = 0;
    virtual int GetStateFlags() const = 0;
    virtual bool SetStateFlags(int flags, bool objects) = 0;
    virtual int GetNumPageObjects() const = 0;
    virtual PdsPageObject* GetPageObject(int index) = 0;
    virtual PdePageMap* GetPageMap() = 0;
    PdfRect GetBBox() {
        PdfRect bbox;
        GetBBox(&bbox);
        return bbox;
    }
    PdfQuad GetQuad() {
        PdfQuad quad;
        GetQuad(&quad);
        return quad;
    }
    PdfGraphicState GetGraphicState() {
        PdfGraphicState g_state;
        GetGraphicState(&g_state);
        return g_state;
    }
};

struct PdeContainer : PdeElement {
};

struct PdeList : PdeElement {
};

struct PdeToc : PdeContainer {
};

struct PdeAnnot : PdeElement {
    virtual PdfAnnot* GetAnnot() = 0;
};

struct PdeFormField : PdeAnnot {
};

struct PdeImage : PdeContainer {
    virtual PdfImageType GetImageType() const = 0;
    virtual PdeElement* GetCaption() = 0;
};

struct PdeLine : PdeElement {
};

struct PdeRect : PdeContainer {
};

struct PdeHeader : PdeContainer {
};

struct PdeFooter : PdeContainer {
};

struct PdeCell : PdeContainer {
    virtual int GetRowSpan() const = 0;
    virtual int GetColSpan() const = 0;
    virtual bool HasBorderGraphicState(int index) const = 0;
    virtual PdeCell* GetSpanCell() = 0;
};

struct PdeTable : PdeContainer {
    virtual int GetNumRows() const = 0;
    virtual int GetNumCols() const = 0;
    virtual PdeCell* GetCell(int row, int col) = 0;
    virtual PdfAlignment GetRowAlignment(int row) const = 0;
    virtual PdfAlignment GetColAlignment(int col) const = 0;
    virtual PdeElement* GetCaption() = 0;
    virtual PdfTableType GetTableType() const = 0;
};

struct PdeTextRun : PdeElement {
    virtual PdsText* GetTextObject() = 0;
    virtual int GetFirstCharIndex() const = 0;
    virtual int GetLastCharIndex() const = 0;
};

struct PdeWord : PdeElement {
    virtual int GetText(_out_ wchar_t* buffer, int len) const = 0;
    virtual bool HasTextState() const = 0;
    virtual void GetTextState(_out_ PdfTextState* text_state) = 0;
    virtual int GetNumChars() const = 0;
    virtual int GetCharCode(int index) const = 0;
    virtual int GetCharText(int index, _out_ wchar_t* buffer, int len) const = 0;
    virtual void GetCharTextState(int index, _out_ PdfTextState* text_state) const = 0;
    virtual void GetCharBBox(int index, _out_ PdfRect* bbox) const = 0;
    virtual int GetWordFlags() const = 0;
    virtual PdeElement* GetBackground() = 0;
    virtual void GetOrigin(_out_ PdfPoint* point) const = 0;
    virtual int GetNumTextRuns() const = 0;
    virtual PdeTextRun* GetTextRun(int index) = 0;
    virtual int GetCharStateFlags(int index) = 0;
    std::wstring GetText() {
        std::wstring buffer;
        buffer.resize(GetText(nullptr, 0));
        GetText((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    PdfTextState GetTextState() {
        PdfTextState text_state;
        GetTextState(&text_state);
        return text_state;
    }
    std::wstring GetCharText(int index) {
        std::wstring buffer;
        buffer.resize(GetCharText(index, nullptr, 0));
        GetCharText(index, (wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    PdfTextState GetCharTextState(int index) {
        PdfTextState text_state;
        GetCharTextState(index, &text_state);
        return text_state;
    }
    PdfRect GetCharBBox(int index) {
        PdfRect bbox;
        GetCharBBox(index, &bbox);
        return bbox;
    }
    PdfPoint GetOrigin() {
        PdfPoint point;
        GetOrigin(&point);
        return point;
    }
};

struct PdeTextLine : PdeElement {
    virtual int GetText(_out_ wchar_t* buffer, int len) const = 0;
    virtual bool HasTextState() const = 0;
    virtual void GetTextState(_out_ PdfTextState* text_state) = 0;
    virtual int GetNumWords() const = 0;
    virtual PdeWord* GetWord(int index) = 0;
    virtual int GetTextLineFlags() const = 0;
    std::wstring GetText() {
        std::wstring buffer;
        buffer.resize(GetText(nullptr, 0));
        GetText((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    PdfTextState GetTextState() {
        PdfTextState text_state;
        GetTextState(&text_state);
        return text_state;
    }
};

struct PdeText : PdeElement {
    virtual int GetText(_out_ wchar_t* buffer, int len) const = 0;
    virtual bool HasTextState() const = 0;
    virtual void GetTextState(_out_ PdfTextState* text_state) = 0;
    virtual int GetNumTextLines() const = 0;
    virtual PdeTextLine* GetTextLine(int index) = 0;
    virtual int GetNumWords() const = 0;
    virtual PdeWord* GetWord(int index) = 0;
    virtual float GetLineSpacing() const = 0;
    virtual float GetIndent() const = 0;
    virtual PdfTextStyle GetTextStyle() const = 0;
    virtual PdfTextFlags GetTextFlags() const = 0;
    virtual int GetLabelLevel() const = 0;
    virtual bool SetLabelLevel(int level) = 0;
    std::wstring GetText() {
        std::wstring buffer;
        buffer.resize(GetText(nullptr, 0));
        GetText((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    PdfTextState GetTextState() {
        PdfTextState text_state;
        GetTextState(&text_state);
        return text_state;
    }
};

struct PdfColorSpace {
    virtual int GetName(_out_ wchar_t* buffer, int len) const = 0;
    virtual PdfColorSpaceFamily GetFamilyType() const = 0;
    virtual int GetNumComps() const = 0;
    virtual PdfColor* CreateColor() = 0;
    std::wstring GetName() {
        std::wstring buffer;
        buffer.resize(GetName(nullptr, 0));
        GetName((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
};

struct PdfColor {
    virtual PdfColorSpace* GetColorSpace() = 0;
    virtual void SetColorSpace(PdfColorSpace* color_space) = 0;
    virtual float GetValue(int index) const = 0;
    virtual void SetValue(int index, float value) = 0;
    virtual bool GetRGB(_out_ PdfRGB* rgb) const = 0;
    virtual bool GetCMYK(_out_ PdfCMYK* cmyk) const = 0;
    virtual bool GetGrayscale(_out_ PdfGray* gray) const = 0;
    virtual void Destroy() = 0;
    PdfRGB GetRGB() {
        PdfRGB rgb;
        GetRGB(&rgb);
        return rgb;
    }
    PdfCMYK GetCMYK() {
        PdfCMYK cmyk;
        GetCMYK(&cmyk);
        return cmyk;
    }
    PdfGray GetGrayscale() {
        PdfGray gray;
        GetGrayscale(&gray);
        return gray;
    }
};

struct PdfAction {
    virtual PdfActionType GetSubtype() const = 0;
    virtual int GetJavaScript(_out_ wchar_t* buffer, int len) const = 0;
    virtual PdsDictionary* GetObject() = 0;
    virtual int GetDestFile(_out_ wchar_t* buffer, int len) const = 0;
    virtual PdfViewDestination* GetViewDestination() = 0;
    virtual bool SetViewDestination(PdfViewDestination* view_dest) = 0;
    virtual bool CanCopy() = 0;
    virtual bool CanPaste(PdfDoc* dest_doc, void* data) = 0;
    virtual void* Copy() = 0;
    virtual PdfAction* Paste(PdfDoc* dest_doc, void* data) = 0;
    virtual void DestroyClipboardData(void* data) = 0;
    virtual int GetNumChildren() const = 0;
    virtual PdfAction* GetChild(int index) const = 0;
    virtual bool RemoveChild(int index) = 0;
    std::wstring GetJavaScript() {
        std::wstring buffer;
        buffer.resize(GetJavaScript(nullptr, 0));
        GetJavaScript((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetDestFile() {
        std::wstring buffer;
        buffer.resize(GetDestFile(nullptr, 0));
        GetDestFile((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
};

struct PdfActionHandler {
    virtual int GetType(_out_ wchar_t* buffer, int len) const = 0;
    virtual void Destroy() = 0;
    virtual void SetCanCopyProc(_callback_ PdfActionHandlerCanCopyProc proc) = 0;
    virtual void SetCopyProc(_callback_ PdfActionHandlerCopyProc proc) = 0;
    virtual void SetCanPasteProc(_callback_ PdfActionHandlerCanPasteProc proc) = 0;
    virtual void SetPasteProc(_callback_ PdfActionHandlerPasteProc proc) = 0;
    virtual void SetDestroyDataProc(_callback_ PdfActionHandlerDestroyDataProc proc) = 0;
    virtual void SetDestroyProc(_callback_ PdfActionHandlerDestroyProc proc) = 0;
    std::wstring GetType() {
        std::wstring buffer;
        buffer.resize(GetType(nullptr, 0));
        GetType((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
};

struct PdfAnnot {
    virtual PdfAnnotSubtype GetSubtype() const = 0;
    virtual PdfAnnotFlags GetFlags() const = 0;
    virtual bool SetFlags(PdfAnnotFlags flags) = 0;
    virtual void GetAppearance(_out_ PdfAnnotAppearance* appearance) = 0;
    virtual PdsStream* GetAppearanceXObject(PdfAnnotAppearanceMode mode) = 0;
    virtual bool SetAppearanceFromXObject(PdsStream* xobj, PdfAnnotAppearanceMode mode) = 0;
    virtual void GetBBox(_out_ PdfRect* bbox) const = 0;
    virtual bool PointInAnnot(const PdfPoint* point) const = 0;
    virtual bool RectInAnnot(const PdfRect* rect) const = 0;
    virtual PdsObject* GetStructObject(bool struct_parent) = 0;
    virtual PdsDictionary* GetObject() = 0;
    virtual void NotifyWillChange(const wchar_t* key) = 0;
    virtual void NotifyDidChange(const wchar_t* key, int err) = 0;
    virtual bool IsValid() const = 0;
    virtual bool IsMarkup() const = 0;
    virtual bool CanCopy() = 0;
    virtual bool CanPaste(PdfPage* dest_page, const PdfPoint* center, void* data) = 0;
    virtual void* Copy() = 0;
    virtual PdfAnnot* Paste(PdfPage* dest_page, PdfPoint* center, void* data) = 0;
    virtual void DestroyClipboardData(void* data) = 0;
    PdfAnnotAppearance GetAppearance() {
        PdfAnnotAppearance appearance;
        GetAppearance(&appearance);
        return appearance;
    }
    PdfRect GetBBox() {
        PdfRect bbox;
        GetBBox(&bbox);
        return bbox;
    }
};

struct PdfLinkAnnot : PdfAnnot {
    virtual int GetNumQuads() const = 0;
    virtual void GetQuad(int index, _out_ PdfQuad* quad) const = 0;
    virtual bool AddQuad(const PdfQuad* quad) = 0;
    virtual bool RemoveQuad(int index) = 0;
    virtual PdfAction* GetAction() = 0;
    virtual bool SetAction(PdfAction* action) = 0;
    PdfQuad GetQuad(int index) {
        PdfQuad quad;
        GetQuad(index, &quad);
        return quad;
    }
};

struct PdfMarkupAnnot : PdfAnnot {
    virtual int GetContents(_out_ wchar_t* buffer, int len) const = 0;
    virtual bool SetContents(const wchar_t* contents) = 0;
    virtual int GetAuthor(_out_ wchar_t* buffer, int len) const = 0;
    virtual bool SetAuthor(const wchar_t* author) = 0;
    virtual int GetNumReplies() const = 0;
    virtual PdfAnnot* GetReply(int index) = 0;
    virtual PdfAnnot* AddReply(const wchar_t* author, const wchar_t* text) = 0;
    std::wstring GetContents() {
        std::wstring buffer;
        buffer.resize(GetContents(nullptr, 0));
        GetContents((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetAuthor() {
        std::wstring buffer;
        buffer.resize(GetAuthor(nullptr, 0));
        GetAuthor((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
};

struct PdfTextAnnot : PdfMarkupAnnot {
};

struct PdfTextMarkupAnnot : PdfMarkupAnnot {
    virtual int GetNumQuads() const = 0;
    virtual void GetQuad(int index, _out_ PdfQuad* quad) const = 0;
    virtual bool AddQuad(const PdfQuad* quad) = 0;
    virtual bool RemoveQuad(int index) = 0;
    PdfQuad GetQuad(int index) {
        PdfQuad quad;
        GetQuad(index, &quad);
        return quad;
    }
};

struct PdfWidgetAnnot : PdfAnnot {
    virtual int GetCaption(_out_ wchar_t* buffer, int len) const = 0;
    virtual int GetFontName(_out_ wchar_t* buffer, int len) const = 0;
    virtual PdfAction* GetAction() = 0;
    virtual bool SetAction(PdfAction* action) = 0;
    virtual PdfAction* GetAAction(PdfActionEventType event) = 0;
    virtual PdfFormField* GetFormField() = 0;
    std::wstring GetCaption() {
        std::wstring buffer;
        buffer.resize(GetCaption(nullptr, 0));
        GetCaption((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetFontName() {
        std::wstring buffer;
        buffer.resize(GetFontName(nullptr, 0));
        GetFontName((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
};

struct PdfAnnotHandler {
    virtual int GetType(_out_ wchar_t* buffer, int len) const = 0;
    virtual void Destroy() = 0;
    virtual void SetCanCopyProc(_callback_ PdfAnnotHandlerCanCopyProc proc) = 0;
    virtual void SetCopyProc(_callback_ PdfAnnotHandlerCopyProc proc) = 0;
    virtual void SetCanPasteProc(_callback_ PdfAnnotHandlerCanPasteProc proc) = 0;
    virtual void SetPasteProc(_callback_ PdfAnnotHandlerPasteProc proc) = 0;
    virtual void SetDestroyDataProc(_callback_ PdfAnnotHandlerDestroyDataProc proc) = 0;
    virtual void SetDestroyProc(_callback_ PdfAnnotHandlerDestroyProc proc) = 0;
    std::wstring GetType() {
        std::wstring buffer;
        buffer.resize(GetType(nullptr, 0));
        GetType((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
};

struct PdfViewDestination {
    virtual int GetPageNum(PdfDoc* doc) const = 0;
    virtual PdfDestFitType GetFitType() const = 0;
    virtual void GetBBox(_out_ PdfRect* bbox) const = 0;
    virtual float GetZoom() const = 0;
    virtual PdsArray* GetObject() = 0;
    PdfRect GetBBox() {
        PdfRect bbox;
        GetBBox(&bbox);
        return bbox;
    }
};

struct PdfSecurityHandler {
    virtual int GetFilter(_out_ wchar_t* buffer, int len) const = 0;
    virtual void Destroy() = 0;
    std::wstring GetFilter() {
        std::wstring buffer;
        buffer.resize(GetFilter(nullptr, 0));
        GetFilter((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
};

struct PdfStandardSecurityHandler : PdfSecurityHandler {
    virtual void SetPassword(const wchar_t* password) = 0;
};

struct PdfCustomSecurityHandler : PdfSecurityHandler {
    virtual bool SetAuthorizationData(void* data) = 0;
    virtual void SetDestroyProc(_callback_ PdfSecurityDestroyProc proc) = 0;
    virtual void SetSetAuthoziationDataProc(_callback_ PdfSecuritySetAuthorizationDataProc proc) = 0;
    virtual void SetOnInitProc(_callback_ PdfSecurityOnInitProc proc) = 0;
    virtual void SetGetPermissionsProc(_callback_ PdfSecurityGetPermissionsProc proc) = 0;
    virtual void SetIsMetadataEncryptedProc(_callback_ PdfSecurityIsMetadataEncryptedProc proc) = 0;
    virtual void SetUpdateEncryptDictProc(_callback_ PdfSecurityUpdateEncryptDictProc proc) = 0;
    virtual void SetGetDecryptSizeProc(_callback_ PdfSecurityGetDecryptSizeProc proc) = 0;
    virtual void SetDecryptContentProc(_callback_ PdfSecurityDecryptContentProc proc) = 0;
    virtual void SetGetEncryptSizeProc(_callback_ PdfSecurityGetEncryptSizeProc proc) = 0;
    virtual void SetEncryptContentProc(_callback_ PdfSecurityEncryptContentProc proc) = 0;
};

struct PdfBaseDigSig {
    virtual void Destroy() = 0;
    virtual bool SetReason(const wchar_t* reason) = 0;
    virtual bool SetLocation(const wchar_t* location) = 0;
    virtual bool SetContactInfo(const wchar_t* contact) = 0;
    virtual bool SetName(const wchar_t* name) = 0;
    virtual bool SetTimeStampServer(const wchar_t* url, const wchar_t* user_name, const wchar_t* password) = 0;
    virtual bool SignDoc(PdfDoc* doc, const wchar_t* path) = 0;
};

struct PdfDigSig : PdfBaseDigSig {
    virtual bool SetPfxFile(const wchar_t* pfx_file, const wchar_t* pfx_password) = 0;
};

#if defined _WIN32 && defined _MSC_VER 
struct PdfCertDigSig : PdfBaseDigSig {
    virtual bool SetPfxFile(const wchar_t* pfx_file, const wchar_t* pfx_password) = 0;
    virtual bool SetCertContext(void* cert_context) = 0;
};
#endif

struct PdfCustomDigSig : PdfBaseDigSig {
    virtual bool RegisterDigestDataProc(_callback_ PdfDigestDataProc proc, void* data) = 0;
};

struct PdfDocUndo {
    virtual bool BeginOperation() = 0;
    virtual bool EndOperation() = 0;
    virtual int GetNumEntries() const = 0;
    virtual bool Execute() = 0;
    virtual int GetTitle(_out_ wchar_t* buffer, int len) const = 0;
    virtual void* GetData() = 0;
    std::wstring GetTitle() {
        std::wstring buffer;
        buffer.resize(GetTitle(nullptr, 0));
        GetTitle((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
};

struct PdfDoc {
    virtual bool Save(const wchar_t* path, PdfSaveFlags save_flags) = 0;
    virtual bool SaveToStream(PsStream* stream, PdfSaveFlags flags) = 0;
    virtual int Close() = 0;
    virtual bool Authorize(PdfGetAuthDataProc callback, void* data) = 0;
    virtual bool IsSecured() = 0;
    virtual void SetSecurityHandler(PdfSecurityHandler* handler) = 0;
    virtual int GetNumPages() const = 0;
    virtual PdfPage* AcquirePage(int page_num) = 0;
    virtual PdfPage* CreatePage(int index, const PdfRect* media_box) = 0;
    virtual bool DeletePages(int index_from, int index_to, _callback_ PdfCancelProc cancel_proc, void* cancel_data) = 0;
    virtual bool InsertPages(int index, const PdfDoc* doc, int index_from, int index_to, PdfPageInsertFlags insert_flags, _callback_ PdfCancelProc cancel_proc, void* cancel_data) = 0;
    virtual bool MovePage(int index_to, int index_from) = 0;
    virtual int GetNumDocumentJavaScripts() = 0;
    virtual int GetDocumentJavaScript(int index, _out_ wchar_t* buffer, int len) = 0;
    virtual int GetDocumentJavaScriptName(int index, _out_ wchar_t* buffer, int len) = 0;
    virtual int GetNumCalculatedFormFields() = 0;
    virtual PdfFormField* GetCalculatedFormField(int index) = 0;
    virtual int GetNumFormFields() = 0;
    virtual PdfFormField* GetFormField(int index) = 0;
    virtual PdfFormField* GetFormFieldByName(const wchar_t* buffer) = 0;
    virtual int GetInfo(const wchar_t* key, _out_ wchar_t* buffer, int len) = 0;
    virtual bool SetInfo(const wchar_t* key, const wchar_t* info) = 0;
    virtual PdfBookmark* GetBookmarkRoot() = 0;
    virtual PdfBookmark* CreateBookmarkRoot() = 0;
    virtual bool ApplyRedaction(_callback_ PdfCancelProc cancel_proc, void* cancel_data) = 0;
    virtual int GetNumAlternates() const = 0;
    virtual PdfAlternate* AcquireAlternate(int index) = 0;
    virtual PdsObject* CreatePdsObject(PdfObjectType type, bool indirect) = 0;
    virtual bool AddTags(_callback_ PdfCancelProc cancel_proc, void* cancel_data) = 0;
    virtual bool RemoveTags(_callback_ PdfCancelProc cancel_proc, void* cancel_data) = 0;
    virtual PdfDocTemplate* GetTemplate() = 0;
    virtual PsMetadata* GetMetadata() = 0;
    virtual int GetLang(_out_ wchar_t* buffer, int len) const = 0;
    virtual bool SetLang(const wchar_t* lang) = 0;
    virtual bool EmbedFonts(bool subset, _callback_ PdfCancelProc cancel_proc, void* cancel_data) = 0;
    virtual bool MakeAccessible(const PdfAccessibleParams* params, _callback_ PdfCancelProc cancel_proc, void* cancel_data) = 0;
    virtual PdsDictionary* GetTrailerObject() = 0;
    virtual PdsDictionary* GetRootObject() = 0;
    virtual PdsDictionary* GetInfoObject() = 0;
    virtual PdsDictionary* CreateDictObject(bool indirect) = 0;
    virtual PdsArray* CreateArrayObject(bool indirect) = 0;
    virtual PdsBoolean* CreateBooleanObject(bool indirect, bool value) = 0;
    virtual PdsName* CreateNameObject(bool indirect, const wchar_t* value) = 0;
    virtual PdsString* CreateStringObject(bool indirect, const wchar_t* value, bool hex) = 0;
    virtual PdsNumber* CreateIntObject(bool indirect, int value) = 0;
    virtual PdsNumber* CreateNumberObject(bool indirect, float value) = 0;
    virtual PdsStream* CreateStreamObject(bool indirect, PdsDictionary* dict, _in_ const uint8_t* buffer, int size) = 0;
    virtual PdsStream* CreateXObjectFromImage(PsStream* image_data, PdfImageFormat format) = 0;
    virtual PdsObject* GetObjectById(int obj_id) = 0;
    virtual PdsStructTree* CreateStructTree() = 0;
    virtual PdsStructTree* GetStructTree() = 0;
    virtual void RemoveStructTree() = 0;
    virtual void RemoveBookmarks() = 0;
    virtual bool CreateBookmarks(_callback_ PdfCancelProc cancel_proc, void* cancel_data) = 0;
    virtual bool AddFontMissingUnicode(_callback_ PdfCancelProc cancel_proc, void* cancel_data) = 0;
    virtual PdfNameTree* GetNameTree(const wchar_t* name, bool create) = 0;
    virtual void RemoveNameTree(const wchar_t* name) = 0;
    virtual int GetPageNumFromObject(PdsObject* page_dict) = 0;
    virtual PdfAnnot* GetAnnotFromObject(PdsObject* annot_dict) = 0;
    virtual PdfBookmark* GetBookmarkFromObject(PdsObject* bookmark_obj) = 0;
    virtual PdfAction* GetActionFromObject(PdsObject* action_obj) = 0;
    virtual PdfAction* GetActionFromViewDest(PdfViewDestination* dest) = 0;
    virtual PdfViewDestination* GetViewDestinationFromObject(PdsObject* dest_obj) = 0;
    virtual PdfViewDestination* CreateViewDestination(int page_num, PdfDestFitType fit_type, const PdfRect* rect, float zoom) = 0;
    virtual PdsForm* CreateFormFromObject(PdsStream* stream, PdfMatrix* matrix) = 0;
    virtual PdfAction* CreateAction(PdfActionType type) = 0;
    virtual PdsContent* CreateContent() = 0;
    virtual PdfColorSpace* CreateColorSpace(PdfColorSpaceFamily cs_family) = 0;
    virtual PdfFont* CreateFont(PsSysFont* sys_font, PdfFontCharset charset, PdfFontCreateFlags flags) = 0;
    virtual PdfDocUndo* CreateUndo(const wchar_t* title, void* client_data) = 0;
    virtual int GetNumUndos() const = 0;
    virtual PdfDocUndo* GetUndo(int index) = 0;
    virtual bool ClearUndos(int index) = 0;
    virtual int GetNumRedos() const = 0;
    virtual PdfDocUndo* GetRedo(int index) = 0;
    virtual bool ClearRedos(int count) = 0;
    virtual PdfDocFlags GetFlags() = 0;
    virtual bool ClearFlags() = 0;
    virtual PdfUserPermissionFlags GetUserPermissions() = 0;
    virtual int GetVersion() = 0;
    std::wstring GetDocumentJavaScript(int index) {
        std::wstring buffer;
        buffer.resize(GetDocumentJavaScript(index, nullptr, 0));
        GetDocumentJavaScript(index, (wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetDocumentJavaScriptName(int index) {
        std::wstring buffer;
        buffer.resize(GetDocumentJavaScriptName(index, nullptr, 0));
        GetDocumentJavaScriptName(index, (wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetInfo(const wchar_t* key) {
        std::wstring buffer;
        buffer.resize(GetInfo(key, nullptr, 0));
        GetInfo(key, (wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetLang() {
        std::wstring buffer;
        buffer.resize(GetLang(nullptr, 0));
        GetLang((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
};

struct PdfDocTemplate {
    virtual bool Update(_callback_ PdfCancelProc cancel_proc, void* cancel_data) = 0;
    virtual bool LoadFromStream(PsStream* stream, PsDataFormat format) = 0;
    virtual bool SaveToStream(PsStream* stream, PsDataFormat format, PdfSaveFlags flags) = 0;
    virtual bool SetDefaults() = 0;
    virtual float GetProperty(const wchar_t* name) const = 0;
    virtual bool SetProperty(const wchar_t* name, float value) = 0;
    virtual int GetRegex(const wchar_t* name, _out_ wchar_t* buffer, int len) const = 0;
    virtual bool SetRegex(const wchar_t* name, const wchar_t* pattern) = 0;
    virtual bool AddPage(int page_num, _callback_ PdfCancelProc cancel_proc, void* cancel_data) = 0;
    virtual PdfPageTemplate* GetPageTemplate(int page_num) = 0;
    std::wstring GetRegex(const wchar_t* name) {
        std::wstring buffer;
        buffer.resize(GetRegex(name, nullptr, 0));
        GetRegex(name, (wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
};

struct PdfPageTemplate {
    virtual int GetPageNum() const = 0;
    virtual PdfRotate GetLogicalRotate() const = 0;
    virtual int GetNumColumns() const = 0;
    virtual bool GetHeaderBBox(_out_ PdfRect* bbox) const = 0;
    virtual bool GetFooterBBox(_out_ PdfRect* bbox) const = 0;
    PdfRect GetHeaderBBox() {
        PdfRect bbox;
        GetHeaderBBox(&bbox);
        return bbox;
    }
    PdfRect GetFooterBBox() {
        PdfRect bbox;
        GetFooterBBox(&bbox);
        return bbox;
    }
};

struct PdfAlternate {
    virtual PdfAlternateType GetSubtype() const = 0;
    virtual int GetName(_out_ wchar_t* buffer, int len) const = 0;
    virtual int GetDescription(_out_ wchar_t* buffer, int len) const = 0;
    virtual int GetFileName(_out_ wchar_t* buffer, int len) const = 0;
    virtual bool SaveContent(const wchar_t* path) = 0;
    virtual int Release() = 0;
    std::wstring GetName() {
        std::wstring buffer;
        buffer.resize(GetName(nullptr, 0));
        GetName((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetDescription() {
        std::wstring buffer;
        buffer.resize(GetDescription(nullptr, 0));
        GetDescription((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetFileName() {
        std::wstring buffer;
        buffer.resize(GetFileName(nullptr, 0));
        GetFileName((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
};

struct PdfHtmlAlternate : PdfAlternate {
    virtual bool SaveResource(const wchar_t* resource_name, const wchar_t* path) = 0;
};

struct PdfFont {
    virtual int GetFontName(_out_ wchar_t* buffer, int len) const = 0;
    virtual int GetFaceName(_out_ wchar_t* buffer, int len) const = 0;
    virtual void GetFontState(_out_ PdfFontState* font_state) = 0;
    virtual int GetSystemFontName(_out_ wchar_t* buffer, int len) = 0;
    virtual PdfFontCharset GetSystemFontCharset() = 0;
    virtual bool GetSystemFontBold() = 0;
    virtual bool GetSystemFontItalic() = 0;
    virtual bool SaveToStream(PsStream* stream, PdfFontFormat format) = 0;
    std::wstring GetFontName() {
        std::wstring buffer;
        buffer.resize(GetFontName(nullptr, 0));
        GetFontName((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetFaceName() {
        std::wstring buffer;
        buffer.resize(GetFaceName(nullptr, 0));
        GetFaceName((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    PdfFontState GetFontState() {
        PdfFontState font_state;
        GetFontState(&font_state);
        return font_state;
    }
    std::wstring GetSystemFontName() {
        std::wstring buffer;
        buffer.resize(GetSystemFontName(nullptr, 0));
        GetSystemFontName((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
};

struct PdfFormField {
    virtual PdfFieldType GetType() const = 0;
    virtual PdfFieldFlags GetFlags() const = 0;
    virtual bool SetFlags(PdfFieldFlags flags) = 0;
    virtual int GetValue(_out_ wchar_t* buffer, int len) const = 0;
    virtual bool SetValue(const wchar_t* value) = 0;
    virtual int GetDefaultValue(_out_ wchar_t* buffer, int len) const = 0;
    virtual int GetFullName(_out_ wchar_t* buffer, int len) const = 0;
    virtual int GetTooltip(_out_ wchar_t* buffer, int len) const = 0;
    virtual int GetNumOptions() const = 0;
    virtual int GetOptionValue(int index, _out_ wchar_t* buffer, int len) const = 0;
    virtual int GetOptionCaption(int index, _out_ wchar_t* buffer, int len) const = 0;
    virtual PdfAction* GetAction() = 0;
    virtual PdfAction* GetAAction(PdfActionEventType event) = 0;
    virtual int GetMaxLength() const = 0;
    virtual int GetWidgetExportValue(const PdfAnnot* annot, _out_ wchar_t* buffer, int len) const = 0;
    virtual PdsDictionary* GetObject() = 0;
    virtual int GetNumExportValues() const = 0;
    virtual int GetExportValue(int index, _out_ wchar_t* buffer, int len) const = 0;
    virtual void NotifyWillChange(const wchar_t* key) = 0;
    virtual void NotifyDidChange(const wchar_t* key, int err) = 0;
    std::wstring GetValue() {
        std::wstring buffer;
        buffer.resize(GetValue(nullptr, 0));
        GetValue((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetDefaultValue() {
        std::wstring buffer;
        buffer.resize(GetDefaultValue(nullptr, 0));
        GetDefaultValue((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetFullName() {
        std::wstring buffer;
        buffer.resize(GetFullName(nullptr, 0));
        GetFullName((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetTooltip() {
        std::wstring buffer;
        buffer.resize(GetTooltip(nullptr, 0));
        GetTooltip((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetOptionValue(int index) {
        std::wstring buffer;
        buffer.resize(GetOptionValue(index, nullptr, 0));
        GetOptionValue(index, (wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetOptionCaption(int index) {
        std::wstring buffer;
        buffer.resize(GetOptionCaption(index, nullptr, 0));
        GetOptionCaption(index, (wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetWidgetExportValue(const PdfAnnot* annot) {
        std::wstring buffer;
        buffer.resize(GetWidgetExportValue(annot, nullptr, 0));
        GetWidgetExportValue(annot, (wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetExportValue(int index) {
        std::wstring buffer;
        buffer.resize(GetExportValue(index, nullptr, 0));
        GetExportValue(index, (wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
};

struct PdfPage {
    virtual int Release() = 0;
    virtual int GetRefNum() = 0;
    virtual void GetCropBox(_out_ PdfRect* crop_box) const = 0;
    virtual void GetMediaBox(_out_ PdfRect* media_box) const = 0;
    virtual PdfRotate GetRotate() const = 0;
    virtual bool SetRotate(PdfRotate rotate) = 0;
    virtual PdfRotate GetLogicalRotate() = 0;
    virtual void GetDefaultMatrix(_out_ PdfMatrix* matrix) const = 0;
    virtual int GetNumber() const = 0;
    virtual PdePageMap* AcquirePageMap() = 0;
    virtual PdfPageView* AcquirePageView(float zoom, PdfRotate rotate) = 0;
    virtual int GetNumAnnots() const = 0;
    virtual PdfAnnot* GetAnnot(int index) = 0;
    virtual bool RemoveAnnot(int index, PdfRemoveAnnotFlags flags) = 0;
    virtual bool AddAnnot(int index, PdfAnnot* annot) = 0;
    virtual PdfAnnot* CreateAnnot(PdfAnnotSubtype subtype, PdfRect* rect) = 0;
    virtual int GetNumAnnotsAtPoint(const PdfPoint* point) const = 0;
    virtual PdfAnnot* GetAnnotAtPoint(const PdfPoint* point, int index) = 0;
    virtual int GetNumAnnotsAtRect(const PdfRect* rect) const = 0;
    virtual PdfAnnot* GetAnnotAtRect(const PdfRect* rect, int index) = 0;
    virtual bool DrawContent(PdfPageRenderParams* params, _callback_ PdfCancelProc cancel_proc, void* cancel_data) = 0;
    virtual PdsContent* GetContent() = 0;
    virtual PdsDictionary* GetResources(const wchar_t* res_type, bool create) = 0;
    virtual PdsDictionary* GetObject() = 0;
    virtual bool FlattenFormXObjects() = 0;
    virtual bool FlattenAnnot(PdfAnnot* annot) = 0;
    virtual PdfPageContentFlags GetContentFlags() = 0;
    virtual bool SetContent() = 0;
    virtual PdfDoc* GetDoc() = 0;
    virtual PdsWordList* AcquireWordList(PdfWordFinderAlgorithm alg) = 0;
    virtual bool RemoveArtifacts(PdfCancelProc cancel_proc, void* cancel_data) = 0;
    virtual bool ArtifactNonTaggedContent(PdfCancelProc cancel_proc, void* cancel_data) = 0;
    virtual PdfPageFlags GetFlags() = 0;
    virtual bool ClearFlags() = 0;
    PdfRect GetCropBox() {
        PdfRect crop_box;
        GetCropBox(&crop_box);
        return crop_box;
    }
    PdfRect GetMediaBox() {
        PdfRect media_box;
        GetMediaBox(&media_box);
        return media_box;
    }
    PdfMatrix GetDefaultMatrix() {
        PdfMatrix matrix;
        GetDefaultMatrix(&matrix);
        return matrix;
    }
};

struct PdePageMap {
    virtual int Release() = 0;
    virtual PdeElement* GetElement() = 0;
    virtual bool GetWhitespace(PdfWhitespaceParams* params, int index, _out_ PdfRect* bbox) = 0;
    virtual void GetBBox(_out_ PdfRect* bbox) const = 0;
    virtual bool HasElements() = 0;
    virtual bool CreateElements(_callback_ PdfCancelProc cancel_proc, void* cancel_data) = 0;
    virtual bool RemoveElements() = 0;
    virtual PdeElement* CreateElement(PdfElementType type, PdeElement* parent) = 0;
    virtual bool AddTags(PdsStructElement* element, _callback_ PdfCancelProc cancel_proc, void* cancel_data) = 0;
    virtual PdfPage* GetPage() = 0;
    virtual int GetNumArtifacts() const = 0;
    virtual PdeElement* GetArtifact(int index) = 0;
    PdfRect GetWhitespace(PdfWhitespaceParams* params, int index) {
        PdfRect bbox;
        GetWhitespace(params, index, &bbox);
        return bbox;
    }
    PdfRect GetBBox() {
        PdfRect bbox;
        GetBBox(&bbox);
        return bbox;
    }
};

struct PdfPageView {
    virtual int Release() = 0;
    virtual int GetDeviceWidth() const = 0;
    virtual int GetDeviceHeight() const = 0;
    virtual void GetDeviceMatrix(_out_ PdfMatrix* matrix) const = 0;
    virtual void RectToDevice(const PdfRect* rect, _out_ PdfDevRect* dev_rect) const = 0;
    virtual void PointToDevice(const PdfPoint* point, _out_ PdfDevPoint* dev_point) const = 0;
    virtual void RectToPage(const PdfDevRect* dev_rect, _out_ PdfRect* rect) const = 0;
    virtual void PointToPage(const PdfDevPoint* dev_point, _out_ PdfPoint* point) const = 0;
    PdfMatrix GetDeviceMatrix() {
        PdfMatrix matrix;
        GetDeviceMatrix(&matrix);
        return matrix;
    }
    PdfDevRect RectToDevice(const PdfRect* rect) {
        PdfDevRect dev_rect;
        RectToDevice(rect, &dev_rect);
        return dev_rect;
    }
    PdfDevPoint PointToDevice(const PdfPoint* point) {
        PdfDevPoint dev_point;
        PointToDevice(point, &dev_point);
        return dev_point;
    }
    PdfRect RectToPage(const PdfDevRect* dev_rect) {
        PdfRect rect;
        RectToPage(dev_rect, &rect);
        return rect;
    }
    PdfPoint PointToPage(const PdfDevPoint* dev_point) {
        PdfPoint point;
        PointToPage(dev_point, &point);
        return point;
    }
};

struct PdfBookmark {
    virtual int GetTitle(_out_ wchar_t* buffer, int len) const = 0;
    virtual void SetTitle(const wchar_t* title) = 0;
    virtual bool GetAppearance(_out_ PdfBookmarkAppearance* appearance) = 0;
    virtual bool SetAppearance(const PdfBookmarkAppearance* appearance) = 0;
    virtual PdfAction* GetAction() = 0;
    virtual bool SetAction(PdfAction* action) = 0;
    virtual int GetNumChildren() = 0;
    virtual PdfBookmark* GetChild(int index) = 0;
    virtual PdfBookmark* GetParent() = 0;
    virtual PdfBookmark* GetNext() = 0;
    virtual PdfBookmark* GetPrev() = 0;
    virtual PdsDictionary* GetObject() = 0;
    virtual bool AddChild(int index, PdfBookmark* bmk) = 0;
    virtual PdfBookmark* AddNewChild(int index, const wchar_t* title) = 0;
    virtual bool IsValid() const = 0;
    virtual PdfBookmark* RemoveChild(int index) = 0;
    virtual bool IsOpen() const = 0;
    virtual bool SetOpen(bool open) = 0;
    std::wstring GetTitle() {
        std::wstring buffer;
        buffer.resize(GetTitle(nullptr, 0));
        GetTitle((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    PdfBookmarkAppearance GetAppearance() {
        PdfBookmarkAppearance appearance;
        GetAppearance(&appearance);
        return appearance;
    }
};

struct PdfNameTree {
    virtual PdsObject* GetObject() = 0;
    virtual PdsObject* Lookup(const wchar_t* name) = 0;
};

struct PsRegex {
    virtual void Destroy() = 0;
    virtual bool SetPattern(const wchar_t* pattern) = 0;
    virtual bool Search(const wchar_t* text, int position) = 0;
    virtual int GetText(_out_ wchar_t* buffer, int len) const = 0;
    virtual int GetPosition() const = 0;
    virtual int GetLength() const = 0;
    virtual int GetNumMatches() const = 0;
    virtual int GetMatchText(int index, _out_ wchar_t* buffer, int len) const = 0;
    std::wstring GetText() {
        std::wstring buffer;
        buffer.resize(GetText(nullptr, 0));
        GetText((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetMatchText(int index) {
        std::wstring buffer;
        buffer.resize(GetMatchText(index, nullptr, 0));
        GetMatchText(index, (wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
};

struct PsStream {
    virtual void Destroy() = 0;
    virtual bool IsEof() const = 0;
    virtual int GetSize() const = 0;
    virtual bool Read(int offset, _out_ uint8_t* buffer, int size) const = 0;
    virtual bool Write(int offset, _in_ const uint8_t* buffer, int size) = 0;
    virtual int GetPos() const = 0;
    virtual bool Flush() const = 0;
    virtual void* GetStream() const = 0;
    virtual PdfStreamType GetType() const = 0;
};

struct PsFileStream : PsStream {
};

struct PsMemoryStream : PsStream {
    virtual bool Resize(int size) = 0;
};

struct PsCustomStream : PsStream {
    virtual void SetReadProc(_callback_ PsStreamReadProc proc) = 0;
    virtual void SetWriteProc(_callback_ PsStreamWriteProc proc) = 0;
    virtual void SetDestroyProc(_callback_ PsStreamDestroyProc proc) = 0;
    virtual void SetGetSizeProc(_callback_ PsStreamGetSizeProc proc) = 0;
};

struct PdsStructElement {
    virtual int GetType(bool mapped, _out_ wchar_t* buffer, int len) const = 0;
    virtual int GetActualText(_out_ wchar_t* buffer, int len) const = 0;
    virtual bool SetActualText(const wchar_t* alt) = 0;
    virtual int GetAlt(_out_ wchar_t* buffer, int len) const = 0;
    virtual bool SetAlt(const wchar_t* alt) = 0;
    virtual int GetTitle(_out_ wchar_t* buffer, int len) const = 0;
    virtual bool SetTitle(const wchar_t* title) = 0;
    virtual int GetAbbreviation(_out_ wchar_t* buffer, int len) const = 0;
    virtual int GetPageNumber() const = 0;
    virtual PdsObject* GetAttrObject(int index) = 0;
    virtual bool AddAttrObj(PdsObject* object) = 0;
    virtual void RemoveAttrObj() = 0;
    virtual PdsObject* GetElementObject() = 0;
    virtual PdsObject* GetChildObject(int index) = 0;
    virtual PdfStructElementType GetChildType(int index) = 0;
    virtual int GetChildPageNumber(int index) const = 0;
    virtual int GetChildMcid(int index) const = 0;
    virtual int GetNumAttrObjects() const = 0;
    virtual int GetNumChildren() const = 0;
    virtual PdsObject* GetParentObject() = 0;
    virtual int GetId(_out_ wchar_t* buffer, int len) const = 0;
    virtual bool SetId(const wchar_t* id) = 0;
    virtual int GetLang(_out_ wchar_t* buffer, int len) const = 0;
    virtual bool SetLang(const wchar_t* alt) = 0;
    virtual bool RemoveChild(int index) = 0;
    virtual bool AddChild(PdsStructElement* element, int index) = 0;
    virtual PdsStructElement* AddNewChild(const wchar_t* type, int index) = 0;
    virtual bool SetParent(PdsStructElement* parent, int index) = 0;
    virtual bool AddPageObject(PdsPageObject* object, int index) = 0;
    virtual bool AddAnnot(PdfAnnot* annot, int index) = 0;
    virtual PdsStructTree* GetStructTree() = 0;
    virtual bool SetType(const wchar_t* type) = 0;
    std::wstring GetType(bool mapped) {
        std::wstring buffer;
        buffer.resize(GetType(mapped, nullptr, 0));
        GetType(mapped, (wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetActualText() {
        std::wstring buffer;
        buffer.resize(GetActualText(nullptr, 0));
        GetActualText((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetAlt() {
        std::wstring buffer;
        buffer.resize(GetAlt(nullptr, 0));
        GetAlt((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetTitle() {
        std::wstring buffer;
        buffer.resize(GetTitle(nullptr, 0));
        GetTitle((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetAbbreviation() {
        std::wstring buffer;
        buffer.resize(GetAbbreviation(nullptr, 0));
        GetAbbreviation((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetId() {
        std::wstring buffer;
        buffer.resize(GetId(nullptr, 0));
        GetId((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
    std::wstring GetLang() {
        std::wstring buffer;
        buffer.resize(GetLang(nullptr, 0));
        GetLang((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
};

struct PdsClassMap {
    virtual PdsObject* GetAttrObject(const wchar_t* class_name, int index) = 0;
    virtual int GetNumAttrObjects(const wchar_t* class_name) const = 0;
    virtual PdsDictionary* GetObject() = 0;
};

struct PdsRoleMap {
    virtual bool DoesMap(const wchar_t* src, const wchar_t* dst) const = 0;
    virtual int GetDirectMap(const wchar_t* type, _out_ wchar_t* buffer, int len) const = 0;
    virtual PdsDictionary* GetObject() = 0;
    std::wstring GetDirectMap(const wchar_t* type) {
        std::wstring buffer;
        buffer.resize(GetDirectMap(type, nullptr, 0));
        GetDirectMap(type, (wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
};

struct PdsStructTree {
    virtual PdsDictionary* GetObject() = 0;
    virtual PdsClassMap* GetClassMap() = 0;
    virtual PdsClassMap* CreateClassMap() = 0;
    virtual bool RemoveClassMap() = 0;
    virtual PdsObject* GetChildObject(int index) = 0;
    virtual int GetNumChildren() const = 0;
    virtual PdsRoleMap* GetRoleMap() = 0;
    virtual PdsRoleMap* CreateRoleMap() = 0;
    virtual bool RemoveRoleMap() = 0;
    virtual PdsStructElement* GetStructElementFromObject(PdsObject* object) = 0;
    virtual bool RemoveChild(int index) = 0;
    virtual bool AddChild(PdsStructElement* element, int index) = 0;
    virtual PdsStructElement* AddNewChild(const wchar_t* type, int index) = 0;
    virtual PdfDoc* GetDoc() = 0;
    virtual bool UpdateParentTree() = 0;
};

struct PsMetadata {
    virtual bool SaveToStream(PsStream* stream) = 0;
};

struct PsEvent {
    virtual PdfEventType GetType() = 0;
    virtual PdfDoc* GetDoc() = 0;
    virtual PdfPage* GetPage() = 0;
    virtual PdsObject* GetObject() = 0;
    virtual PdfFormField* GetFormField() = 0;
    virtual float GetProgress() const = 0;
    virtual PdfDocUndo* GetUndo() = 0;
    virtual int GetName(_out_ wchar_t* buffer, int len) = 0;
    virtual int GetIndex() = 0;
    std::wstring GetName() {
        std::wstring buffer;
        buffer.resize(GetName(nullptr, 0));
        GetName((wchar_t*)buffer.c_str(), (int)buffer.size());
        return buffer;
    }
};

struct PsAuthorization {
    virtual bool SaveToStream(PsStream* stream, PsDataFormat format) = 0;
    virtual bool IsAuthorized() = 0;
    virtual bool IsAuthorizedPlatform(PdfAuthPlatform platform)  = 0;
    virtual bool IsAuthorizedOption(PdfAuthOption option) = 0;
    virtual PdfAuthorizationType GetType() const = 0;
};

struct PsAccountAuthorization : PsAuthorization {
    virtual bool Authorize(const wchar_t* email, const wchar_t* serial_number) = 0;
};

struct PsStandardAuthorization : PsAuthorization {
    virtual bool Activate(const wchar_t* serial_number) = 0;
    virtual bool Deactivate() = 0;
    virtual bool Update() = 0;
    virtual bool Reset() = 0;
};

struct PsCommand {
    virtual void Destroy() = 0;
    virtual PsCommandStatus Execute(_callback_ PdfCancelProc cancel_proc, void* cancel_data) = 0;
    virtual bool LoadFromStream(PsStream* stream, PsDataFormat format) = 0;
    virtual bool SaveToStream(PsStream* stream, PsDataFormat format) = 0;
};

struct PsRenderDeviceContext {
    virtual PsRenderDeviceType GetType() = 0;
};

struct PsImage {
    virtual void Destroy() = 0;
    virtual bool Save(const wchar_t* path, PdfImageParams* params) = 0;
    virtual bool SaveRect(const wchar_t* path, PdfImageParams* params, PdfDevRect* dev_rect) = 0;
    virtual bool SaveToStream(PsStream* stream, PdfImageParams* params) = 0;
    virtual bool SaveRectToStream(PsStream* stream, PdfImageParams* params, PdfDevRect* dev_rect) = 0;
    virtual void GetPointColor(const PdfDevPoint* point, _out_ PdfRGB* color) const = 0;
    virtual bool SaveDataToStream(PsStream* stream) const = 0;
    PdfRGB GetPointColor(const PdfDevPoint* point) {
        PdfRGB color;
        GetPointColor(point, &color);
        return color;
    }
};

struct PsSysFont {
    virtual void Destroy() = 0;
};

struct Pdfix {
    virtual void Destroy() = 0;
    virtual PsAuthorization* GetAuthorization() = 0;
    virtual PsStandardAuthorization* GetStandardAuthorization() = 0;
    virtual PsAccountAuthorization* GetAccountAuthorization() = 0;
    virtual int GetErrorType() const = 0;
    virtual const char* GetError() = 0;
    virtual void SetError(int type, const char* error) = 0;
    virtual int GetVersionMajor() const = 0;
    virtual int GetVersionMinor() const = 0;
    virtual int GetVersionPatch() const = 0;
    virtual PdfDoc* CreateDoc() = 0;
    virtual PdfDoc* OpenDoc(const wchar_t* path, const wchar_t* password) = 0;
    virtual PdfDoc* OpenDocFromStream(PsStream* stream, const wchar_t* password) = 0;
    virtual PdfDigSig* CreateDigSig() = 0;
#if defined _WIN32 && defined _MSC_VER
    virtual PdfCertDigSig* CreateCertDigSig() = 0;
#endif
    virtual PdfCustomDigSig* CreateCustomDigSig() = 0;
    virtual PdfStandardSecurityHandler* CreateStandardSecurityHandler( const wchar_t* password, const PdfStandardSecurityParams* params) = 0;
    virtual PdfCustomSecurityHandler* CreateCustomSecurityHandler(const wchar_t* name, void* client_data) = 0;
    virtual bool RegisterSecurityHandler(_callback_ PdfSecurityCreateHandlerProc proc, const wchar_t* name, void* data) = 0;
    virtual PdfAnnotHandler* RegisterAnnotHandler(const wchar_t* type) = 0;
    virtual PdfActionHandler* RegisterActionHandler(const wchar_t* type) = 0;
    virtual PsRegex* CreateRegex() = 0;
    virtual PsFileStream* CreateFileStream(const wchar_t* path, PsFileMode mode) = 0;
    virtual PsMemoryStream* CreateMemStream() = 0;
    virtual PsCustomStream* CreateCustomStream(_callback_ PsStreamReadProc read_proc, void* client_data) = 0;
    virtual bool RegisterEvent(PdfEventType type, _callback_ PdfEventProc proc, void* data) = 0;
    virtual bool UnregisterEvent(PdfEventType type, PdfEventProc proc, void* data) = 0;
    virtual PsImage* CreateImage(int width, int height, PsImageDIBFormat format) = 0;
    virtual PsRenderDeviceContext* CreateRenderDeviceContext(void* device, PsRenderDeviceType type) = 0;
    virtual bool RegisterPlugin(PdfixPlugin* plugin, const wchar_t* name) = 0;
    virtual PdfixPlugin* GetPluginByName(const wchar_t* name) = 0;
    virtual PsEvent* GetEvent() = 0;
    virtual PsCommand* CreateCommand(const wchar_t* name) = 0;
    virtual PsSysFont* FindSysFont(const wchar_t* font_family, PdfFontFlags font_flags, PdfFontCodepage codepage) = 0;
    virtual bool LoadSettingsFromStream(PsStream* settings, PsDataFormat format) = 0;
};

struct PdfixPlugin {
    virtual void Destroy() = 0;
    virtual bool Initialize(Pdfix* pdfix) = 0;
    virtual int GetVersionMajor() const = 0;
    virtual int GetVersionMinor() const = 0;
    virtual int GetVersionPatch() const = 0;
    virtual int GetPdfixVersionMajor() = 0;
    virtual int GetPdfixVersionMinor() = 0;
    virtual int GetPdfixVersionPatch() = 0;
    virtual Pdfix* GetPdfix() = 0;
};

///////////////////////////////////////////////////////////////////////////////
// Pdfix initialization

#ifdef PDFIX_STATIC_LIB
Pdfix* GetPdfix();
#else

#ifdef _WIN32
#define DLL_HANDLE HMODULE
#define PdfixLoadLibrary LoadLibraryA
#define PdfixFreeLibrary FreeLibrary
#define PdfixGetProcAddress GetProcAddress
#else //UNIX
#include <dlfcn.h>
#define DLL_HANDLE void*
#define PdfixLoadLibrary(name) dlopen(name, RTLD_NOW)
#define PdfixFreeLibrary dlclose
#define PdfixGetProcAddress dlsym
#endif //  _WIN32

// method prototypes
typedef Pdfix* (*GetPdfixProcType)( );

// initialization
extern DLL_HANDLE* Pdfix_init(const char* path);
extern void Pdfix_destroy();
// static declarations
extern DLL_HANDLE g_Pdfix_handle;
extern GetPdfixProcType GetPdfix;

class PdfixException : public std::exception {
public:
    PdfixException(){}
    const char* what() const _NOEXCEPT override {
        return (GetPdfix ? GetPdfix()->GetError() : "unknown error");
    }
    int code() const {
        return (GetPdfix ? GetPdfix()->GetErrorType() : 1);
    }
};

// static definitions Pdfix (use in the source file)
#define Pdfix_statics \
    namespace PDFixSDK { \
    GetPdfixProcType GetPdfix = nullptr;\
    DLL_HANDLE g_Pdfix_handle = nullptr;\
    void Pdfix_destroy() {\
    if (g_Pdfix_handle) PdfixFreeLibrary(g_Pdfix_handle);\
    g_Pdfix_handle = nullptr;\
    GetPdfix = nullptr;\
}\
    DLL_HANDLE* Pdfix_init(const char* path) {\
    if (g_Pdfix_handle == nullptr) g_Pdfix_handle = PdfixLoadLibrary(path);   if (!g_Pdfix_handle) return nullptr;\
    GetPdfix = (GetPdfixProcType)PdfixGetProcAddress(g_Pdfix_handle, "GetPdfix");\
    if (GetPdfix == nullptr) { Pdfix_destroy(); return nullptr; } return &g_Pdfix_handle; } \
} // namespace PDFixSDK

#ifndef Pdfix_MODULE_NAME
#if defined _WIN32
#define Pdfix_MODULE_NAME "pdfix.dll"
#elif defined __linux__
#define Pdfix_MODULE_NAME "../libs/libpdfix.so"
#elif defined __APPLE__
#define Pdfix_MODULE_NAME "./libpdfix.dylib"
#else
#error unknown platform
#endif
#endif //Pdfix_MODULE_NAME

#endif // PDFIX_STATIC_LIB
} // namespace PDFixSDK
#endif //_Pdfix_h
