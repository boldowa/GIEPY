#pragma once
/**
 * @file strres_jp.h
 *   にほんごてきすとりそぉす
 */

#ifndef GUI
#define		JP_SPECIALNAME				"つきのいし"
#else
#define		JP_SPECIALNAME				"おつきみやま"
#endif
#define		OYAPIPPI				"おや？ピッピのようすが…"

/* 0 - 99 : title */
#define		GSID_JP_STR_NOT_FOUND			(GSID_JP_BEGIN +    0)
#define		GSTR_JP_STR_NOT_FOUND			"(***文字列なし***)"

#define		GSID_JP_DEBUG				(GSID_JP_BEGIN +    1)
#define		GSTR_JP_DEBUG				"デバッグ情報"
#define		GSID_JP_INFO				(GSID_JP_BEGIN +    2)
#define		GSTR_JP_INFO				"    情報    "
#define		GSID_JP_INQUIRY				(GSID_JP_BEGIN +    3)
#define		GSTR_JP_INQUIRY				" 問い合わせ "
#define		GSID_JP_WARN				(GSID_JP_BEGIN +    4)
#define		GSTR_JP_WARN				"    警告    "
#define		GSID_JP_ERROR				(GSID_JP_BEGIN +    5)
#define		GSTR_JP_ERROR				"   エラー   "
#define		GSID_JP_FATAL				(GSID_JP_BEGIN +    6)
#define		GSTR_JP_FATAL				"致命的エラー"
#define		GSID_JP_STRING				(GSID_JP_BEGIN +   99)
#define		GSTR_JP_STRING				"%s"

/* 100 - 9899 : generic */
#define		GSID_JP_MEW_LOAD_FAILED			(GSID_JP_BEGIN +  100)
#define		GSTR_JP_MEW_LOAD_FAILED			"Mewthreeモジュールの読み込みに失敗しました。セットアップをやり直してください。"
#define		GSID_JP_MEMALLOC_FAILED			(GSID_JP_BEGIN +  101)
#define		GSTR_JP_MEMALLOC_FAILED			"メモリ確保に失敗しました。お使いのパソコンでは、このプログラムの実行を継続できません。(場所:%s %d行目: %s)"
#define		GSID_JP_PROCESS_FAILED			(GSID_JP_BEGIN +  102)
#define		GSTR_JP_PROCESS_FAILED			"--- 失敗しちゃいました… ヾ(・ω・`;)ﾉぁゎゎ"
#define		GSID_JP_PROCESS_SUCCEED			(GSID_JP_BEGIN +  103)
#define		GSTR_JP_PROCESS_SUCCEED			"--- やりました (　-`ω-)=3 ﾌﾝｽｯ"
#define		GSID_JP_PROGLOGIC_ERROR			(GSID_JP_BEGIN +  104)
#define		GSTR_JP_PROGLOGIC_ERROR			"プログラム内部エラー: dst に何も入っていません。(場所:%s)"
#define		GSID_JP_UNINSTALLING			(GSID_JP_BEGIN +  105)
#define		GSTR_JP_UNINSTALLING			"%s をアンインストールします…"
#define		GSID_JP_PROCESS_ABORTED			(GSID_JP_BEGIN +  106)
#define		GSTR_JP_PROCESS_ABORTED			"--- やめろォ！"

#define		GSID_JP_OPTION_PARSE_FAILED		(GSID_JP_BEGIN +  110)
#define		GSTR_JP_OPTION_PARSE_FAILED		"オプション情報の解析に失敗しました。正しい設定を入力してください。"
#define		GSID_JP_UNKNOWN_OPTION			(GSID_JP_BEGIN +  111)
#define		GSTR_JP_UNKNOWN_OPTION			"不明なオプション \"%s\" が入力されました。"
#define		GSID_JP_OPTION_ARGMISSING		(GSID_JP_BEGIN +  112)
#define		GSTR_JP_OPTION_ARGMISSING		"\"%s\" オプションに必要な情報が不足しています。"
#define		GSID_JP_OPTION_INVALID			(GSID_JP_BEGIN +  113)
#define		GSTR_JP_OPTION_INVALID			"\"%s\" オプションに不正な値を設定しようとしています。: \"%s\""
#define		GSID_JP_OPTION_MISSPARSER		(GSID_JP_BEGIN +  114)
#define		GSTR_JP_OPTION_MISSPARSER		"プログラムエラー: オプションパーサの数が足りません。(場所: %s): "
#define		GSID_JP_OPTION_USAGE			(GSID_JP_BEGIN +  115)
#define		GSTR_JP_OPTION_USAGE			"使い方: %s [オプション] <ROM>"
#define		GSID_JP_OPTION_MOREINFO			(GSID_JP_BEGIN +  116)
#define		GSTR_JP_OPTION_MOREINFO			"'-?' もしくは '--help' オプションを使うと詳細な使い方を表示します。"

