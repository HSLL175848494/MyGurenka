
#define SCREEN_W			800				// 表示画面の幅
#define SCREEN_H			600				// 表示画面の高さ

#define SCREEN				0				// 表示画面
#define BACKSCREEN			1				// 裏画面
#define PAGE01				11				// 作業用画面
#define PAGE02				12				//
#define PAGE03				13				//
#define PAGE04				14				//
#define PAGE05				15				//
#define PAGE06				16				//
#define PAGE07				17				//
#define PAGE08				18				//
#define PAGE09				19				//
#define PAGE10				20				//

#define RGB_WHITE			0xffffff		// 白
#define RGB_GRAY			0x7f7f7f		// 灰色
#define RGB_BLACK			0x000000		// 黒
#define RGB_RED				0xff0000		// 赤
#define RGB_GREEN			0x00ff00		// 緑
#define RGB_BLUE			0x0000ff		// 青

#define KEY_UP				0x00000001
#define KEY_DOWN			0x00000002
#define KEY_LEFT			0x00000004
#define KEY_RIGHT			0x00000008
#define KEY_RET				0x00000010
#define KEY_ESC				0x00000020
#define KEY_SPACE			0x00000040
#define KEY_BACK			0x00000080
#define KEY_PREV			0x00000100
#define KEY_NEXT			0x00000200
#define KEY_END				0x00000400
#define KEY_HOME			0x00000800
#define KEY_PRTSC			0x00001000
#define KEY_TAB				0x00010000
#define KEY_SHIFT			0x00020000
#define KEY_CTRL			0x00040000
#define KEY_LCLICK			0x01000000
#define KEY_RCLICK			0x02000000
#define KEY_MCLICK			0x04000000

#define KEY_CURSOR			(KEY_UP|KEY_DOWN|KEY_LEFT|KEY_RIGHT)
#define KEY_DECIDE			(KEY_LCLICK|KEY_RET)
#define KEY_CANCEL			(KEY_RCLICK|KEY_ESC)
#define PUSH_KEY			0xffff

#define C_LOWER				0x01
#define C_UPPER				0x02
#define C_ALPHA				0x03
#define C_DIGIT				0x04
#define C_HEX				0x08
#define C_SET				0x10
#define C_BLANK				0x20
#define C_KANJI				0x40
#define C_KANA				0x80

#define _NULL_				0
#define _KEYWORD_			1
#define _STRING_			2
#define _CONST_				3
#define _SYMBOL_			4

#define REPLAY_NONE			0
#define REPLAY_STORE		1
#define REPLAY_LOAD			2

#define FRAMETIME			17
#define FRAMESKIP			3

#define CURSOR_HIDE_CNT		60

#define TR_LINEAR			0
#define TR_POW				1
#define TR_EXPO				2
#define TR_CIRC				3
#define TR_SINE				4
#define TR_BACK				5
#define TR_BOUNCE			6
#define TR_ELASTIC			7
#define TR_MAX				8

#define TR_IN				0
#define TR_OUT				1
#define TR_INOUT			2

#define TR_POW_IN			((TR_IN << 8) | TR_POW)
#define TR_EXPO_IN			((TR_IN << 8) | TR_EXPO)
#define TR_CIRC_IN			((TR_IN << 8) | TR_CIRC)
#define TR_SINE_IN			((TR_IN << 8) | TR_SINE)
#define TR_BACK_IN			((TR_IN << 8) | TR_BACK)
#define TR_BOUNCE_IN		((TR_IN << 8) | TR_BOUNCE)
#define TR_ELASTIC_IN		((TR_IN << 8) | TR_ELASTIC)

#define TR_POW_OUT			((TR_OUT << 8) | TR_POW)
#define TR_EXPO_OUT			((TR_OUT << 8) | TR_EXPO)
#define TR_CIRC_OUT			((TR_OUT << 8) | TR_CIRC)
#define TR_SINE_OUT			((TR_OUT << 8) | TR_SINE)
#define TR_BACK_OUT			((TR_OUT << 8) | TR_BACK)
#define TR_BOUNCE_OUT		((TR_OUT << 8) | TR_BOUNCE)
#define TR_ELASTIC_OUT		((TR_OUT << 8) | TR_ELASTIC)

