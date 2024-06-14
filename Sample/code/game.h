
#define SCREEN_W			800				// �\����ʂ̕�
#define SCREEN_H			600				// �\����ʂ̍���

#define SCREEN				0				// �\�����
#define BACKSCREEN			1				// �����
#define PAGE01				11				// ��Ɨp���
#define PAGE02				12				//
#define PAGE03				13				//
#define PAGE04				14				//
#define PAGE05				15				//
#define PAGE06				16				//
#define PAGE07				17				//
#define PAGE08				18				//
#define PAGE09				19				//
#define PAGE10				20				//

#define RGB_WHITE			0xffffff		// ��
#define RGB_GRAY			0x7f7f7f		// �D�F
#define RGB_BLACK			0x000000		// ��
#define RGB_RED				0xff0000		// ��
#define RGB_GREEN			0x00ff00		// ��
#define RGB_BLUE			0x0000ff		// ��

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

#define P_INIT				0				// �^�X�N������
#define P_MAIN				1				// �^�X�N���C��
#define P_END				-1				// �^�X�N���

#define T_MAX				2048			// �^�X�N�̓o�^����
#define T_WSIZE				128				// �^�X�N�̃��[�N�T�C�Y
#define T_PRI_MAX			32				// �^�X�N�̃v���C�I���e�B����
#define T_ALL				-1				// �S�^�X�N�w��

#define T_OFF				0x00			// �������p
#define T_USE				0x40			// �g�p�t���O
#define T_SAVE				0x80			// �I���֎~�t���O

/////////////////////////////////////////////////////////////////////////////
#define EVT(func)			int func(int msg, EVENT* ev)

#define MAX_EVENT			64				// �C�x���g�̍ő�o�^��

#define EV_INIT				0				// ������
#define EV_CHECK			1				// �C�x���g�̈�`�F�b�N
#define EV_ENTER			2				// �C�x���g�̈�ɓ�����
#define EV_LEAVE			3				// �C�x���g�̈�𗣂ꂽ
#define EV_LB_DOWN			4				// ���{�^���������ꂽ
#define EV_RB_DOWN			5				// �E�{�^���������ꂽ
#define EV_LB_UP			6				// ���{�^���������ꂽ
#define EV_RB_UP			7				// �E�{�^���������ꂽ
#define EV_DRAG				8				// �h���b�O
#define EV_TIMER			9				// �^�C�}�[�C�x���g
#define EV_CURSOR			10				// �J�[�\������
#define EV_USER				11				// ���[�U�[��`

#define IMAGE_MAX			512				// �L���b�V���摜��

/////////////////////////////////////////////////////////////////////////////
// �ėp�}�N��
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
	uint	id;								// �f�[�^ID
	uint	size;							// �f�[�^�T�C�Y
} DATALIST;

typedef struct{
	uint	index;							// �z��̊J�n�ʒu
	uint	size;							// �z��̐�
} MTABLE;

typedef struct _task_{
	uint			state;					// ����t���O
	uint			id;						// �^�X�N�h�c
	ushort			speed;					// �x�����s���x
	ushort			delay;					// �x���J�E���^
	int				phase;					// ����X�e�[�^�X
	int				pause;					// ��~�t���O
	void			(*func)();				// �^�X�N�����֐��ւ̃|�C���^
	int				data;					// �^�X�N�ւ̈���
	uint			begin_time;				// �N���J�n����
	uint			pause_time;				// ��~�J�n����
	struct _task_*	parent;					// �^�X�N�̌Ăяo����
	struct _task_*	prev;					// ���v���C�I���e�B�ւ̑O�������N
	struct _task_*	next;					// ���v���C�I���e�B�ւ̌�������N
	uchar			work[T_WSIZE];			// ���[�U�[�p���[�N
} TASK;

typedef struct _event_{
	int		(*func)();						// �C�x���g����������֐�
	int		data;							// �ėp�f�[�^
	bool	timer;							// �^�C�}�[�ʒm
	bool	keyboard;						// �L�[�{�[�h����
	int		id;								//
	int		*var;							//
} EVENT;

typedef struct _evlist_{
	EVENT	event[MAX_EVENT];				// �C�x���g��`�o�b�t�@
	int		count;							// �C�x���g�o�^��
	int		hold;							// �C�x���g������
	EVENT*	last;							//
	EVENT*	left;							//
	EVENT*	right;							//
	int		var[16];						//
} EVLIST;

typedef struct _replay_{
	uint	key;							// �L�[����
	short	x;								// �����W
	short	y;								// �����W
} REPLAYDATA;

typedef struct _image_{
	uchar	file[32];						// �摜�t�@�C����
	int		page;							// �摜������
	int		back_page;						// ���摜������
	int		width;							// ��
	int		height;							// ����
	int		depth;							// �F�[�x
	int		id;								// �h�c
	int		ref;							// �Q�ƃJ�E���^
	bool	cache;							// �L���b�V���t���O
	bool	is_file;						// �摜�t�@�C����
} IMAGE;

typedef struct _sprseq_{
	KEYFRAME*	key[SEQ_MAX];				// �p�����[�^
	int			key_count[SEQ_MAX];			// �p�����[�^��
	int			key_max[SEQ_MAX];			// �p�����[�^�T�C�Y
	int			ref;						// �Q�ƃJ�E���^
} SPRSEQ;

/////////////////////////////////////////////////////////////////////////////
extern bool			exit_task;				// �^�X�N�I���t���O
extern bool			frame_skip;				// �t���[���X�L�b�v
extern bool			frame_count;			// �t���[���J�E���g
extern RECT			rc_update;				// �X�V��`
extern uint			disp_flag;				// �X�v���C�g�\��
extern uint			sort_flag;				// �X�v���C�g�\�[�g

extern bool			key_mode;				// �L�[�{�[�h����
extern int			hide_cursor_count;		// �����}�E�X�����J�E���^
extern char			key_scan[2][256];		// �L�[�X�L�����f�[�^
extern int			key_scan_num;			// �L�[�X�L�����ʒu
extern uint			push_key, press_key;	// �L�[���̓f�[�^
extern int			mouse_ox, mouse_oy;		// �P�t���[���O�̃}�E�X���W
extern int			mouse_x, mouse_y;		// ���݂̃}�E�X���W
extern bool			mouse_over;				//

extern uchar		char_type[256];			//
extern uchar		token[1024];			//
extern int			token_type;				//
extern int			token_value;			//

extern TASK			task_list[T_MAX];		// �^�X�N��`�o�b�t�@
extern TASK*		task_head[T_PRI_MAX];	// �^�X�N�̐擪�|�C���^
extern TASK*		task_tail[T_PRI_MAX];	// �^�X�N�̍ŏI�|�C���^
extern int			task_count[T_PRI_MAX];	// �v���C�I���e�B�ʂ̃^�X�N�o�^��
extern int			task_total;				// �o�^����Ă���^�X�N�̑���
extern uint			task_time;				// �^�X�N�̎��s����
extern uint			task_frame;				// �^�X�N�̃t���[���J�E���g
extern TASK*		exec_task;				// ���s���̃^�X�N
extern int			exec_prio;				// ���s���̃v���C�I���e�B

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
// BLTMODE �\���̂̏�����
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
// RECT �\���̂̏�����
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
// �t�H���g�̐ݒ�
void SETFONT(int n, uchar* name, int size, int angle, uint style)
{
	memset(&gfs, 0, sizeof(gfs));
	strcpy(gfs.name, name);
	gfs.size = size;
	gfs.angle = angle;
	gfs.style = style;
	font_set(n, &gfs);
}