#define		GSID_JP_ROM_OPEN_FAILED			(GSID_JP_BEGIN +  150)
#define		GSTR_JP_ROM_OPEN_FAILED			"ROMファイルを開くことができませんでした。ROMが存在するか、または他のプログラムで開いていないことを確認してください。"
#define		GSID_JP_ROM_IS_NOT_SMW			(GSID_JP_BEGIN +  151)
#define		GSTR_JP_ROM_IS_NOT_SMW			"指定されたファイルはSMW ROM ファイルではありません。"
#define		GSID_JP_ROM_WRITE_FAILED		(GSID_JP_BEGIN +  152)
#define		GSTR_JP_ROM_WRITE_FAILED		"ROMの書き込みに失敗しました。記憶メディアの状態を確認してください。"

#define		GSID_JP_ASM_OPEN_FAILED			(GSID_JP_BEGIN +  160)
#define		GSTR_JP_ASM_OPEN_FAILED			"ASMファイル \"%s\" を開くことができませんでした。"

#define		GSID_JP_LIST_NOT_FOUND			(GSID_JP_BEGIN +  170)
#define		GSTR_JP_LIST_NOT_FOUND			"リストファイル \"%s\" が見つかりません。 "
#define		GSID_JP_LIST_OPEN_FAILED		(GSID_JP_BEGIN +  171)
#define		GSTR_JP_LIST_OPEN_FAILED		"リストファイル \"%s\" を開くことができませんでした。ファイルのアクセス権を確認してください。"
#define		GSID_JP_LIST_PARSE_START		(GSID_JP_BEGIN +  172)
#define		GSTR_JP_LIST_PARSE_START		"リストファイル \"%s\" の読み込みを開始します。"
#define		GSID_JP_LIST_PARSE_FAILED		(GSID_JP_BEGIN +  173)
#define		GSTR_JP_LIST_PARSE_FAILED		"リストファイルの読み込みに失敗しました。"
#define		GSID_JP_LIST_SYNTAX_ERROR		(GSID_JP_BEGIN +  174)
#define		GSTR_JP_LIST_SYNTAX_ERROR		"\"%s\": line %d: 文法エラーです。"
#define		GSID_JP_LIST_INVALID_SECTION		(GSID_JP_BEGIN +  175)
#define		GSTR_JP_LIST_INVALID_SECTION		"セクション \"%s\" は存在しません。この指定は無視されます。"
#define		GSID_JP_LIST_SECTION_MATCH		(GSID_JP_BEGIN +  176)
#define		GSTR_JP_LIST_SECTION_MATCH		"セクション \"%s\" にマッチしました。"
#define		GSID_JP_LIST_INSERT_NUM_LARGE		(GSID_JP_BEGIN +  177)
#define		GSTR_JP_LIST_INSERT_NUM_LARGE		"挿入番号が大きすぎます。(最大: $%02x)"
#define		GSID_JP_LIST_INSERT_NUM_FEW		(GSID_JP_BEGIN +  178)
#define		GSTR_JP_LIST_INSERT_NUM_FEW		"挿入番号が小さすぎます。(最小: $%02x)"
#define		GSID_JP_LIST_INSERT_CONFLICT		(GSID_JP_BEGIN +  179)
#define		GSTR_JP_LIST_INSERT_CONFLICT		"挿入先が重複しています。(Num:%02x / Grp:%1x \"%s\")"