#define TR_POW_INOUT		((TR_INOUT << 8) | TR_POW)
#define TR_EXPO_INOUT		((TR_INOUT << 8) | TR_EXPO)
#define TR_CIRC_INOUT		((TR_INOUT << 8) | TR_CIRC)
#define TR_SINE_INOUT		((TR_INOUT << 8) | TR_SINE)
#define TR_BACK_INOUT		((TR_INOUT << 8) | TR_BACK)
#define TR_BOUNCE_INOUT		((TR_INOUT << 8) | TR_BOUNCE)
#define TR_ELASTIC_INOUT	((TR_INOUT << 8) | TR_ELASTIC)

#define SEQ_X				0
#define SEQ_Y				1
#define SEQ_ZX				2
#define SEQ_ZY				3
#define SEQ_ROT				4
#define SEQ_LV				5
#define SEQ_MAX				6

/////////////////////////////////////////////////////////////////////////////
#define OBJ(func)				void func(TASK* task)
#define CALL(prio,func)			task_begin(prio, func, NULL, 0, -1)
#define CALLA(prio,func,arg)	task_begin(prio, func, arg, 0, -1)

#define P_INIT				0				// タスク初期化
#define P_MAIN				1				// タスクメイン
#define P_END				-1				// タスク解放

#define T_MAX				2048			// タスクの登録総数
#define T_WSIZE				128				// タスクのワークサイズ
#define T_PRI_MAX			32				// タスクのプライオリティ総数
#define T_ALL				-1				// 全タスク指定

#define T_OFF				0x00			// 初期化用
#define T_USE				0x40			// 使用フラグ
#define T_SAVE				0x80			// 終了禁止フラグ

/////////////////////////////////////////////////////////////////////////////
#define EVT(func)			int func(int msg, EVENT* ev)

#define MAX_EVENT			64				// イベントの最大登録数

#define EV_INIT				0				// 初期化
#define EV_CHECK			1				// イベント領域チェック
#define EV_ENTER			2				// イベント領域に入った
#define EV_LEAVE			3				// イベント領域を離れた
#define EV_LB_DOWN			4				// 左ボタンが押された
#define EV_RB_DOWN			5				// 右ボタンが押された
#define EV_LB_UP			6				// 左ボタンが離された
#define EV_RB_UP			7				// 右ボタンが離された
#define EV_DRAG				8				// ドラッグ
#define EV_TIMER			9				// タイマーイベント
#define EV_CURSOR			10				// カーソル入力
#define EV_USER				11				// ユーザー定義

#define IMAGE_MAX			512				// キャッシュ画像数

/////////////////////////////////////////////////////////////////////////////
// 汎用マクロ
#define HSV(h,s,v)			hsvtorgb((((h)<<16) | ((s)<<8) | (v)))
#define RGB(r,g,b)			(((r)<<16) | ((g)<<8) | (b))
#define PAL(col)			((1<<24) | col)
#define GETR(col)			(((col)>>16) & 0xff)
#define GETG(col)			(((col)>>8) & 0xff)
#define GETB(col)			((col) & 0xff)

#define FIX(x)				(65536*x)
#define FL(x)				((x)<<16)
#define FR(x)				((x)>>16)
#define BIT(x)				(1<<(x))

#define MIN(a,b)			((a < b) ? (a) : (b))
#define MAX(a,b)			((a > b) ? (a) : (b))
#define SWAP(a,b)			{ a = b - a; b -= a; a += b; }

#define RAND(x)				(rand()%(x))
#define RAND2(a,b)			(rand()%((b)-(a)+1)+(a))

#define ZERO(p,size)		memset((p),0,(size))

#define ISKANJI(c)			 ((c) >= 0x81 && (c) <= 0xFE)

/////////////////////////////////////////////////////////////////////////////
typedef struct{
	uint	id;								// データID
	uint	size;							// データサイズ
} DATALIST;

typedef struct{
	uint	index;							// 配列の開始位置
	uint	size;							// 配列の数
} MTABLE;

typedef struct _task_{
	uint			state;					// 動作フラグ
	uint			id;						// タスクＩＤ
	ushort			speed;					// 遅延実行速度
	ushort			delay;					// 遅延カウンタ
	int				phase;					// 動作ステータス
	int				pause;					// 停止フラグ
	void			(*func)();				// タスク処理関数へのポインタ
	int				data;					// タスクへの引数
	uint			begin_time;				// 起動開始時間
	uint			pause_time;				// 停止開始時間
	struct _task_*	parent;					// タスクの呼び出し元
	struct _task_*	prev;					// 同プライオリティへの前方リンク
	struct _task_*	next;					// 同プライオリティへの後方リンク
	uchar			work[T_WSIZE];			// ユーザー用ワーク
} TASK;