#define		GSID_JP_UNINSTALL_NOT_INSTALLED		(GSID_JP_BEGIN +  200)
#define		GSTR_JP_UNINSTALL_NOT_INSTALLED		"このROMにはインストールされていません。"
#define		GSID_JP_UNINSTALL_RESADR_NULL		(GSID_JP_BEGIN +  201)
#define		GSTR_JP_UNINSTALL_RESADR_NULL		"リストアデータ挿入先がありません。ROMが壊れている可能性があります。"
#define		GSID_JP_UNINSTALL_DATA_BROKEN		(GSID_JP_BEGIN +  202)
#define		GSTR_JP_UNINSTALL_DATA_BROKEN		"スプライトのアンインストールに必要な情報が壊れています…。"
#define		GSID_JP_UNINSTALL_INVALID_RATS		(GSID_JP_BEGIN +  203)
#define		GSTR_JP_UNINSTALL_INVALID_RATS		"データのRATSタグが壊れています…。"
#define		GSID_JP_UNINSTALL_RATSCLEAN_NG		(GSID_JP_BEGIN +  204)
#define		GSTR_JP_UNINSTALL_RATSCLEAN_NG		"ルーチンデータのクリアに失敗しました。データが壊れている可能性があります。"
#define		GSID_JP_UNINSTALL_PROCESS_GRP		(GSID_JP_BEGIN +  205)
#define		GSTR_JP_UNINSTALL_PROCESS_GRP		"テーブルグループ%d のスプライトをアンインストール中…"
#define		GSID_JP_UNINSTALL_EXTENDED		(GSID_JP_BEGIN +  206)
#define		GSTR_JP_UNINSTALL_EXTENDED		"拡張スプライトをアンインストール中…"
#define		GSID_JP_UNINSTALL_CLUSTER		(GSID_JP_BEGIN +  207)
#define		GSTR_JP_UNINSTALL_CLUSTER		"クラスタースプライトをアンインストール中…"
#define		GSID_JP_UNINSTALL_OVERWORLD		(GSID_JP_BEGIN +  208)
#define		GSTR_JP_UNINSTALL_OVERWORLD		"Overworldスプライトをアンインストール中…"
#define		GSID_JP_UNINSTALL_SPRITES		(GSID_JP_BEGIN +  209)
#define		GSTR_JP_UNINSTALL_SPRITES		"全スプライトをアンインストールします…"
#define		GSID_JP_UNINSTALL_LIBRARIES		(GSID_JP_BEGIN +  210)
#define		GSTR_JP_UNINSTALL_LIBRARIES		"ライブラリをアンインストールします…"
#define		GSID_JP_UNINSTALL_COREDATA		(GSID_JP_BEGIN +  211)
#define		GSTR_JP_UNINSTALL_COREDATA		"コアデータをアンインストールします…"
#define		GSID_JP_UNINSTALL_RESTORE		(GSID_JP_BEGIN +  212)
#define		GSTR_JP_UNINSTALL_RESTORE		"Hijackデータを復元しています…"
#define		GSID_JP_UNINSTALL_CLEANING		(GSID_JP_BEGIN +  213)
#define		GSTR_JP_UNINSTALL_CLEANING		"RatsClean @ $%06x, 0x%x bytes."
#define		GSID_JP_UNINSTALL_TWEAKING		(GSID_JP_BEGIN +  214)
#define		GSTR_JP_UNINSTALL_TWEAKING		"Restore @ $%06x, %d bytes"
#define		GSID_JP_UNINSTALL_TWEAKS		(GSID_JP_BEGIN +  215)
#define		GSTR_JP_UNINSTALL_TWEAKS		"Tweaks情報をアンインストールします…"
#define		GSID_JP_UNINSTALL_LEAKDATA		(GSID_JP_BEGIN +  216)
#define		GSTR_JP_UNINSTALL_LEAKDATA		"コーディングエラー: クリーンの前のアンインストール(開発者にお問い合わせください)"
#define		GSID_JP_UNINSTALL_NOTIMP_RES		(GSID_JP_BEGIN +  217)
#define		GSTR_JP_UNINSTALL_NOTIMP_RES		"コーディングエラー: リストア処理なし(開発者にお問い合わせください)"

#define		GSID_JP_INSERT_PARSE_LIST		(GSID_JP_BEGIN +  220)
#define		GSTR_JP_INSERT_PARSE_LIST		"リストファイルを読み込みます…。"
#define		GSID_JP_INSERT_CREATE_WORKSP		(GSID_JP_BEGIN +  221)
#define		GSTR_JP_INSERT_CREATE_WORKSP		"作業ファイルを作成します…。"
#define		GSID_JP_INSERT_COLLECT_LIBS		(GSID_JP_BEGIN +  222)
#define		GSTR_JP_INSERT_COLLECT_LIBS		"ライブラリ情報を集めます…。"
#define		GSID_JP_INSERT_INITIALIZE_ROM		(GSID_JP_BEGIN +  223)
#define		GSTR_JP_INSERT_INITIALIZE_ROM		"ROMを初期化します…。"
#define		GSID_JP_INSERT_SPRITES			(GSID_JP_BEGIN +  224)
#define		GSTR_JP_INSERT_SPRITES			"スプライト情報を挿入します…。"
#define		GSID_JP_INSERT_EXTENDED			(GSID_JP_BEGIN +  225)
#define		GSTR_JP_INSERT_EXTENDED			"拡張スプライト情報を挿入します…。"
#define		GSID_JP_INSERT_CLUSTER			(GSID_JP_BEGIN +  226)
#define		GSTR_JP_INSERT_CLUSTER			"クラスタースプライト情報を挿入します…。"
#define		GSID_JP_INSERT_OVERWORLD		(GSID_JP_BEGIN +  227)
#define		GSTR_JP_INSERT_OVERWORLD		"(未実装)Overworldスプライト情報を挿入します…。"
#define		GSID_JP_INSERT_CLEAN_WORKSP		(GSID_JP_BEGIN +  228)
#define		GSTR_JP_INSERT_CLEAN_WORKSP		"作業ファイルを削除します…。"
#define		GSID_JP_INSERT_ASSEMBLE_ALL		(GSID_JP_BEGIN +  229)
#define		GSTR_JP_INSERT_ASSEMBLE_ALL		"全スプライトのASMをアセンブルします…。"
#define		GSID_JP_INSERT_CHK_DATA_LEAK		(GSID_JP_BEGIN +  230)
#define		GSTR_JP_INSERT_CHK_DATA_LEAK		"残存データを確認します…。"
#define		GSID_JP_INSERT_DATA_LEAK_DETECT		(GSID_JP_BEGIN +  231)
#define		GSTR_JP_INSERT_DATA_LEAK_DETECT		"残存データがあります。データを破棄して継続します。"

#define		GSID_JP_LIBRARY_FOUND			(GSID_JP_BEGIN +  240)
#define		GSTR_JP_LIBRARY_FOUND			"ライブラリ \"%s\" を検出…。"
#define		GSID_JP_LIBRARY_CANT_OPEN		(GSID_JP_BEGIN +  241)
#define		GSTR_JP_LIBRARY_CANT_OPEN		"ライブラリ \"%s\" を開くことができません。"
#define		GSID_JP_LIBRARY_NOTHING			(GSID_JP_BEGIN +  242)
#define		GSTR_JP_LIBRARY_NOTHING			"ライブラリは検出されませんでした。"
#define		GSID_JP_LIBRARY_FILE_NUMBERS		(GSID_JP_BEGIN +  243)
#define		GSTR_JP_LIBRARY_FILE_NUMBERS		"ライブラリ ファイル数: %d"
#define		GSID_JP_LIBRARY_LABEL_NUMBERS		(GSID_JP_BEGIN +  244)
#define		GSTR_JP_LIBRARY_LABEL_NUMBERS		"ライブラリ   ラベル数: %d"

#define		GSID_JP_ASSEMBLE_FILE			(GSID_JP_BEGIN +  250)
#define		GSTR_JP_ASSEMBLE_FILE			"\"%s\" をアセンブルします。"
#define		GSID_JP_ASSEMBLE_FAILED			(GSID_JP_BEGIN +  251)
#define		GSTR_JP_ASSEMBLE_FAILED			"\"%s\" のアセンブルに失敗しました…。"
#define		GSID_JP_ASSEMBLE_STR_WITH_ASMNAME	(GSID_JP_BEGIN +  252)
#define		GSTR_JP_ASSEMBLE_STR_WITH_ASMNAME	"%s: %s"
#define		GSID_JP_ASSEMBLE_CFG_NOT_FOUND		(GSID_JP_BEGIN +  253)
#define		GSTR_JP_ASSEMBLE_CFG_NOT_FOUND		"CFGファイル \"%s\" が検索パスにありません。 "
#define		GSID_JP_ASSEMBLE_EMPTY_ASM		(GSID_JP_BEGIN +  254)
#define		GSTR_JP_ASSEMBLE_EMPTY_ASM		"アセンブル後データがありません。空データを指定していると思われます。"
#define		GSID_JP_ASSEMBLE_USING_ASAR		(GSID_JP_BEGIN +  255)
#define		GSTR_JP_ASSEMBLE_USING_ASAR		"\"%s\" を ASARでアセンブルします。"
#define		GSID_JP_ASSEMBLE_USING_TRASM		(GSID_JP_BEGIN +  256)
#define		GSTR_JP_ASSEMBLE_USING_TRASM		"\"%s\" を TRASMでアセンブルします。"
#define		GSID_JP_ASSEMBLE_ASM_NOT_FOUND		(GSID_JP_BEGIN +  257)
#define		GSTR_JP_ASSEMBLE_ASM_NOT_FOUND		"ASMファイル \"%s\" が検索パスにありません。 "
#define		GSID_JP_ASSEMBLE_CFG_READ_ERROR		(GSID_JP_BEGIN +  258)
#define		GSTR_JP_ASSEMBLE_CFG_READ_ERROR		"CFGファイル \"%s\" の読み込みに失敗しました。"
/* 259 : missing number */
#define		GSID_JP_ASSEMBLE_CFG_PROC_FAILED	(GSID_JP_BEGIN +  260)
#define		GSTR_JP_ASSEMBLE_CFG_PROC_FAILED	"CFGファイル \"%s\" の処理に失敗しました…。"
#define		GSID_JP_ASSEMBLE_BIN_OPEN_ERROR		(GSID_JP_BEGIN +  261)
#define		GSTR_JP_ASSEMBLE_BIN_OPEN_ERROR		"森久保ですけど？"
#define		GSID_JP_ASSEMBLE_PIPE_OPEN_ERROR	(GSID_JP_BEGIN +  262)
#define		GSTR_JP_ASSEMBLE_PIPE_OPEN_ERROR	"パイプオープンエラー: 開発者にお問合せください。(場所: %s )"
#define		GSID_JP_ASSEMBLE_TRASM_ERR_COL		(GSID_JP_BEGIN +  263)
#define		GSTR_JP_ASSEMBLE_TRASM_ERR_COL		"TRASMのエラー情報を取得できません。システムを再セットアップしてください。"
#define		GSID_JP_ASSEMBLE_TRASM_LO_ONLY		(GSID_JP_BEGIN +  264)
#define		GSTR_JP_ASSEMBLE_TRASM_LO_ONLY		"TRASMはLoROM以外サポートしていません。このASMは動作しないかもしれません。"
#define		GSID_JP_ASSEMBLE_ENTRY_NOT_FOUND	(GSID_JP_BEGIN +  265)
#define		GSTR_JP_ASSEMBLE_ENTRY_NOT_FOUND	"INITもMAINもありません。ASMの先頭をエントリーポイントにします。"
#define		GSID_JP_ASSEMBLE_FREE_NOT_FOUND		(GSID_JP_BEGIN +  266)
#define		GSTR_JP_ASSEMBLE_FREE_NOT_FOUND		"ROM内に空きスペースが見つかりません。"