typedef struct _event_{
	int		(*func)();						// イベントを処理する関数
	int		data;							// 汎用データ
	bool	timer;							// タイマー通知
	bool	keyboard;						// キーボード入力
	int		id;								//
	int		*var;							//
} EVENT;

typedef struct _evlist_{
	EVENT	event[MAX_EVENT];				// イベント定義バッファ
	int		count;							// イベント登録数
	int		hold;							// イベント処理中
	EVENT*	last;							//
	EVENT*	left;							//
	EVENT*	right;							//
	int		var[16];						//
} EVLIST;

typedef struct _replay_{
	uint	key;							// キー入力
	short	x;								// ｘ座標
	short	y;								// ｙ座標
} REPLAYDATA;

typedef struct _image_{
	uchar	file[32];						// 画像ファイル名
	int		page;							// 画像メモリ
	int		back_page;						// 裏画像メモリ
	int		width;							// 幅
	int		height;							// 高さ
	int		depth;							// 色深度
	int		id;								// ＩＤ
	int		ref;							// 参照カウンタ
	bool	cache;							// キャッシュフラグ
	bool	is_file;						// 画像ファイルか
} IMAGE;

typedef struct _sprseq_{
	KEYFRAME*	key[SEQ_MAX];				// パラメータ
	int			key_count[SEQ_MAX];			// パラメータ数
	int			key_max[SEQ_MAX];			// パラメータサイズ
	int			ref;						// 参照カウンタ
} SPRSEQ;

/////////////////////////////////////////////////////////////////////////////
extern bool			exit_task;				// タスク終了フラグ
extern bool			frame_skip;				// フレームスキップ
extern bool			frame_count;			// フレームカウント
extern RECT			rc_update;				// 更新矩形
extern uint			disp_flag;				// スプライト表示
extern uint			sort_flag;				// スプライトソート

extern bool			key_mode;				// キーボード操作
extern int			hide_cursor_count;		// 自動マウス消去カウンタ
extern char			key_scan[2][256];		// キースキャンデータ
extern int			key_scan_num;			// キースキャン位置
extern uint			push_key, press_key;	// キー入力データ
extern int			mouse_ox, mouse_oy;		// １フレーム前のマウス座標
extern int			mouse_x, mouse_y;		// 現在のマウス座標
extern bool			mouse_over;				//

extern uchar		char_type[256];			//
extern uchar		token[1024];			//
extern int			token_type;				//
extern int			token_value;			//

extern TASK			task_list[T_MAX];		// タスク定義バッファ
extern TASK*		task_head[T_PRI_MAX];	// タスクの先頭ポインタ
extern TASK*		task_tail[T_PRI_MAX];	// タスクの最終ポインタ
extern int			task_count[T_PRI_MAX];	// プライオリティ別のタスク登録数
extern int			task_total;				// 登録されているタスクの総数
extern uint			task_time;				// タスクの実行時間
extern uint			task_frame;				// タスクのフレームカウント
extern TASK*		exec_task;				// 実行中のタスク
extern int			exec_prio;				// 実行中のプライオリティ

extern REPLAYDATA*	replay;					//
extern int			replay_mode;			//
extern int			replay_size;			//
extern int			replay_count;			//
extern int			replay_index;			//

extern IMAGE		image[IMAGE_MAX];		//
extern int			image_count;			//
extern uchar		cache_image;			//

extern BLTMODE		gbm;					//
extern RECT			grc, grcs, grcd;		//
extern FONTSTYLE	gfs;					//
extern MOUSE		gms;					//

extern int			err_no;					//
extern uchar*		err_msg;				//

extern int			seq_play_count;			//