#define		GSID_JP_PARSECFG_FIELDS_TOO_FEW		(GSID_JP_BEGIN +  270)
#define		GSTR_JP_PARSECFG_FIELDS_TOO_FEW		"\"%s\": CFGファイルの情報が足りません。"
#define		GSID_JP_PARSECFG_READING_FILE		(GSID_JP_BEGIN +  271)
#define		GSTR_JP_PARSECFG_READING_FILE		"CFGファイル \"%s\" を読み込みます。"
#define		GSID_JP_PARSECFG_SYNTAX_ERROR		(GSID_JP_BEGIN +  272)
#define		GSTR_JP_PARSECFG_SYNTAX_ERROR		GSTR_JP_LIST_SYNTAX_ERROR
#define		GSID_JP_PARSECFG_OLD_FORMAT_SP		(GSID_JP_BEGIN +  273)
#define		GSTR_JP_PARSECFG_OLD_FORMAT_SP		"\"%s\": スプライト種別を特定できません(古いフォーマットのファイルです)。通常スプライトと仮定します。"
#define		GSID_JP_PARSECFG_OLD_FORMAT_SH		(GSID_JP_BEGIN +  274)
#define		GSTR_JP_PARSECFG_OLD_FORMAT_SH		"\"%s\": スプライト種別を特定できません(古いフォーマットのファイルです)。シューターと仮定します。"
#define		GSID_JP_PARSECFG_OLD_FORMAT_GEN		(GSID_JP_BEGIN +  275)
#define		GSTR_JP_PARSECFG_OLD_FORMAT_GEN		"\"%s\": スプライト種別を特定できません(古いフォーマットのファイルです)。ジェネレータと仮定します。"
#define		GSID_JP_PARSECFG_TAG_INVALID		(GSID_JP_BEGIN +  276)
#define		GSTR_JP_PARSECFG_TAG_INVALID		GSTR_JP_LIST_INVALID_SECTION

#define		GSID_JP_EXTCLS_BROKEN_RELOC_ADR		(GSID_JP_BEGIN +  280)
#define		GSTR_JP_EXTCLS_BROKEN_RELOC_ADR		"ROMの位置情報が破損しています。過去のデータからROMを復元してください。"
#define		GSID_JP_EXTCLS_EXTENDED_LOCATION	(GSID_JP_BEGIN +  281)
#define		GSTR_JP_EXTCLS_EXTENDED_LOCATION	"extended テーブル: $%06x"
#define		GSID_JP_EXTCLS_CLUSTER_LOCATION		(GSID_JP_BEGIN +  282)
#define		GSTR_JP_EXTCLS_CLUSTER_LOCATION		"cluster テーブル: $%06x"

#define		GSID_JP_SPRITES_TWEAKS_LOC		(GSID_JP_BEGIN +  290)
#define		GSTR_JP_SPRITES_TWEAKS_LOC		"Tweaks テーブル: $%06x"
#define		GSID_JP_SPRITES_TABLE_GRP_LOC		(GSID_JP_BEGIN +  291)
#define		GSTR_JP_SPRITES_TABLE_GRP_LOC		"スプライト グループ%d テーブル: $%06x"

#define		GSID_JP_INITIALIZE_OLD_TOOL		(GSID_JP_BEGIN +  300)
#define		GSTR_JP_INITIALIZE_OLD_TOOL		"ツールのバージョンがROMのシステムバージョンより古いです。ツールを最新版に更新してください。"
#define		GSID_JP_INITIALIZE_FORCE		(GSID_JP_BEGIN +  301)
#define		GSTR_JP_INITIALIZE_FORCE		"システムの強制再インストールをします。"
#define		GSID_JP_INITIALIZE_EXBYTE_QUES		(GSID_JP_BEGIN +  302)
#define		GSTR_JP_INITIALIZE_EXBYTE_QUES		"EXTRA BYTES機能を無効にしようとしています。EXTRA BYTES情報が失われますが、処理を続行しても良いですか？"
#define		GSID_JP_INITIALIZE_TERMED		(GSID_JP_BEGIN +  303)
#define		GSTR_JP_INITIALIZE_TERMED		"システムの初期化処理を中断しました。"
#define		GSID_JP_INITIALIZE_UPDATING		(GSID_JP_BEGIN +  304)
#define		GSTR_JP_INITIALIZE_UPDATING		"システムアップデートを実行します…。"
#define		GSID_JP_INITIALIZE_UPDATE_NG		(GSID_JP_BEGIN +  305)
#define		GSTR_JP_INITIALIZE_UPDATE_NG		"システムアップデートに失敗しました…。"
#define		GSID_JP_INITIALIZE_INSTALLING		(GSID_JP_BEGIN +  306)
#define		GSTR_JP_INITIALIZE_INSTALLING		"システムインストールを実行します…。"
#define		GSID_JP_INITIALIZE_VER_NOT_MATCH	(GSID_JP_BEGIN +  307)
#define		GSTR_JP_INITIALIZE_VER_NOT_MATCH	"ASMとツールのバージョン情報が合致しません。開発者にお問い合わせください。"
#define		GSID_JP_INITIALIZE_EXB_LMVER_SMALL	(GSID_JP_BEGIN +  308)
#define		GSTR_JP_INITIALIZE_EXB_LMVER_SMALL	"Lunar Magic のバージョンが 1.80 未満の為、extra bytes機能を有効にできません。"

#define		GSID_JP_COLLECT_LM_NOT_INSTALLED	(GSID_JP_BEGIN +  310)
#define		GSTR_JP_COLLECT_LM_NOT_INSTALLED	"ROMがLunar Magicで編集されていません。一度Lunar Magicで編集してください。"
#define		GSID_JP_COLLECT_UNKNOWN_CHUNK		(GSID_JP_BEGIN +  311)
#define		GSTR_JP_COLLECT_UNKNOWN_CHUNK		"不明チャンクがあります。(value: %02x %02x %02x %02x)"
#define		GSID_JP_COLLECT_LM_DOWNGRADED		(GSID_JP_BEGIN +  312)
#define		GSTR_JP_COLLECT_LM_DOWNGRADED		"Lunar Magicが1.80以下にダウングレードされている為、処理を中断します。"
#define		GSID_JP_COLLECT_BROKEN_ROM		(GSID_JP_BEGIN +  313)
#define		GSTR_JP_COLLECT_BROKEN_ROM		"ROMデータが壊れています…。"

#define		GSID_JP_CHKASM_JUDGE_TRASM		(GSID_JP_BEGIN +  320)
#define		GSTR_JP_CHKASM_JUDGE_TRASM		"\"%s\" line %d: TRASM用ASMと断定します。"
#define		GSID_JP_CHKASM_JUDGE_ASAR		(GSID_JP_BEGIN +  321)
#define		GSTR_JP_CHKASM_JUDGE_ASAR		"\"%s\" line %d: ASAR(XKAS)用ASMと断定します。"
#define		GSID_JP_CHKASM_NON_COLON_TRASM		(GSID_JP_BEGIN +  322)
#define		GSTR_JP_CHKASM_NON_COLON_TRASM		"\"%s\" コロンなし TRASM用ASMと断定します。"
#define		GSID_JP_CHKASM_COLON_ASAR		(GSID_JP_BEGIN +  323)
#define		GSTR_JP_CHKASM_COLON_ASAR		"\"%s\" コロンあり ASAR用ASMと断定します。"

#define		GSID_JP_EXBYTES_SPR_DATA_REMAP		(GSID_JP_BEGIN +  325)
#define		GSTR_JP_EXBYTES_SPR_DATA_REMAP		"Level %03X のスプライトデータが再構成されました。 ($%06x => $%06x)"
#define		GSID_JP_EXBYTES_INVALID_ADDR		(GSID_JP_BEGIN +  326)
#define		GSTR_JP_EXBYTES_INVALID_ADDR		"Levelデータのアドレスが無効です。(Level %03X: $%06x) Lunar Magic でデータを一度も編集していない場合は、編集後にやり直してください。"
#define		GSID_JP_EXBYTES_SPRTERM_NOT_FOUND	(GSID_JP_BEGIN +  327)
#define		GSTR_JP_EXBYTES_SPRTERM_NOT_FOUND	"Level %03X: スプライトデータ末端が見つかりません。おそらくデータが破損しています…。"

#define		GSID_JP_RAMEN_END			(GSID_JP_BEGIN +  400)
#define		GSTR_JP_RAMEN_END			"ラーメンできたよ(*^ﾟπﾟ)ﾁｭﾙｯ"
#define		GSID_JP_RAMEN_END_TITLE			(GSID_JP_BEGIN +  401)
#define		GSTR_JP_RAMEN_END_TITLE			"チーン"

/* Tooltip warnings */
#define		GSID_JP_TOOLTIP_OPEN_FAILED		(GSID_JP_BEGIN +  410)
#define		GSTR_JP_TOOLTIP_OPEN_FAILED	"\"%s\" を開けませんでした。ファイルのアクセス権を確認してください。"

#if defined(GUI) && isWindows

/*--- win32 menu ---*/
/* File */
#define		GSID_JP_MENU_FILE			(GSID_JP_BEGIN + 2000)
#define		GSTR_JP_MENU_FILE			"ファイル(&F)"
#define		GSID_JP_MENU_FILE_INSERT		(GSID_JP_BEGIN + 2001)
#define		GSTR_JP_MENU_FILE_INSERT		"挿入(&I)"
#define		GSID_JP_MENU_FILE_UNINSTALL		(GSID_JP_BEGIN + 2002)
#define		GSTR_JP_MENU_FILE_UNINSTALL		"アンインストール(&U)"
#define		GSID_JP_MENU_FILE_ABORT			(GSID_JP_BEGIN + 2003)
#define		GSTR_JP_MENU_FILE_ABORT			"中断(&A)"
#define		GSID_JP_MENU_FILE_EXIT			(GSID_JP_BEGIN + 2004)
#define		GSTR_JP_MENU_FILE_EXIT			"終了(&X)"