/////////////////////////////////////////////////////////////////////////////
// BLTMODE 構造体の初期化
BLTMODE* TRANS(tr)				{ gbm.mode = BLT_TRANS_S; gbm.trans = tr; return &gbm; }
BLTMODE* TRANSD(tr)				{ gbm.mode = BLT_TRANS_D; gbm.trans = tr; return &gbm; }
BLTMODE* BLEND(lv)				{ gbm.mode = BLT_BLEND; gbm.level = lv; return &gbm; }
BLTMODE* BLENDT(lv, tr)			{ gbm.mode = BLT_BLENDT; gbm.trans = tr; gbm.level = lv; return &gbm; }
BLTMODE* BLENDC(lv, col)		{ gbm.mode = BLT_BLENDCOL; gbm.level = lv; gbm.color = col; return &gbm; }
BLTMODE* ALPHA(lv)				{ gbm.mode = BLT_ALPHA_S; gbm.level = lv; return &gbm; }
BLTMODE* ALPHAD(lv)				{ gbm.mode = BLT_ALPHA_D; gbm.level = lv; return &gbm; }
BLTMODE* ALPHAS(lv)				{ gbm.mode = BLT_ALPHA_SD; gbm.level = lv; return &gbm; }
BLTMODE* ALPHAC(lv, col, rng)	{ gbm.mode = BLT_ALPHA_C; gbm.level = lv; gbm.color = col; gbm.range = rng; return &gbm; }
BLTMODE* PATTERN(lv, rng)		{ gbm.mode = BLT_PATTERN; gbm.level = lv; gbm.range = rng; return &gbm; }
BLTMODE* PATBLEND(lv, rng)		{ gbm.mode = BLT_PATBLEND; gbm.level = lv; gbm.range = rng; return &gbm; }
BLTMODE* ADD(lv)				{ gbm.mode = BLT_ADD; gbm.level = lv; return &gbm; }
BLTMODE* SUB(lv)				{ gbm.mode = BLT_SUB; gbm.level = lv; return &gbm; }
BLTMODE* MUL(lv)				{ gbm.mode = BLT_MUL; gbm.level = lv; return &gbm; }
BLTMODE* AND(lv)				{ gbm.mode = BLT_AND; gbm.level = lv; return &gbm; }
BLTMODE* OR(lv)					{ gbm.mode = BLT_OR; gbm.level = lv; return &gbm; }
BLTMODE* XOR(lv)				{ gbm.mode = BLT_XOR; gbm.level = lv; return &gbm; }
BLTMODE* SCRN(lv)				{ gbm.mode = BLT_SCREEN; gbm.level = lv; return &gbm; }
BLTMODE* BRIGHT(lv)				{ gbm.mode = BLT_BRIGHTEN; gbm.level = lv; return &gbm; }
BLTMODE* DARK(lv)				{ gbm.mode = BLT_DARKEN; gbm.level = lv; return &gbm; }
BLTMODE* CHANNEL(mask)			{ gbm.mode = BLT_CHANNEL; gbm.mask = mask; return &gbm; }
BLTMODE* ACHANNEL()				{ gbm.mode = BLT_ACHANNEL; return &gbm; }
BLTMODE* COLALPHA(lv, col)		{ gbm.mode = BLT_COLALPHA; gbm.level = lv, gbm.color = col; return &gbm; }
BLTMODE* MONOC(lv, col, rng)	{ gbm.mode = BLT_MONO_C; gbm.level = lv; gbm.color = col; gbm.range = rng; return &gbm; }
BLTMODE* OVERLAY(lv)			{ gbm.mode = BLT_OVERLAY; gbm.level = lv; return &gbm; }
BLTMODE* SOFTLIGHT(lv)			{ gbm.mode = BLT_SOFT; gbm.level = lv; return &gbm; }
BLTMODE* HARDLIGHT(lv)			{ gbm.mode = BLT_HARD; gbm.level = lv; return &gbm; }
BLTMODE* DODGE(lv)				{ gbm.mode = BLT_DODGE; gbm.level = lv; return &gbm; }
BLTMODE* BURN(lv)				{ gbm.mode = BLT_BURN; gbm.level = lv; return &gbm; }

/////////////////////////////////////////////////////////////////////////////
// RECT 構造体の初期化
RECT* RC(int x, int y, int width, int height)
{
	rc_box(&grc, x, y, width, height);
	return &grc;
}

RECT* RCS(int x, int y, int width, int height)
{
	rc_box(&grcs, x, y, width, height);
	return &grcs;
}

RECT* RCD(int x, int y, int width, int height)
{
	rc_box(&grcd, x, y, width, height);
	return &grcd;
}

/////////////////////////////////////////////////////////////////////////////
// フォントの設定
void SETFONT(int n, uchar* name, int size, int angle, uint style)
{
	memset(&gfs, 0, sizeof(gfs));
	strcpy(gfs.name, name);
	gfs.size = size;
	gfs.angle = angle;
	gfs.style = style;
	font_set(n, &gfs);
}