/* Option */
#define		GSID_JP_MENU_OPTION		(GSID_JP_BEGIN + 2010)
#define		GSTR_JP_MENU_OPTION		"オプション(&O)"
#define		GSID_JP_MENU_OPTION_PIXI	(GSID_JP_BEGIN + 2011)
#define		GSTR_JP_MENU_OPTION_PIXI	"PIXI 互換コードを挿入する(&P)"
#define		GSID_JP_MENU_OPTION_EBEN	(GSID_JP_BEGIN + 2012)
#define		GSTR_JP_MENU_OPTION_EBEN	"Extra Bytesを有効にする(&B)"
#define		GSID_JP_MENU_OPTION_FORCE	(GSID_JP_BEGIN + 2013)
#define		GSTR_JP_MENU_OPTION_FORCE	"強制再インストール(&F)"
#define		GSID_JP_MENU_OPTION_DEBUG	(GSID_JP_BEGIN + 2014)
#define		GSTR_JP_MENU_OPTION_DEBUG	"デバッグ情報を出力する(&S)"
#define		GSID_JP_MENU_OPTION_WARN	(GSID_JP_BEGIN + 2015)
#define		GSTR_JP_MENU_OPTION_WARN	"警告情報を出力しない(&W)"
#define		GSID_JP_MENU_OPTION_DIR		(GSID_JP_BEGIN + 2016)
#define		GSTR_JP_MENU_OPTION_DIR		"ディレクトリ オプション(&D)"
#define		GSID_JP_MENU_OPTION_SSC		(GSID_JP_BEGIN + 2017)
#define		GSTR_JP_MENU_OPTION_SSC		"カスタムツールチップを作成しない(&T)"

/* Help */
#define		GSID_JP_MENU_HELP		(GSID_JP_BEGIN + 2020)
#define		GSTR_JP_MENU_HELP		"ヘルプ(&H)"
#define		GSID_JP_MENU_HELP_VERSION	(GSID_JP_BEGIN + 2021)
#define		GSTR_JP_MENU_HELP_VERSION	"バージョン情報(&V)"

/*--- Main window ---*/
#define		GSID_JP_MAINWND_ROM_FILE	(GSID_JP_BEGIN + 2100)
#define		GSTR_JP_MAINWND_ROM_FILE	"ROM ファイル"
#define		GSID_JP_MAINWND_LIST_FILE	(GSID_JP_BEGIN + 2101)
#define		GSTR_JP_MAINWND_LIST_FILE	"リスト ファイル"
#define		GSID_JP_MAINWND_DEFINES		(GSID_JP_BEGIN + 2102)
#define		GSTR_JP_MAINWND_DEFINES		"ユーザー定義"
#define		GSID_JP_MAINWND_INSERT		(GSID_JP_BEGIN + 2103)
#define		GSTR_JP_MAINWND_INSERT		"挿入"

#define		GSID_JP_OFNFILTER_ROM_FILE	(GSID_JP_BEGIN + 2110)
#define		GSTR_JP_OFNFILTER_ROM_FILE	"SNES ROM ファイル(*.smc,*.sfc,*.bin)\0*.smc;*.sfc;*.bin\0すべてのファイル(*.*)\0*.*\0\0"
#define		GSID_JP_OFNFILTER_LIST_FILE	(GSID_JP_BEGIN + 2111)
#define		GSTR_JP_OFNFILTER_LIST_FILE	"テキストファイル(*.txt)\0*.txt\0すべてのファイル(*.*)\0*.*\0\0"
#define		GSID_JP_OFNFILTER_DEFAULT_ROM	(GSID_JP_BEGIN + 2112)
#define		GSTR_JP_OFNFILTER_DEFAULT_ROM	"rom.smc"
#define		GSID_JP_OFNFILTER_DEFAULT_LIST	(GSID_JP_BEGIN + 2113)
#define		GSTR_JP_OFNFILTER_DEFAULT_LIST	"list.txt"

#define		GSID_JP_LISTVIEW_COL_NAME	(GSID_JP_BEGIN + 2120)
#define		GSTR_JP_LISTVIEW_COL_NAME	"定義名"
#define		GSID_JP_LISTVIEW_COL_VALUE	(GSID_JP_BEGIN + 2121)
#define		GSTR_JP_LISTVIEW_COL_VALUE	"値"

/* Common button */
#define		GSID_JP_BUTTON_OK		(GSID_JP_BEGIN + 2200)
#define		GSTR_JP_BUTTON_OK		"OK"
#define		GSID_JP_BUTTON_CANCEL		(GSID_JP_BEGIN + 2201)
#define		GSTR_JP_BUTTON_CANCEL		"キャンセル"
#define		GSID_JP_BUTTON_LOAD		(GSID_JP_BEGIN + 2202)
#define		GSTR_JP_BUTTON_LOAD		"読み込み"
#define		GSID_JP_BUTTON_SAVE		(GSID_JP_BEGIN + 2203)
#define		GSTR_JP_BUTTON_SAVE		"保存"

/* Directory Option Dialog */
#define		GSID_JP_DIROPTDLG		(GSID_JP_BEGIN + 2210)
#define		GSTR_JP_DIROPTDLG		"ディレクトリ オプション"
#define		GSID_JP_DIROPTDLG_LIBRARIES	(GSID_JP_BEGIN + 2211)
#define		GSTR_JP_DIROPTDLG_LIBRARIES	"ライブラリ ディレクトリ"
#define		GSID_JP_DIROPTDLG_SPRITES	(GSID_JP_BEGIN + 2212)
#define		GSTR_JP_DIROPTDLG_SPRITES	"スプライト ディレクトリ"
#define		GSID_JP_DIROPTDLG_EXTENDEDES	(GSID_JP_BEGIN + 2213)
#define		GSTR_JP_DIROPTDLG_EXTENDEDES	"エキストラスプライト ディレクトリ"
#define		GSID_JP_DIROPTDLG_CLUSTERS	(GSID_JP_BEGIN + 2214)
#define		GSTR_JP_DIROPTDLG_CLUSTERS	"クラスタースプライト ディレクトリ"
#define		GSID_JP_DIROPTDLG_ERREMPTY	(GSID_JP_BEGIN + 2215)
#define		GSTR_JP_DIROPTDLG_ERREMPTY	"\"%s\" が空です。"

/* Information Dialog */
#define		GSID_JP_INFDLG			(GSID_JP_BEGIN + 2230)
#define		GSTR_JP_INFDLG			"情報"
#define		GSID_JP_INFDLG_VERSION		(GSID_JP_BEGIN + 2231)
#define		GSTR_JP_INFDLG_VERSION		"バージョン情報"
#define		GSID_JP_INFDLG_VER_DESC		(GSID_JP_BEGIN + 2232)
#define		GSTR_JP_INFDLG_VER_DESC		"%s バージョン %s [コード: v0x%04x]\n(C)2018 boldowa"
#define		GSID_JP_INFDLG_DEBUG		(GSID_JP_BEGIN + 2233)
#define		GSTR_JP_INFDLG_DEBUG		"デバッグ情報"
#define		GSID_JP_INFDLG_WARNING		(GSID_JP_BEGIN + 2234)
#define		GSTR_JP_INFDLG_WARNING		"警告情報"
#define		GSID_JP_INFDLG_WARNING_DESC	(GSID_JP_BEGIN + 2235)
#define		GSTR_JP_INFDLG_WARNING_DESC	"警告が出力されています。\n可能な限りこれらを取り除いてください。"
#define		GSID_JP_INFDLG_ERROR		(GSID_JP_BEGIN + 2236)
#define		GSTR_JP_INFDLG_ERROR		"エラー情報"
#define		GSID_JP_INFDLG_ERROR_DESC	(GSID_JP_BEGIN + 2237)
#define		GSTR_JP_INFDLG_ERROR_DESC	"エラーが出力されています。\nこれらを取り除いてください。"

/* Information Dialog Tabs */
#define		GSID_JP_INFDLG_TAB_LIBS		(GSID_JP_BEGIN + 2250)
#define		GSTR_JP_INFDLG_TAB_LIBS		"ライブラリ"
#define		GSID_JP_INFDLG_TAB_DEBUG	(GSID_JP_BEGIN + 2251)
#define		GSTR_JP_INFDLG_TAB_DEBUG	"デバッグ"
#define		GSID_JP_INFDLG_TAB_WARNING	(GSID_JP_BEGIN + 2252)
#define		GSTR_JP_INFDLG_TAB_WARNING	"警告"
#define		GSID_JP_INFDLG_TAB_ERROR	(GSID_JP_BEGIN + 2253)
#define		GSTR_JP_INFDLG_TAB_ERROR	"エラー"
#define		GSID_JP_INFDLG_TAB_FATAL	(GSID_JP_BEGIN + 2254)
#define		GSTR_JP_INFDLG_TAB_FATAL	"致命傷"

/* Status bar */
#define		GSID_JP_STSBAR_READY		(GSID_JP_BEGIN + 2260)
#define		GSTR_JP_STSBAR_READY		"レディ"
#define		GSID_JP_STSBAR_PROGRESS		(GSID_JP_BEGIN + 2261)
#define		GSTR_JP_STSBAR_PROGRESS		"アセンブル中 ... %d / %d"

/* input error */
#define		GSID_JP_INPUTERR_EMPTY		(GSID_JP_BEGIN + 2270)
#define		GSTR_JP_INPUTERR_EMPTY		"\"%s\" が入力されていません。"
#define		GSID_JP_INPUTERR_DUPKEY		(GSID_JP_BEGIN + 2271)
#define		GSTR_JP_INPUTERR_DUPKEY		"定義名が重複しています。"

/* Ini file error */
#define		GSID_JP_INI_LOAD_FAILED		(GSID_JP_BEGIN + 2280)
#define		GSTR_JP_INI_LOAD_FAILED		"ini ファイルの読み込みに失敗しました。初期設定で起動しますか？"
#define		GSID_JP_INI_SAVE_FAILED		(GSID_JP_BEGIN + 2281)
#define		GSTR_JP_INI_SAVE_FAILED		"ini ファイルの書き込みに失敗しました。ファイルのアクセス権を確認してください。"


#endif
